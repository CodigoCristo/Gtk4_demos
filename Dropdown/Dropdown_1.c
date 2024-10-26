#include <gtk/gtk.h>

// Función de configuración de elementos de la lista
static void
setup_cb (GtkSignalListItemFactory *self, GtkListItem *listitem, gpointer user_data) {
    // Crea una etiqueta para cada elemento de la lista y la asigna como hijo del elemento de la lista
    GtkWidget *lb = gtk_label_new (NULL);
    gtk_list_item_set_child (listitem, lb);
}

// Función de enlace para asociar datos a cada elemento de la lista
static void
bind_cb (GtkSignalListItemFactory *self, GtkListItem *listitem, gpointer user_data) {
    // Obtiene el widget de etiqueta asociado al elemento de la lista
    GtkWidget *lb = gtk_list_item_get_child (listitem);
    // Obtiene el objeto de cadena que contiene el texto para el elemento actual
    GtkStringObject *strobj = gtk_list_item_get_item (listitem);
    // Establece el texto de la etiqueta con el contenido de la cadena del objeto
    gtk_label_set_text (GTK_LABEL (lb), gtk_string_object_get_string (strobj));
}

// Función que se llama al desasociar datos de un elemento de la lista
static void
unbind_cb (GtkSignalListItemFactory *self, GtkListItem *listitem, gpointer user_data) {
    // 
}

// Función que se llama al eliminar el elemento de la lista
static void
teardown_cb (GtkSignalListItemFactory *self, GtkListItem *listitem, gpointer user_data) {
    // 
}

// Función que maneja la actualización del elemento seleccionado en el DropDown
static void on_selected_item_changed(GtkDropDown *dropdown, GParamSpec *pspec, gpointer user_data) {
    // Obtiene el label para actualizar el texto
    GtkLabel *label = GTK_LABEL(user_data);
    // Obtiene el objeto de la cadena seleccionada en el DropDown
    GtkStringObject *selected_item = gtk_drop_down_get_selected_item(dropdown);

    // Si hay un elemento seleccionado, actualiza el texto del label
    if (selected_item) {
        const gchar *selected_text = gtk_string_object_get_string(selected_item);
        gtk_label_set_text(label, selected_text);
    }
}

// Función que se ejecuta al activar la aplicación (inicia la ventana principal y sus widgets)
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *label;
    GtkWidget *dropdown;

    // Crea la ventana principal de la aplicación
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "DropDown");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);

    // Crea una caja para organizar los widgets verticalmente
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_halign (vbox, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (vbox, GTK_ALIGN_CENTER);
    gtk_window_set_child(GTK_WINDOW(window), vbox);

    // Crea un Label para mostrar la selección actual
    label = gtk_label_new("Selecciona un elemento");
    gtk_box_append(GTK_BOX(vbox), label);
    gtk_widget_set_size_request(label, 20, 60);

    // Lista de cadenas que serán opciones en el DropDown
    char *array[] = {
        "Item 1", 
        "Item 2", 
        "Item 3",
        NULL
    };

    // Crea una GtkStringList para administrar las opciones del DropDown
    GtkStringList *sl = gtk_string_list_new((const char * const *) array);
    GtkNoSelection *ns = gtk_no_selection_new(G_LIST_MODEL(sl));

    // Crea una fábrica de elementos de lista y conecta las señales
    GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(setup_cb), NULL);
    g_signal_connect(factory, "bind", G_CALLBACK(bind_cb), NULL);
    g_signal_connect(factory, "unbind", G_CALLBACK(unbind_cb), NULL);
    g_signal_connect(factory, "teardown", G_CALLBACK(teardown_cb), NULL);

    // Crear una expresión para asignar la propiedad de cadena al DropDown
    GtkExpression *exp = gtk_property_expression_new(
        GTK_TYPE_STRING_OBJECT,
        NULL,
        "string"
    );

    // Crea el DropDown con el modelo de lista y la expresión de propiedad
    dropdown = gtk_drop_down_new(G_LIST_MODEL(sl), exp);
    gtk_drop_down_set_enable_search(GTK_DROP_DOWN(dropdown), TRUE);
    gtk_drop_down_set_search_match_mode(GTK_DROP_DOWN(dropdown), GTK_STRING_FILTER_MATCH_MODE_SUBSTRING);

    // Conecta la señal notify::selected-item para actualizar el label al cambiar la selección
    g_signal_connect(dropdown, "notify::selected-item", G_CALLBACK(on_selected_item_changed), label);
    
    // Añade el DropDown al contenedor vertical
    gtk_box_append(GTK_BOX(vbox), dropdown);

    // Muestra la ventana en pantalla
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    // Crea una nueva instancia de GtkApplication con un ID único
    app = gtk_application_new("org.gtk.dropdown", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    
    // Ejecuta la aplicación GTK y espera hasta que se cierre
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

