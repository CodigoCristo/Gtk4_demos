
#include <gtk/gtk.h>
#include <stdio.h>

static GtkTextIter iter; 
GtkTextBuffer *buffer;
GtkWidget *button;

void finaal(gpointer button) {
    printf("Proceso de ping finalizado\n");
}

void final(GtkWidget *button) {
    g_print("Proceso de ping finalizado\n"); // Asegúrate de usar g_print
    gtk_button_set_label(GTK_BUTTON(button), "Finalizado"); // Cambiar el texto del botón
}

static gboolean data_ready(GIOChannel *channel, GIOCondition cond, gpointer data) {
    FILE *fp = data;
    char line[256];

    if (fgets(line, sizeof line, fp)) {
        gtk_text_buffer_get_end_iter(buffer, &iter);             
        gtk_text_buffer_insert(buffer, &iter, line, -1);
        return TRUE;
        
    } else {
        fclose(fp);
            printf("hola\n");
        g_io_channel_unref(channel); // Liberar el canal
         // Pasar el botón a la función final
        return FALSE;        
    }
    final(button);

    
}

void show(GtkWidget *button, gpointer user_data) {

    FILE *fp = popen("ping -c 3 www.google.com", "r");
    if (!fp) {
        gtk_text_buffer_set_text(buffer, "Error al ejecutar el comando.\n", -1);
        return;
    }
 
    GIOChannel *channel = g_io_channel_unix_new(fileno(fp));
    g_io_add_watch_full(channel, G_PRIORITY_DEFAULT, G_IO_IN, data_ready, fp, finaal);
    
}

void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *scrolled_window;
    GtkWidget *button_box;
    
    GtkWidget *box;
    GtkWidget *view;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Ping");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);

    scrolled_window = gtk_scrolled_window_new();
    buffer = gtk_text_buffer_new(NULL);
    view = gtk_text_view_new_with_buffer(buffer);

    gtk_text_view_set_editable(GTK_TEXT_VIEW(view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(view), GTK_WRAP_WORD);

    button = gtk_button_new_with_label("Ping");
    button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(box), scrolled_window);
    gtk_box_append(GTK_BOX(box), button_box);
    
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), view);
    gtk_box_append(GTK_BOX(button_box), button);
    gtk_window_set_child(GTK_WINDOW(window), box);
    
    g_signal_connect(button, "clicked", G_CALLBACK(show), NULL);


    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char *argv[]) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.tmp", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

