
#include <adwaita.h>

// Función de la señal para cambiar a la siguiente página del carrusel
static void next_page_callback(GtkButton *button, gpointer user_data) {
    AdwCarousel *carousel = ADW_CAROUSEL(user_data); // Convertir el user_data en el tipo AdwCarousel

    int total_pages = adw_carousel_get_n_pages(carousel);          // Número total de páginas
    int current_page = adw_carousel_get_position(carousel);        // Página actual
    int next_page_index = (current_page + 1) % total_pages;        // Índice de la siguiente página (vuelve a la primera si está en la última)

    GtkWidget *next_page = adw_carousel_get_nth_page(carousel, next_page_index); // Obtener el widget de la siguiente página
    adw_carousel_scroll_to(carousel, next_page, TRUE);           // Desplazar a la siguiente página con animación
}

static void activate(GtkApplication *app, gpointer user_data) {
    // Cambia "ui_file.ui" a la ruta relativa o absoluta donde esté el archivo
    GtkBuilder *builder = gtk_builder_new_from_file("ui_file.ui");
 

    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "AppWindow"));
    gtk_window_set_application(GTK_WINDOW(window), app);

    // Conectar señales (por ejemplo, el botón "Next")
    GtkWidget *next_button = GTK_WIDGET(gtk_builder_get_object(builder, "next_button"));
    GtkWidget *carousel = GTK_WIDGET(gtk_builder_get_object(builder, "carousel"));
    g_signal_connect(next_button, "clicked", G_CALLBACK(next_page_callback), carousel);

    gtk_window_present(GTK_WINDOW(window));
    g_object_unref(builder);
}

int main(int argc, char *argv[]) {
    g_autoptr (AdwApplication) app = adw_application_new("com.example.app", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    return g_application_run(G_APPLICATION(app), argc, argv);
}

