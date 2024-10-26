
#include <gio/gio.h>
#include <gtk/gtk.h>

static void
setup_cb (GtkSignalListItemFactory *self, GtkListItem *listitem, gpointer user_data) {
    GtkWidget *lb = gtk_label_new (NULL);
    gtk_list_item_set_child (listitem, lb);
}

static void
bind_cb (GtkSignalListItemFactory *self, GtkListItem *listitem, gpointer user_data) {
    GtkWidget *lb = gtk_list_item_get_child (listitem);
    GtkStringObject *strobj = gtk_list_item_get_item (listitem);
    gtk_label_set_text (GTK_LABEL (lb), gtk_string_object_get_string (strobj));
}

static void
unbind_cb (GtkSignalListItemFactory *self, GtkListItem *listitem, gpointer user_data) {
    /* No hay nada que hacer aquí. */
}

static void
teardown_cb (GtkSignalListItemFactory *self, GtkListItem *listitem, gpointer user_data) {
    /* No hay nada que hacer aquí. */
}

static void on_selected_item_changed(GtkDropDown *dropdown, GParamSpec *pspec, gpointer user_data) {
    GtkLabel *label = GTK_LABEL(user_data);
    GtkStringObject *selected_item = gtk_drop_down_get_selected_item(dropdown);

    if (selected_item) {
        const gchar *selected_text = gtk_string_object_get_string(selected_item);
        gtk_label_set_text(label, selected_text);
    }
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *label;
    GtkWidget *dropdown;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "DropDown Example");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_window_set_child(GTK_WINDOW(window), vbox);

    // Crear un Label para mostrar la selección
    label = gtk_label_new("Selecciona un elemento");
    gtk_box_append(GTK_BOX(vbox), label);

    char *array[] = {
        "one", "two", "three", "four", NULL
    };
    GtkStringList *sl = gtk_string_list_new((const char * const *) array);
    GtkNoSelection *ns = gtk_no_selection_new(G_LIST_MODEL(sl));

    GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(setup_cb), NULL);
    g_signal_connect(factory, "bind", G_CALLBACK(bind_cb), NULL);
    g_signal_connect(factory, "unbind", G_CALLBACK(unbind_cb), NULL);
    g_signal_connect(factory, "teardown", G_CALLBACK(teardown_cb), NULL);

    // Crear la expresión que se utilizará en el DropDown
    GtkExpression *exp = gtk_property_expression_new(
        GTK_TYPE_STRING_OBJECT,
        NULL,
        "string"
    );

    // Crear el DropDown
    dropdown = gtk_drop_down_new(G_LIST_MODEL(sl), exp);
    gtk_drop_down_set_enable_search(GTK_DROP_DOWN(dropdown), TRUE);
    gtk_drop_down_set_search_match_mode(GTK_DROP_DOWN(dropdown), GTK_STRING_FILTER_MATCH_MODE_SUBSTRING);

    // Conectar la señal notify::selected-item
    g_signal_connect(dropdown, "notify::selected-item", G_CALLBACK(on_selected_item_changed), label);
    
    // Agregar el DropDown al contenedor
    gtk_box_append(GTK_BOX(vbox), dropdown);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.dropdown.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

