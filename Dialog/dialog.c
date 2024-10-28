
#include <gtk/gtk.h>

// Función para cerrar la ventana principal
static void close_main_window(GtkWidget *widget, gpointer user_data) {
    GtkWindow *window = GTK_WINDOW(user_data);

    if (G_IS_OBJECT(window)) {
        g_signal_handlers_disconnect_by_data(window, window);
        gtk_window_destroy(window);
    } else {
        g_warning("La instancia de la ventana no es válida.");
    }
}

// Función para manejar el evento del botón en el "diálogo" personalizado
static void on_dialog_button_clicked(GtkWidget *button, gpointer user_data) {
    GtkWidget *dialog_window = GTK_WIDGET(user_data);
    const char *label_text = gtk_button_get_label(GTK_BUTTON(button));

    if (g_strcmp0(label_text, "Cerrar") == 0) {
        GtkWindow *main_window = g_object_get_data(G_OBJECT(dialog_window), "main_window");
        close_main_window(GTK_WIDGET(main_window), main_window);
    }

    gtk_window_destroy(GTK_WINDOW(dialog_window));
}

// Función para mostrar el "diálogo" personalizado
static void show_custom_dialog(GtkWindow *parent) {
    GtkWidget *dialog_window;
    GtkWidget *box;
    GtkWidget *label;
    GtkWidget *button_box;
    GtkWidget *close_button;
    GtkWidget *cancel_button;

    dialog_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(dialog_window), "Confirmar Cierre");
    gtk_window_set_modal(GTK_WINDOW(dialog_window), TRUE);
    gtk_window_set_transient_for(GTK_WINDOW(dialog_window), parent);
    gtk_window_set_default_size(GTK_WINDOW(dialog_window), 300, 150);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_window_set_child(GTK_WINDOW(dialog_window), box);
    gtk_widget_set_margin_bottom(GTK_WIDGET(box), 20);
    gtk_widget_set_margin_end(GTK_WIDGET(box), 20);
    gtk_widget_set_margin_start(GTK_WIDGET(box), 20);
    gtk_widget_set_margin_top(GTK_WIDGET(box), 20);

    label = gtk_label_new("¿Estás seguro de que deseas cerrar la aplicación?");
    gtk_box_append(GTK_BOX(box), label);

    button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_halign(button_box, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(button_box, TRUE);
    gtk_box_append(GTK_BOX(box), button_box);

    // Botón para cerrar
    close_button = gtk_button_new_with_label("Cerrar");
    gtk_widget_set_hexpand(close_button, TRUE);
    gtk_widget_set_halign(close_button, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class(close_button, "destructive-action");
    g_signal_connect(close_button, "clicked", G_CALLBACK(on_dialog_button_clicked), dialog_window);
    gtk_box_append(GTK_BOX(button_box), close_button);

    // Botón para cancelar
    cancel_button = gtk_button_new_with_label("Cancelar");
    gtk_widget_set_hexpand(cancel_button, TRUE);
    gtk_widget_set_halign(cancel_button, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class(cancel_button, "text-button");
    g_signal_connect(cancel_button, "clicked", G_CALLBACK(on_dialog_button_clicked), dialog_window);
    gtk_box_append(GTK_BOX(button_box), cancel_button);

    g_object_set_data(G_OBJECT(dialog_window), "main_window", parent);

    gtk_window_present(GTK_WINDOW(dialog_window));
}

// Callback para el botón que inicia el "diálogo"
static void on_button_clicked(GtkWidget *button, gpointer user_data) {
    GtkWindow *window = GTK_WINDOW(user_data);
    show_custom_dialog(window);
}

// Función para inicializar la aplicación
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *header_bar;
    GtkWidget *button;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Aplicación con Diálogo Personalizado");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);

    header_bar = gtk_header_bar_new();
    gtk_header_bar_set_title_widget(GTK_HEADER_BAR(header_bar), gtk_label_new("Aplicación"));
    gtk_header_bar_set_decoration_layout(GTK_HEADER_BAR(header_bar), "");

    gtk_window_set_titlebar(GTK_WINDOW(window), header_bar);

    button = gtk_button_new_with_label("Cerrar aplicación");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), window);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), button);

    g_signal_connect(window, "destroy", G_CALLBACK(close_main_window), window);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

