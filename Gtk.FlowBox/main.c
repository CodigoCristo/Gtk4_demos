
#include <gtk/gtk.h>

// Función para manejar la señal "clicked" de los botones
static void button_clicked(GtkWidget *button, gpointer user_data) {
    const char *label = gtk_button_get_label(GTK_BUTTON(button));
    g_print("Botón %s presionado\n", label);
}

// Función para activar la aplicación y construir la interfaz
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *flowbox;
    GtkWidget *button;
    GtkWidget *box;
    
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Ejemplo de GtkFlowBox");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    // Crear un GtkFlowBox
    flowbox = gtk_flow_box_new();
    gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(flowbox), 4);
    gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(flowbox), GTK_SELECTION_NONE);
    gtk_widget_set_margin_start(flowbox, 10);
    gtk_widget_set_margin_end(flowbox, 10);
    gtk_widget_set_margin_top(flowbox, 10);
    gtk_widget_set_margin_bottom(flowbox, 10);

    // Agregar botones al GtkFlowBox
    for (int i = 1; i <= 12; i++) {
        char label[20];
        snprintf(label, sizeof(label), "Botón %d", i);
        button = gtk_button_new_with_label(label);
        g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), NULL);
        gtk_flow_box_insert(GTK_FLOW_BOX(flowbox), button, -1);
    }

    // Añadir el GtkFlowBox a la ventana
    gtk_window_set_child(GTK_WINDOW(window), flowbox);
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.flowboxexample", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

