
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

static GtkTextIter iter; 
static GtkTextBuffer *buffer;
static GtkWidget *button; 
static GtkTextMark *mark; 
static GtkWidget *view;
static GtkWidget *progress_bar;
static guint pulse_handler_id = 0;

// Función para pulsar la barra de progreso
static gboolean pulse_progress(gpointer user_data) {
    gtk_progress_bar_pulse(GTK_PROGRESS_BAR(progress_bar));
    return G_SOURCE_CONTINUE; // Continúa llamando a esta función periódicamente
}

// Funciones auxiliares para cambiar el texto del botón
static gboolean set_button_label_running(gpointer user_data) {
    gtk_button_set_label(GTK_BUTTON(button), "Ejecutando...");
    gtk_widget_set_sensitive(button, FALSE); // Desactiva el botón
    gtk_widget_set_visible(progress_bar, TRUE); // Muestra la barra de progreso
    
    // Inicia la animación de pulsación de la barra de progreso
    pulse_handler_id = g_timeout_add(100, pulse_progress, NULL);

    return G_SOURCE_REMOVE;
}

static gboolean set_button_label_finished(gpointer user_data) {
    gtk_button_set_label(GTK_BUTTON(button), "Comando Completo");
    gtk_widget_set_sensitive(button, TRUE); // Reactiva el botón
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), 1.0); // Completa la barra de progreso
    
    // Detiene la animación de pulsación de la barra de progreso
    if (pulse_handler_id > 0) {
        g_source_remove(pulse_handler_id);
        pulse_handler_id = 0;
    }

    gtk_widget_set_visible(progress_bar, FALSE); // Oculta la barra de progreso
    return G_SOURCE_REMOVE;
}

// Función para actualizar la vista de texto con la salida del comando
static gboolean update_text_view(gpointer data) {
    char *line = (char *)data;
    gtk_text_buffer_get_end_iter(buffer, &iter);
    gtk_text_buffer_insert(buffer, &iter, line, -1);

    // Mueve el mark al final del texto y desplaza la vista
    gtk_text_buffer_move_mark(buffer, mark, &iter);
    gtk_text_view_scroll_mark_onscreen(GTK_TEXT_VIEW(view), mark);

    g_free(line);
    return G_SOURCE_REMOVE;
}

// Función que se ejecuta en el hilo para ejecutar el comando
static gpointer execute_command(gpointer user_data) {
    FILE *fp = popen("ping -c 13 www.google.com", "r");
    if (!fp) {
        g_idle_add((GSourceFunc)update_text_view, g_strdup("Error al ejecutar el comando.\n"));
        return NULL;
    }

    // Cambia el texto del botón a "Ejecutando..." y desactiva el botón
    g_idle_add(set_button_label_running, NULL);

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        g_idle_add((GSourceFunc)update_text_view, g_strdup(line));
    }

    fclose(fp);
    g_idle_add((GSourceFunc)update_text_view, g_strdup("Ping finalizado.\n"));

    // Cambia el texto del botón a "Comando Completo" y reactiva el botón
    g_idle_add(set_button_label_finished, NULL);
    return NULL;
}

// Función que se llama cuando se hace clic en el botón para iniciar el comando
static void start_ping(GtkWidget *btn, gpointer user_data) {
    button = btn;
    g_thread_new("command_thread", execute_command, NULL);
}

// Función para activar la aplicación y construir la interfaz
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *scrolled_window;
    GtkWidget *box;
    GtkWidget *frame;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Ping Application");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);

    frame = gtk_frame_new("Ping Output");
    gtk_widget_set_margin_bottom(GTK_WIDGET(frame), 20);
    gtk_widget_set_margin_end(GTK_WIDGET(frame), 20);
    gtk_widget_set_margin_start(GTK_WIDGET(frame), 20);
    gtk_widget_set_margin_top(GTK_WIDGET(frame), 20);

    scrolled_window = gtk_scrolled_window_new();
    buffer = gtk_text_buffer_new(NULL);
    view = gtk_text_view_new_with_buffer(buffer);

    gtk_text_view_set_editable(GTK_TEXT_VIEW(view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(view), GTK_WRAP_WORD);

    gtk_text_buffer_get_end_iter(buffer, &iter);
    mark = gtk_text_buffer_create_mark(buffer, "end_mark", &iter, FALSE);

    button = gtk_button_new_with_label("Start Ping");
    progress_bar = gtk_progress_bar_new(); // Creación de la barra de progreso
    gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(progress_bar), FALSE); // Ocultar texto en la barra
    gtk_widget_set_margin_bottom(progress_bar, 10);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_append(GTK_BOX(box), progress_bar); // Agregar la barra de progreso
    gtk_box_append(GTK_BOX(box), scrolled_window);
    gtk_box_append(GTK_BOX(box), button);
    
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), view);

    gtk_widget_set_hexpand(scrolled_window, TRUE);
    gtk_widget_set_vexpand(scrolled_window, TRUE);

    gtk_frame_set_child(GTK_FRAME(frame), box);
    gtk_window_set_child(GTK_WINDOW(window), frame);
    
    g_signal_connect(button, "clicked", G_CALLBACK(start_ping), NULL);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.ping", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

