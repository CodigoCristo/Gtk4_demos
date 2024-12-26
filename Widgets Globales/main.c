#include <gtk/gtk.h>

// Estructura para almacenar los widgets globales
// Esta estructura se usa para pasar referencias a los widgets principales (entrada de texto y botón)
// entre diferentes funciones del programa.
typedef struct {
    GtkWidget *entry;  // Entrada de texto donde el usuario ingresa información.
    GtkWidget *button; // Botón que el usuario presiona para mostrar el texto ingresado.
} AppWidgets;

// Función que maneja el clic del botón
// Se ejecuta cuando el usuario hace clic en el botón.
// Lee el texto de la entrada y lo imprime en la terminal.
void on_button_clicked(GtkButton *button, gpointer user_data) {
    // Recupera la estructura de widgets desde el parámetro user_data.
    AppWidgets *app_widgets = (AppWidgets *)user_data;

    // Obtiene el texto ingresado en el campo de entrada.
    const gchar *text = gtk_editable_get_text(GTK_EDITABLE(app_widgets->entry));

    // Imprime el texto ingresado en la terminal.
    g_print("Texto ingresado: %s\n", text);
}

// Función que se ejecuta cuando la aplicación se activa
// Configura y muestra la ventana principal de la aplicación.
void on_application_activate(GtkApplication *app, gpointer user_data) {
    // Crear la ventana principal.
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Ejemplo Simple"); // Título de la ventana.
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200); // Tamaño predeterminado de la ventana.

    // Crear una estructura para almacenar los widgets principales.
    AppWidgets *app_widgets = g_new0(AppWidgets, 1);

    // Crear un contenedor vertical para organizar los widgets dentro de la ventana.
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(window), box); // Establecer el contenedor como hijo de la ventana.

    // Crear un campo de entrada de texto y añadirlo al contenedor.
    app_widgets->entry = gtk_entry_new();          // Entrada de texto para que el usuario ingrese información.
    gtk_box_append(GTK_BOX(box), app_widgets->entry); // Añadir la entrada al contenedor vertical.

    // Crear un botón con una etiqueta y añadirlo al contenedor.
    app_widgets->button = gtk_button_new_with_label("Mostrar texto"); // Botón con etiqueta "Mostrar texto".
    gtk_box_append(GTK_BOX(box), app_widgets->button);                // Añadir el botón al contenedor.

    // Conectar la señal "clicked" del botón a la función que maneja el evento.
    // Cuando el usuario haga clic en el botón, se ejecutará la función on_button_clicked.
    g_signal_connect(app_widgets->button, "clicked", G_CALLBACK(on_button_clicked), app_widgets);

    // Mostrar la ventana principal al usuario.
    gtk_window_present(GTK_WINDOW(window)); // Muestra la ventana correctamente en GTK 4.
}

// Función principal
// Configura la aplicación GTK y ejecuta el bucle principal.
int main(int argc, char **argv) {
    // Crear una nueva instancia de GtkApplication.
    // El identificador de la aplicación debe ser único, por ejemplo, "org.example.simple".
    GtkApplication *app = gtk_application_new("org.example.simple", G_APPLICATION_DEFAULT_FLAGS);

    // Conectar la señal "activate" de la aplicación a la función on_application_activate.
    // Esta señal se emite cuando se lanza la aplicación.
    g_signal_connect(app, "activate", G_CALLBACK(on_application_activate), NULL);

    // Ejecutar la aplicación. Este método entra en el bucle principal de GTK y
    // no retorna hasta que la aplicación termine.
    int status = g_application_run(G_APPLICATION(app), argc, argv);

    // Retornar el estado de salida de la aplicación.
    return status;
}

