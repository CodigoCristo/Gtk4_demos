#include <adwaita.h>
#include <gtk/gtk.h>

static void activate(GtkApplication *app, gpointer user_data) {


    // Usa GtkBuilder para cargar la interfaz desde el recurso empaquetado
    GtkBuilder *builder = gtk_builder_new_from_resource("/org/gtk/Example/window.ui");
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    gtk_window_set_application(GTK_WINDOW(window), app);
    gtk_window_present(GTK_WINDOW(window));

    g_object_unref(builder); // Liberar el objeto builder después de su uso
}


int main(int argc, char *argv[]) {
    g_autoptr (AdwApplication) app = adw_application_new("org.gtk.Example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    return g_application_run(G_APPLICATION(app), argc, argv);
}

