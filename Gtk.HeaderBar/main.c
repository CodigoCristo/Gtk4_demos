
#include <gtk/gtk.h>

static void on_dialog_response(GtkDialog *dialog, gint response_id, gpointer user_data) {
    GtkWindow *window = GTK_WINDOW(user_data);
    
    if (response_id == GTK_RESPONSE_OK) {
        gtk_window_destroy(window); // Cierra la ventana si el usuario confirma
    }

    gtk_window_destroy(GTK_WINDOW(dialog)); // Destruir el diálogo después de su uso
}

// Función para crear y mostrar el diálogo de confirmación
static void show_question_dialog(GtkWindow *parent) {

    // Ignorar las advertencias sobre el uso de funciones obsoletas en este bloque
    G_GNUC_BEGIN_IGNORE_DEPRECATIONS

    GtkWidget *question_dialog = gtk_message_dialog_new(
        GTK_WINDOW(parent),
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_QUESTION,
        GTK_BUTTONS_OK_CANCEL,
        "¿Está seguro de que desea continuar?"
    );

    // Establecer la ventana como modal
    gtk_window_set_modal(GTK_WINDOW(question_dialog), TRUE);

    // Añadir texto secundario
    gtk_message_dialog_format_secondary_text(
        GTK_MESSAGE_DIALOG(question_dialog),
        "Esta acción no se puede deshacer."
    );

    // Obtener el botón de respuesta "OK" para aplicar estilos
    GtkWidget *destructive_button = gtk_dialog_get_widget_for_response(
        GTK_DIALOG(question_dialog),
        GTK_RESPONSE_OK
    );

    // Establecer el botón "OK" como respuesta predeterminada
    gtk_dialog_set_default_response(GTK_DIALOG(question_dialog), GTK_RESPONSE_OK);

    // Añadir la clase CSS "destructive-action" para darle un estilo de acción destructiva
    gtk_widget_add_css_class(destructive_button, "destructive-action");

    // Conectar la respuesta del diálogo a la función de manejo
    g_signal_connect(question_dialog, "response", G_CALLBACK(on_dialog_response), parent);

    // Mostrar el diálogo
    gtk_widget_set_visible(question_dialog, TRUE);

    G_GNUC_END_IGNORE_DEPRECATIONS
}

// Función para manejar la señal "clicked" del botón de la barra de encabezado
static void header_button_clicked(GtkWidget *button, gpointer user_data) {
    GtkWindow *window = GTK_WINDOW(user_data);
    show_question_dialog(window);
}

// Función para activar la aplicación y construir la interfaz
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *header_bar;
    GtkWidget *button;

    // Crear la ventana principal
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Ejemplo de GtkMessageDialog");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);

    // Crear la GtkHeaderBar
    header_bar = gtk_header_bar_new();
    gtk_header_bar_set_title_widget(GTK_HEADER_BAR(header_bar), gtk_label_new("Aplicación con HeaderBar"));

    // Configurar la disposición de los botones en la GtkHeaderBar para que no aparezcan
    gtk_header_bar_set_decoration_layout(GTK_HEADER_BAR(header_bar), "");

    // Asignar la GtkHeaderBar a la ventana
    gtk_window_set_titlebar(GTK_WINDOW(window), header_bar);

    // Crear un botón y añadirlo a la GtkHeaderBar
    button = gtk_button_new_with_label("Salir");
    gtk_widget_add_css_class(button, "destructive-action");
    g_signal_connect(button, "clicked", G_CALLBACK(header_button_clicked), window);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), button);

    // Mostrar la ventana
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.messagedialogexample", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

