#include <gtk/gtk.h>

static void
print_hello (GtkWidget *button, gpointer data)
{
    gtk_button_set_label(GTK_BUTTON(button), "Hola Mundo");
    g_print("Hello World\n");
}


static void
activate (GtkApplication *app, gpointer user_data)
{
    GtkBuilder *builder;
    GtkWidget *window;
    GtkWidget *button;



    // Crear GtkBuilder e instanciar la interfaz a partir del recurso
    builder = gtk_builder_new_from_resource("/com/example/myapp/builder.ui");

    // Obtener la ventana y botones del archivo UI
    window = GTK_WIDGET (gtk_builder_get_object(builder, "window"));
    gtk_window_set_application(GTK_WINDOW(window), app);

    button = GTK_WIDGET (gtk_builder_get_object(builder, "button1"));
    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

    button = GTK_WIDGET (gtk_builder_get_object(builder, "quit"));
    g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_window_destroy), window);

    gtk_window_present(GTK_WINDOW(window));

    g_object_unref(builder);
}

int
main (int argc, char *argv[])
{
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.example.myapp", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
