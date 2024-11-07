#include <adwaita.h>

static void
show_toast (AdwToastOverlay *overlay)
{
  AdwToast *toast = adw_toast_new ("Este es un ejemplo de Toast en GTK4.");
  adw_toast_set_timeout (toast, 3); // Duración de 3 segundos
  adw_toast_overlay_add_toast (overlay, toast);
}

static void
activate_cb (GtkApplication *app)
{
  GtkWidget *window = adw_application_window_new (app);
  GtkWidget *toolbar_view = adw_toolbar_view_new ();
  GtkWidget *header_bar = adw_header_bar_new ();
  GtkWidget *button = gtk_button_new_with_label ("Mostrar Toast");
  GtkWidget *overlay = adw_toast_overlay_new ();

  // Configurar la ventana y su contenido
  adw_toolbar_view_add_top_bar (ADW_TOOLBAR_VIEW (toolbar_view), header_bar);
  
  // Configurar el botón dentro de una caja
  GtkWidget *box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 10);
  gtk_box_append (GTK_BOX (box), button);
  
  // Añadir el box al overlay como contenido principal
  adw_toast_overlay_set_child (ADW_TOAST_OVERLAY (overlay), box);
  adw_toolbar_view_set_content (ADW_TOOLBAR_VIEW (toolbar_view), overlay);

  // Conectar el botón para mostrar el toast y pasar el overlay correctamente
  g_signal_connect_swapped (button, "clicked", G_CALLBACK (show_toast), overlay);

  gtk_window_set_title (GTK_WINDOW (window), "Ejemplo de Toast");
  adw_application_window_set_content (ADW_APPLICATION_WINDOW (window), toolbar_view);
  gtk_window_present (GTK_WINDOW (window));
}

int
main (int argc, char *argv[])
{
  g_autoptr (AdwApplication) app = NULL;

  app = adw_application_new ("org.example.ToastExample", 0);

  g_signal_connect (app, "activate", G_CALLBACK (activate_cb), NULL);

  return g_application_run (G_APPLICATION (app), argc, argv);
}
