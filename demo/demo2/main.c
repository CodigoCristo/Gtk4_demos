
#include <adwaita.h>
#include <locale.h>
#include <libintl.h>

#define _(String) gettext(String)

static void
show_about_dialog (GtkWidget *button, GtkApplication *app)
{
  const char *developers[] = {
    "Codigo Cristo",
    NULL
  };

  const char *designers[] = {
    "Codigo Cristo",
    NULL
  };

  // Mostrar el diálogo "Acerca de"
  adw_show_about_dialog (GTK_WIDGET (gtk_application_get_active_window (app)),
                         "application-name", "MayusApp",
                         "application-icon", "preferences-desktop-keyboard",
                         "version", "1.0.0",
                         "copyright", "© 2024 MayusApp",
                         "issue-url", "https://github.com/your-repo/issues",
                         "license-type", GTK_LICENSE_MIT_X11,
                         "developers", developers,
                         "designers", designers,
                         "translator-credits", _("translator-credits"),
                         NULL);
}

static void
activate_cb (GtkApplication *app)
{
  GtkWidget *window = adw_application_window_new (app);
  GtkWidget *toolbar_view = adw_toolbar_view_new ();
  GtkWidget *header_bar = adw_header_bar_new ();
  GtkWidget *about_button = gtk_button_new_from_icon_name ("help-about-symbolic");

  // Configurar barra de herramientas y botón "Acerca de"
  gtk_widget_set_tooltip_text (about_button, "About");
  adw_header_bar_pack_end (ADW_HEADER_BAR (header_bar), about_button);

  GtkWidget *label = gtk_label_new ("Welcome to MayusApp!");

  adw_toolbar_view_add_top_bar (ADW_TOOLBAR_VIEW (toolbar_view), header_bar);
  adw_toolbar_view_set_content (ADW_TOOLBAR_VIEW (toolbar_view), label);

  gtk_window_set_title (GTK_WINDOW (window), "MayusApp");
  adw_application_window_set_content (ADW_APPLICATION_WINDOW (window), toolbar_view);

  // Conectar el botón al diálogo "Acerca de"
  g_signal_connect (about_button, "clicked", G_CALLBACK (show_about_dialog), app);

  gtk_window_present (GTK_WINDOW (window));
}

int
main (int   argc,
      char *argv[])
{
  g_autoptr (AdwApplication) app = NULL;

  app = adw_application_new ("org.example.MayusApp", 0);

  g_signal_connect (app, "activate", G_CALLBACK (activate_cb), NULL);

  return g_application_run (G_APPLICATION (app), argc, argv);
}

