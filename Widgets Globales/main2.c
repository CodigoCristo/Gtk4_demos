#include <gtk/gtk.h>
#include <adwaita.h>

// Estructura para almacenar widgets de la aplicación
typedef struct {
    GtkWidget *button;
    GtkWidget *label;
} AppWidgets;

// Función que se ejecuta cuando el botón es presionado
static void
print_hello (GtkWidget *button,
             gpointer   user_data)
{
    // Obtiene los widgets desde la estructura
    AppWidgets *widgets = (AppWidgets *)user_data;

    // Imprime "Hola Mundo" en la consola
    g_print ("Hola Mundo\n");

    // Cambia el texto del botón
    gtk_button_set_label(GTK_BUTTON(widgets->button), "Hola Mundo");

    // Actualiza el texto del label
    gtk_label_set_text(GTK_LABEL(widgets->label), "¡Has presionado el botón!");
}

// Función de activación de la aplicación
static void
activate_cb (GtkApplication *app, gpointer user_data)
{
    // Crea una instancia de la estructura de widgets
    AppWidgets *widgets = g_new(AppWidgets, 1);

    GtkWidget *window;
    GtkWidget *box;

    // Configura la ventana principal
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Estructura de Widgets");
    gtk_window_set_default_size (GTK_WINDOW (window), 300, 200);

    // Crea una caja con orientación vertical para organizar los widgets
    box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_halign (box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (box, GTK_ALIGN_CENTER);
    gtk_window_set_child (GTK_WINDOW (window), box);

    // Crea los widgets y almacénalos en la estructura
    widgets->button = gtk_button_new_with_label ("Presiona Aquí");
    widgets->label = gtk_label_new ("¡Etiqueta inicial!");

    // Conecta la señal "clicked" del botón a la función print_hello
    // Pasa la estructura como argumento
    g_signal_connect (widgets->button, "clicked", G_CALLBACK (print_hello), widgets);

    // Añade los widgets a la caja
    gtk_box_append (GTK_BOX (box), widgets->label);
    gtk_box_append (GTK_BOX (box), widgets->button);

    // Muestra la ventana en pantalla
    gtk_window_present (GTK_WINDOW (window));
}

int
main (int argc, char *argv[])
{
    g_autoptr (AdwApplication) app = NULL;
    const gchar *appID = "github.CodigoCristo.GTK4-Demos"; // ID único de la aplicación

    app = adw_application_new (appID, G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect (app, "activate", G_CALLBACK (activate_cb), NULL);

    return g_application_run (G_APPLICATION (app), argc, argv);
}

