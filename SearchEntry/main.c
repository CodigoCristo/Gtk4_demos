
#include <gtk/gtk.h>

// Función que se ejecuta cuando el texto en GtkSearchEntry cambia
static void
on_search_changed (GtkSearchEntry *entry, gpointer user_data)
{
    GtkListBox *list_box = GTK_LIST_BOX(user_data);
    gtk_list_box_invalidate_filter(list_box);
}

// Función de filtro para la lista
static gboolean
filter_list_func (GtkListBoxRow *row, gpointer user_data)
{
    GtkSearchEntry *entry = GTK_SEARCH_ENTRY(user_data);
    const gchar *search_text = gtk_editable_get_text(GTK_EDITABLE(entry));

    // Obtener el GtkCheckButton y el GtkLabel de la fila
    GtkWidget *box = gtk_list_box_row_get_child(row);
    GtkWidget *label = g_object_get_data(G_OBJECT(box), "label");

    const gchar *row_text = gtk_label_get_text(GTK_LABEL(label));

    // Si el texto de búsqueda está vacío, mostramos todos los elementos
    if (g_strcmp0(search_text, "") == 0) {
        return TRUE;
    }

    // Mostrar solo los elementos que contienen el texto de búsqueda (ignorando mayúsculas)
    return g_strrstr(g_ascii_strdown(row_text, -1), g_ascii_strdown(search_text, -1)) != NULL;
}

// Función que se ejecuta cuando el GtkCheckButton es presionado
static void
on_check_button_toggled (GtkCheckButton *button, gpointer user_data)
{
    const gchar *label_text = (const gchar *)user_data;

    if (gtk_check_button_get_active(button)) {
        g_print("'%s' seleccionado.\n", label_text);
    } else {
        g_print("'%s' desmarcado.\n", label_text);
    }
}

// Función que se ejecuta cuando la aplicación se activa
static void
activate (GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *search_entry;
    GtkWidget *list_box;
    GtkWidget *row;
    GtkWidget *row_box;
    GtkWidget *check_button;
    GtkWidget *label;

    // Crear la ventana principal
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Lista con Selección y Búsqueda");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 400);

    // Crear un contenedor vertical
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(window), box);

    // Crear GtkSearchEntry
    search_entry = gtk_search_entry_new();
    gtk_box_append(GTK_BOX(box), search_entry);

    // Crear GtkListBox
    list_box = gtk_list_box_new();
    gtk_box_append(GTK_BOX(box), list_box);

    // Añadir elementos a la lista con un GtkCheckButton en cada fila
    const gchar *items[] = {"Manzana", "Banana", "Cereza", "Durazno", "Frutilla", "Mango", "Melón", "Naranja"};
    for (int i = 0; i < G_N_ELEMENTS(items); i++) {
        row = gtk_list_box_row_new();
        row_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
        gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row), row_box);

        // Crear un GtkCheckButton
        check_button = gtk_check_button_new();
        gtk_box_append(GTK_BOX(row_box), check_button);

        // Crear un GtkLabel
        label = gtk_label_new(items[i]);
        gtk_box_append(GTK_BOX(row_box), label);

        // Guardar el label en la fila para usarlo en la función de filtro
        g_object_set_data(G_OBJECT(row_box), "label", label);

        // Conectar la señal "toggled" del GtkCheckButton a la función on_check_button_toggled
        g_signal_connect(check_button, "toggled", G_CALLBACK(on_check_button_toggled), (gpointer)items[i]);

        // Añadir la fila a la lista
        gtk_list_box_append(GTK_LIST_BOX(list_box), row);
    }

    // Configurar la función de filtrado de la lista
    gtk_list_box_set_filter_func(GTK_LIST_BOX(list_box), filter_list_func, search_entry, NULL);

    // Conectar la señal "search-changed" a la función on_search_changed
    g_signal_connect(search_entry, "search-changed", G_CALLBACK(on_search_changed), list_box);

    // Mostrar la ventana
    gtk_window_present(GTK_WINDOW(window));
}

// Función principal
int
main (int argc, char **argv)
{
    GtkApplication *app;
    int status;

    // Crear la aplicación
    app = gtk_application_new("com.ejemplo.searchlist", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    // Ejecutar la aplicación
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

