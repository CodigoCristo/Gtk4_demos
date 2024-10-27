
#include <gtk/gtk.h>

// Función que actualiza el estado de los items seleccionados
static void update_selected_items(GtkCheckButton *check_button, GtkLabel *label) {
    // Variables para almacenar el estado de cada CheckButton
    static gboolean item1_selected = FALSE;
    static gboolean item2_selected = FALSE;
    static gboolean item3_selected = FALSE;

    // Verifica cuál CheckButton disparó la señal y actualiza su estado
    const gchar *label_text = gtk_check_button_get_label(check_button);
    if (g_strcmp0(label_text, "ITEM1") == 0) {
        item1_selected = gtk_check_button_get_active(check_button);
    } else if (g_strcmp0(label_text, "ITEM2") == 0) {
        item2_selected = gtk_check_button_get_active(check_button);
    } else if (g_strcmp0(label_text, "ITEM3") == 0) {
        item3_selected = gtk_check_button_get_active(check_button);
    }

    // Genera el texto de estado
    gchar *status = g_strdup_printf("Seleccionados: %s%s%s",
        item1_selected ? "ITEM1 " : "",
        item2_selected ? "ITEM2 " : "",
        item3_selected ? "ITEM3 " : "");

    // Imprime el estado en la consola y actualiza el label
    g_print("Estado: %s\n", status);
    gtk_label_set_text(label, status);
    g_free(status);
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

    // Crea los CheckButtons para ITEM1, ITEM2 e ITEM3
    GtkWidget *check_button1 = gtk_check_button_new_with_label("ITEM1");
    GtkWidget *check_button2 = gtk_check_button_new_with_label("ITEM2");
    GtkWidget *check_button3 = gtk_check_button_new_with_label("ITEM3");

    // Crea el Label para mostrar el estado
    GtkWidget *status_label = gtk_label_new("Seleccionados:");

    // Conecta la señal de cambio de estado de cada CheckButton
    g_signal_connect(check_button1, "toggled", G_CALLBACK(update_selected_items), status_label);
    g_signal_connect(check_button2, "toggled", G_CALLBACK(update_selected_items), status_label);
    g_signal_connect(check_button3, "toggled", G_CALLBACK(update_selected_items), status_label);

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
    GtkApplication *app = gtk_application_new("com.example.CheckButtonApp", G_APPLICATION_DEFAULT_FLAGS);

    // Conecta la señal para activar la aplicación
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    // Ejecuta la aplicación
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

