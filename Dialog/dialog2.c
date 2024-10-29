
#include <gtk/gtk.h>

// Función que destruye la ventana principal de manera segura
static gboolean destroy_main_window(gpointer user_data) {
    GtkWindow *window = GTK_WINDOW(user_data);

    // Verifica si la instancia de la ventana es válida antes de destruirla
    if (G_IS_OBJECT(window)) {
        gtk_window_destroy(window);
    }

    return G_SOURCE_REMOVE; // Elimina esta función de la cola de eventos para que solo se ejecute una vez
}

// Callback para manejar la respuesta del diálogo de alerta
static void on_dialog_response(GtkAlertDialog *dialog, GAsyncResult *result, gpointer user_data) {
    int response = gtk_alert_dialog_choose_finish(dialog, result, NULL);
    GtkWindow *window = GTK_WINDOW(user_data);
    
    // Si la respuesta es "OK" (0), se destruye la ventana principal usando destroy_main_window
    if (response == 0) {
        g_idle_add(destroy_main_window, window);
    }

    // Libera la memoria asociada con el diálogo
    g_object_unref(dialog);
}

// Función para mostrar un diálogo de confirmación con GtkAlertDialog
static void show_question_dialog(GtkWindow *parent) {
    // Crea un nuevo GtkAlertDialog con un mensaje principal
    GtkAlertDialog *alert_dialog = gtk_alert_dialog_new(
        "¿Está seguro de que desea continuar?",
        NULL
    );

    // Establece un mensaje secundario en el diálogo
    gtk_alert_dialog_set_detail(alert_dialog, "Esta acción no se puede deshacer.");

    // Define los botones del diálogo y sus etiquetas
    gtk_alert_dialog_set_buttons(alert_dialog, (const char*[]){"OK", "Cancelar", NULL});

    // Establece el botón "OK" como predeterminado
    gtk_alert_dialog_set_default_button(alert_dialog, 0);

    // Establece el botón "Cancelar" como botón de cancelación
    gtk_alert_dialog_set_cancel_button(alert_dialog, 1);
    
    // Muestra el diálogo y conecta la respuesta con la función on_dialog_response
    gtk_alert_dialog_choose(alert_dialog, GTK_WINDOW(parent), NULL, (GAsyncReadyCallback)on_dialog_response, parent);
}

// Callback que se activa cuando se presiona el botón en la HeaderBar
static void header_button_clicked(GtkWidget *button, gpointer user_data) {
    GtkWindow *window = GTK_WINDOW(user_data);
    show_question_dialog(window); // Muestra el diálogo de confirmación
}

// Función que inicializa la aplicación y crea la interfaz
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *header_bar;
    GtkWidget *button;

    // Crea la ventana principal de la aplicación
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Ejemplo de GtkAlertDialog");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);

    // Crea una GtkHeaderBar para la ventana
    header_bar = gtk_header_bar_new();
    gtk_header_bar_set_title_widget(GTK_HEADER_BAR(header_bar), gtk_label_new("Aplicación con HeaderBar"));
    gtk_header_bar_set_decoration_layout(GTK_HEADER_BAR(header_bar), "");

    // Asigna la GtkHeaderBar a la ventana principal
    gtk_window_set_titlebar(GTK_WINDOW(window), header_bar);

    // Crea un botón y lo añade a la GtkHeaderBar
    button = gtk_button_new_with_label("Salir");
    gtk_widget_add_css_class(button, "destructive-action");
    g_signal_connect(button, "clicked", G_CALLBACK(header_button_clicked), window);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), button);

    // Muestra la ventana principal
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    // Crea una nueva instancia de GtkApplication
    app = gtk_application_new("org.gtk.alertdialogexample", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    // Ejecuta la aplicación
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

