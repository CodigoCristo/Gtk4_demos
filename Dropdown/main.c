
#include <gtk/gtk.h>

// Función de configuración de elementos de la lista
static void setup_cb(GtkSignalListItemFactory *self, GtkListItem *listitem, gpointer user_data) {
    GtkWidget *lb = gtk_label_new(NULL);
    gtk_list_item_set_child(listitem, lb);
}

// Función de enlace para asociar datos a cada elemento de la lista
static void bind_cb(GtkSignalListItemFactory *self, GtkListItem *listitem, gpointer user_data) {
    GtkWidget *lb = gtk_list_item_get_child(listitem);
    GtkStringObject *strobj = gtk_list_item_get_item(listitem);
    gtk_label_set_text(GTK_LABEL(lb), gtk_string_object_get_string(strobj));
}

// Función que maneja la actualización del elemento seleccionado en el DropDown
static void on_selected_item_changed(GtkDropDown *dropdown, GParamSpec *pspec, gpointer user_data) {
    GtkStringObject *selected_item = gtk_drop_down_get_selected_item(dropdown);

    // Si hay un elemento seleccionado, imprime el texto en la consola
    if (selected_item) {
        const gchar *selected_text = gtk_string_object_get_string(selected_item);
        printf("Seleccionaste: %s\n", selected_text);
    }
}

// Función que se ejecuta al activar la aplicación
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *label;
    GtkWidget *dropdown;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "DropDown");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_halign(vbox, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(vbox, GTK_ALIGN_CENTER);
    gtk_window_set_child(GTK_WINDOW(window), vbox);

    label = gtk_label_new("Selecciona un elemento");
    gtk_box_append(GTK_BOX(vbox), label);
    gtk_widget_set_size_request(label, 20, 60);

    char *array[] = {
        "Item 1",
        "Item 2",
        "Item 3",
        NULL
    };

    GtkStringList *sl = gtk_string_list_new((const char * const *) array);
    GtkNoSelection *ns = gtk_no_selection_new(G_LIST_MODEL(sl));

    GtkExpression *exp = gtk_property_expression_new(
        GTK_TYPE_STRING_OBJECT,
        NULL,
        "string"
    );

    dropdown = gtk_drop_down_new(G_LIST_MODEL(sl), exp);
    gtk_drop_down_set_search_match_mode(GTK_DROP_DOWN(dropdown), GTK_STRING_FILTER_MATCH_MODE_SUBSTRING);

    g_signal_connect(dropdown, "notify::selected-item", G_CALLBACK(on_selected_item_changed), label);
    
    gtk_box_append(GTK_BOX(vbox), dropdown);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.dropdown", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

