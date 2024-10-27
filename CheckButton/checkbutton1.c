
#include <gtk/gtk.h>

// Función que se llama al cambiar el estado del checkButton
static void on_check_button_toggled(GtkCheckButton *check_button, GtkLabel *label) {
    gboolean is_active = gtk_check_button_get_active(check_button);
    const char *status = is_active ? "Seleccionado" : "No seleccionado";

    // Imprime el estado en la consola
    g_print("Estado: %s\n", status);

    // Actualiza el texto del label con el estado actual
    gtk_label_set_text(label, status);
}

// Función para activar la aplicación y configurar la interfaz
static void on_activate(GtkApplication *app) {
    // Crea la ventana principal
    GtkWidget *main_window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(main_window), "CheckButton Example");
    gtk_window_set_default_size(GTK_WINDOW(main_window), 300, 200);

    // Crea una caja vertical para organizar el layout
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(main_window), vbox);

    // Crea el CheckButton
    GtkWidget *check_button = gtk_check_button_new_with_label("Seleccionar");

    // Crea el Label para mostrar el estado
    GtkWidget *status_label = gtk_label_new("No seleccionado");

    // Conecta la señal para detectar el cambio de estado del checkButton
    g_signal_connect(check_button, "toggled", G_CALLBACK(on_check_button_toggled), status_label);

    // Añade el CheckButton y el Label a la caja
    gtk_box_append(GTK_BOX(vbox), check_button);
    gtk_box_append(GTK_BOX(vbox), status_label);

    // Muestra la ventana
    gtk_window_present(GTK_WINDOW(main_window));
}

int main(int argc, char *argv[]) {
    // Crea la aplicación
    GtkApplication *app = gtk_application_new("com.example.CheckButtonApp", G_APPLICATION_DEFAULT_FLAGS);

    // Conecta la señal para activar la aplicación
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    // Ejecuta la aplicación
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

