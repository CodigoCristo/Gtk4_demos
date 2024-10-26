#include <gtk/gtk.h>




#define MAX_TIMEZONES 1000 // Tamaño máximo del array
#define TIMEZONE_NAME_LENGTH 50 // Longitud máxima de cada nombre de zona horaria



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

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *label;
    GtkWidget *dropdown;
    GtkStringList *string_list;

    // Crear la ventana principal
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Timezones DropDown");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);

    // Crea una caja para organizar los widgets verticalmente
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_halign (vbox, GTK_ALIGN_CENTER);
    gtk_widget_set_valign (vbox, GTK_ALIGN_CENTER);
    gtk_window_set_child(GTK_WINDOW(window), vbox);

    // Crear un Label para mostrar la selección
    label = gtk_label_new("Selecciona una zona horaria:");
    gtk_box_append(GTK_BOX(vbox), label);
    gtk_widget_set_size_request(label, 20, 60);

    // Crear un StringList para almacenar las zonas horarias
    string_list = gtk_string_list_new(NULL);

    // Ejecutar el comando y abrir un pipe para leer su salida
    FILE *fp = popen("timedatectl --no-pager list-timezones", "r");
    if (fp == NULL) {
        perror("popen failed");
        return;
    }

    // Leer cada línea de la salida y agregarla al StringList
    char buffer[TIMEZONE_NAME_LENGTH];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // Eliminar el salto de línea al final de la cadena
        buffer[strcspn(buffer, "\n")] = '\0';
        gtk_string_list_append(string_list, buffer);
    }

    // Cerrar el pipe
    pclose(fp);



    // Crear una expresión para asignar la propiedad de cadena al DropDown
    GtkExpression *exp = gtk_property_expression_new(
        GTK_TYPE_STRING_OBJECT,
        NULL,
        "string"
    );

    // Crea el DropDown con el modelo de lista y la expresión de propiedad
    dropdown = gtk_drop_down_new(G_LIST_MODEL(string_list), exp);
    gtk_drop_down_set_enable_search(GTK_DROP_DOWN(dropdown), TRUE);
    gtk_drop_down_set_search_match_mode(GTK_DROP_DOWN(dropdown), GTK_STRING_FILTER_MATCH_MODE_SUBSTRING);

    // Conecta la señal notify::selected-item para actualizar el label al cambiar la selección
    g_signal_connect(dropdown, "notify::selected-item", G_CALLBACK(on_selected_item_changed), label);
    
    // Añade el DropDown al contenedor vertical
    gtk_box_append(GTK_BOX(vbox), dropdown);

    // Mostrar la ventana en pantalla
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

