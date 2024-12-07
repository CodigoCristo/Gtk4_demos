#include <adwaita.h>

static void
activate_cb (GtkApplication *app)
{
  GtkBuilder *builder;
  GtkWidget *window;

  /* Cargar la interfaz desde el archivo UI */
  builder = gtk_builder_new_from_file ("ui.ui");
  window = GTK_WIDGET (gtk_builder_get_object (builder, "main_window"));

  /* Configurar la aplicación */
  gtk_window_set_application (GTK_WINDOW (window), GTK_APPLICATION (app));

  /* Mostrar la ventana */
  gtk_window_present (GTK_WINDOW (window));

  /* Liberar recursos */
  g_object_unref (builder);
}

int
main (int argc, char *argv[])
{
  g_autoptr (AdwApplication) app = NULL;

  app = adw_application_new ("org.example.HelloBuilder", 0);

  g_signal_connect (app, "activate", G_CALLBACK (activate_cb), NULL);

  return g_application_run (G_APPLICATION (app), argc, argv);
}

