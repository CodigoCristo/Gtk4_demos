#include <gtk/gtk.h>
#include <adwaita.h>


// Función que se ejecuta cuando el botón es presionado
static void
print_hello (GtkWidget *button,
             gpointer   data)
{
  // Imprime "Hola Mundo" en la consola
  g_print ("Hola Mundo\n");

  // Cambia el texto del botón a "Hola Mundo"
  gtk_button_set_label(GTK_BUTTON(button), "Hola Mundo");
}


static void
activate_cb (GtkApplication *app)
{
  GtkWidget *window = gtk_application_window_new (app);  // Declaración de la ventana principal
  GtkWidget *button;  // Declaración del botón
  GtkWidget *box;     // Declaración de una caja para contener el botón

// Configura el título de la ventana
  gtk_window_set_title (GTK_WINDOW (window), "Window");

  // Establece el tamaño predeterminado de la ventana
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);

  // Crea una caja con orientación vertical para organizar los widgets
  box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);

  // Centra horizontal y verticalmente la caja dentro de la ventana
  gtk_widget_set_halign (box, GTK_ALIGN_CENTER);
  gtk_widget_set_valign (box, GTK_ALIGN_CENTER);

  // Agrega la caja como el widget principal de la ventana
  gtk_window_set_child (GTK_WINDOW (window), box);

  // Crea un botón con la etiqueta "Presiona Aqui"
  button = gtk_button_new_with_label ("Presiona Aqui");

  // Conecta la señal "clicked" del botón a la función print_hello
  // Pasa el botón como argumento para cambiar su etiqueta en la función
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), button);

  // Añade el botón a la caja para que aparezca en la ventana
  gtk_box_append (GTK_BOX (box), button);

  // Muestra la ventana en pantalla
  gtk_window_present (GTK_WINDOW (window));
}

int
main (int   argc,
      char *argv[])
{
  g_autoptr (AdwApplication) app = NULL;
  const gchar *appID = "github.CodigoCristo.GTK4-Demos"; // ID único de la aplicación

  app = adw_application_new (appID, G_APPLICATION_DEFAULT_FLAGS);

  g_signal_connect (app, "activate", G_CALLBACK (activate_cb), NULL);

  return g_application_run (G_APPLICATION (app), argc, argv);
}
