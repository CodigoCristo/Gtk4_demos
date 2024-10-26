
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

static GtkTextIter iter; 
static GtkTextBuffer *buffer;
static GtkWidget *button; // Añadido para acceder al botón globalmente

static gboolean on_ping_finished(gpointer user_data) {
    gtk_text_buffer_insert(buffer, &iter, "Ping finalizado.\n", -1);
    
    // Cambiar el texto del botón
    gtk_button_set_label(GTK_BUTTON(button), "Ping Completo");
    
    return G_SOURCE_REMOVE; // Indica que no se debe volver a llamar a esta función
}

static gboolean read_ping_output(gpointer data) {
    FILE *fp = (FILE *)data;
    char line[256];

    if (fgets(line, sizeof(line), fp)) {
        gtk_text_buffer_get_end_iter(buffer, &iter);             
        gtk_text_buffer_insert(buffer, &iter, line, -1);
        return TRUE; // Continua llamando a la función
    } else {
        fclose(fp);
        g_idle_add(on_ping_finished, NULL); // Llamar a la función de finalización
        return FALSE; // Detiene la llamada a esta función
    }
}

static void start_ping(GtkWidget *btn, gpointer user_data) {
    button = btn; // Guardar el puntero al botón para poder usarlo en otras funciones
    FILE *fp = popen("ping -c 13 www.google.com", "r");
    if (!fp) {
        gtk_text_buffer_set_text(buffer, "Error al ejecutar el comando.\n", -1);
        return;
    }

    // Leer la salida del ping en el hilo principal
    g_idle_add(read_ping_output, fp);
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *scrolled_window;
    GtkWidget *box;
    GtkWidget *view;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Ping Application");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);

    scrolled_window = gtk_scrolled_window_new();
    buffer = gtk_text_buffer_new(NULL);
    view = gtk_text_view_new_with_buffer(buffer);

    gtk_text_view_set_editable(GTK_TEXT_VIEW(view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(view), GTK_WRAP_WORD);

    button = gtk_button_new_with_label("Start Ping"); // Inicialización del botón

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(box), scrolled_window);
    gtk_box_append(GTK_BOX(box), button);
    
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), view);
    gtk_window_set_child(GTK_WINDOW(window), box);
    
    g_signal_connect(button, "clicked", G_CALLBACK(start_ping), NULL);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.ping", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

