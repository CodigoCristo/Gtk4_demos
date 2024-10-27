
#include <gtk/gtk.h>

/* Función para actualizar el estado del botón seleccionado en el label
static void on_check_button_toggled(GtkCheckButton *check_button, GtkLabel *label) {
    if (gtk_check_button_get_active(check_button)) {
        const gchar *selected_item = gtk_check_button_get_label(check_button);

        // Imprime el estado en la consola y actualiza el label
        g_print("Seleccionado: %s\n", selected_item);
        gtk_label_set_text(label, selected_item);
    }
}

*/

// Función para actualizar el estado del botón seleccionado en el label
static void on_check_button_toggled(GtkCheckButton *check_button, GtkLabel *label) {
    if (gtk_check_button_get_active(check_button)) {
        const gchar *selected_item = gtk_check_button_get_label(check_button);

        // Genera el texto con el formato "Seleccionado: ITEM..."
        gchar *status_text = g_strdup_printf("Seleccionado: %s", selected_item);

        // Imprime el estado en la consola y actualiza el label
        g_print("%s\n", status_text);
        gtk_label_set_text(label, status_text);

        // Libera la memoria del texto generado
        g_free(status_text);
    }
}

// Función para activar la aplicación y configurar la interfaz
static void on_activate(GtkApplication *app) {
    // Crea la ventana principal
    GtkWidget *main_window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(main_window), "CheckButton Group Example");
    gtk_window_set_default_size(GTK_WINDOW(main_window), 300, 200);

    // Crea una caja vertical para organizar el layout
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(main_window), vbox);

    // Crea el primer CheckButton para ITEM1
    GtkWidget *check_button1 = gtk_check_button_new_with_label("ITEM1");

    // Crea el segundo y tercer CheckButton y agrúpalos con el primero
    GtkWidget *check_button2 = gtk_check_button_new_with_label("ITEM2");
    GtkWidget *check_button3 = gtk_check_button_new_with_label("ITEM3");
    gtk_check_button_set_group(GTK_CHECK_BUTTON(check_button2), GTK_CHECK_BUTTON(check_button1));
    gtk_check_button_set_group(GTK_CHECK_BUTTON(check_button3), GTK_CHECK_BUTTON(check_button1));

    // Crea el Label para mostrar el item seleccionado
    GtkWidget *status_label = gtk_label_new("Seleccionado: Ninguno");

    // Conecta la señal para detectar cambios de estado en cada CheckButton
    g_signal_connect(check_button1, "toggled", G_CALLBACK(on_check_button_toggled), status_label);
    g_signal_connect(check_button2, "toggled", G_CALLBACK(on_check_button_toggled), status_label);
    g_signal_connect(check_button3, "toggled", G_CALLBACK(on_check_button_toggled), status_label);

    // Añade los CheckButtons y el Label a la caja
    gtk_box_append(GTK_BOX(vbox), check_button1);
    gtk_box_append(GTK_BOX(vbox), check_button2);
    gtk_box_append(GTK_BOX(vbox), check_button3);
    gtk_box_append(GTK_BOX(vbox), status_label);

    // Muestra la ventana
    gtk_window_present(GTK_WINDOW(main_window));
}

int main(int argc, char *argv[]) {
    // Crea la aplicación
    GtkApplication *app = gtk_application_new("com.example.CheckButtonGroupApp", G_APPLICATION_DEFAULT_FLAGS);

    // Conecta la señal para activar la aplicación
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    // Ejecuta la aplicación
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

