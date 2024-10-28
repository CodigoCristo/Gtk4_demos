
// Incluye GTK
#include <gtk/gtk.h>

// Función para cerrar la segunda ventana y reactivar la principal
static void on_close_second_window(GtkWidget *button, GtkWidget *main_window) {
    GtkWidget *second_window = gtk_widget_get_parent(button); 
    gtk_window_destroy(GTK_WINDOW(second_window)); // Cierra la segunda ventana
}

// Función para abrir la segunda ventana y desactivar la principal
static void on_open_second_window(GtkWidget *button, GtkWidget *main_window) {
    // Desactiva la ventana principal
    gtk_widget_set_sensitive(main_window, FALSE);

    // Crea la segunda ventana
    GtkWidget *second_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(second_window), "Segunda Ventana");
    gtk_window_set_default_size(GTK_WINDOW(second_window), 300, 200);

    // Crea el botón de cierre en la segunda ventana
    GtkWidget *close_button = gtk_button_new_with_label("Cerrar");

    // Conecta la señal para cerrar la segunda ventana y reactivar la principal
    g_signal_connect(close_button, "clicked", G_CALLBACK(on_close_second_window), main_window);
    g_signal_connect_swapped(second_window, "destroy", G_CALLBACK(gtk_widget_set_sensitive), main_window);

    // Coloca el botón en la segunda ventana y la muestra
    gtk_window_set_child(GTK_WINDOW(second_window), close_button);
    gtk_window_present(GTK_WINDOW(second_window));
}

// Función para activar la aplicación y crear la ventana principal
static void on_activate(GtkApplication *app) {
    // Crea la ventana principal
    GtkWidget *main_window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(main_window), "Ventana Principal");
    gtk_window_set_default_size(GTK_WINDOW(main_window), 400, 300);

    // Crea el botón para abrir la segunda ventana
    GtkWidget *open_button = gtk_button_new_with_label("Abrir Segunda Ventana");

    // Conecta la señal para abrir la segunda ventana
    g_signal_connect(open_button, "clicked", G_CALLBACK(on_open_second_window), main_window);

    // Coloca el botón en la ventana principal y la muestra
    gtk_window_set_child(GTK_WINDOW(main_window), open_button);
    gtk_window_present(GTK_WINDOW(main_window));
}

int main(int argc, char *argv[]) {
    // Crea la aplicación
    GtkApplication *app = gtk_application_new("com.example.GtkApplication", G_APPLICATION_DEFAULT_FLAGS);

    // Conecta la señal para activar la aplicación
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    // Ejecuta la aplicación
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

