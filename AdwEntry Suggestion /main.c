#include <adwaita.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <gio/gio.h>

typedef struct _AutocompleteData {
    int ref_count;
    GtkPopover *popover;
    AdwEntryRow *entry_row;
} AutocompleteData;

typedef struct _SuggestionData {
    int ref_count;
    GtkListBox *list_box;
    AdwEntryRow *entry_row;
    GtkPopover *popover;
} SuggestionData;

// Incrementa la referencia de AutocompleteData
static AutocompleteData *autocomplete_data_ref(AutocompleteData *data) {
    g_atomic_int_inc(&data->ref_count);
    return data;
}

// Libera AutocompleteData
static void autocomplete_data_unref(void *user_data) {
    AutocompleteData *data = (AutocompleteData *)user_data;
    if (g_atomic_int_dec_and_test(&data->ref_count)) {
        g_clear_object(&data->popover);
        g_clear_object(&data->entry_row);
        g_slice_free(AutocompleteData, data);
    }
}

// Incrementa la referencia de SuggestionData
static SuggestionData *suggestion_data_ref(SuggestionData *data) {
    g_atomic_int_inc(&data->ref_count);
    return data;
}

// Libera SuggestionData
static void suggestion_data_unref(void *user_data) {
    SuggestionData *data = (SuggestionData *)user_data;
    if (g_atomic_int_dec_and_test(&data->ref_count)) {
        g_clear_object(&data->list_box);
        g_clear_object(&data->entry_row);
        g_clear_object(&data->popover);
        g_slice_free(SuggestionData, data);
    }
}

// Maneja la selección de una sugerencia
static void on_suggestion_selected(GtkListBox *list_box, GtkListBoxRow *row, gpointer user_data) {
    SuggestionData *data = (SuggestionData *)user_data;
    GtkLabel *label = GTK_LABEL(gtk_list_box_row_get_child(row));
    const gchar *text = gtk_label_get_text(label);

    gtk_editable_set_text(GTK_EDITABLE(data->entry_row), text);
    gtk_editable_set_position(GTK_EDITABLE(data->entry_row), -1);
    adw_entry_row_grab_focus_without_selecting(data->entry_row);

    gtk_popover_popdown(data->popover);
    suggestion_data_unref(data);
}

// Muestra las sugerencias en el campo de entrada
void show_suggestions(AdwEntryRow *entry_row, GtkPopover *popover) {
    const gchar *input_text = gtk_editable_get_text(GTK_EDITABLE(entry_row));
    if (!input_text || strlen(input_text) == 0) {
        gtk_popover_popdown(popover);
        return;
    }

    gchar *lower_input_text = g_utf8_strdown(input_text, -1);
    const gchar *suggestions[] = { "GNOME",
                                     "gnominious",
                                     "Gnomonic projection",
                                     "total",
                                     "totally",
                                     "toto",
                                     "tottery",
                                     "totterer",
                                     "Totten trust",
                                     "totipotent",
                                     "totipotency",
                                     "totemism",
                                     "totem pole",
                                     "Totara",
                                     "totalizer",
                                     "totalizator",
                                     "totalitarianism",
                                     "total parenteral nutrition",
                                     "total hysterectomy",
                                     "total eclipse",
                                     "Totipresence",
                                     "Totipalmi",
                                     "Tomboy",
                                     "zombie"};
    GtkListBox *list_box = GTK_LIST_BOX(gtk_list_box_new());
    int suggestion_count = 0;

    for (int i = 0; i < G_N_ELEMENTS(suggestions); i++) {
        gchar *lower_suggestion = g_utf8_strdown(suggestions[i], -1);
        if (g_str_has_prefix(lower_suggestion, lower_input_text)) {
            GtkListBoxRow *row = GTK_LIST_BOX_ROW(gtk_list_box_row_new());
            GtkLabel *label = GTK_LABEL(gtk_label_new(suggestions[i]));
            gtk_widget_set_halign(GTK_WIDGET(label), GTK_ALIGN_START);
            gtk_list_box_row_set_child(row, GTK_WIDGET(label));

            // Remover sombreado de la fila
            gtk_widget_remove_css_class(GTK_WIDGET(row), "row");
            gtk_widget_add_css_class(GTK_WIDGET(row), "custom-row");

            gtk_list_box_append(list_box, GTK_WIDGET(row));
            suggestion_count++;
        }
        g_free(lower_suggestion);
        if (suggestion_count >= 10) break;
    }

    g_free(lower_input_text);

    if (suggestion_count > 0) {
        SuggestionData *data = g_new0(SuggestionData, 1);
        data->list_box = list_box;
        data->entry_row = g_object_ref(entry_row);
        data->popover = g_object_ref(popover);

        g_signal_connect(list_box, "row-activated", G_CALLBACK(on_suggestion_selected), data);

        gtk_popover_set_child(popover, GTK_WIDGET(list_box));
        gtk_popover_popup(popover);
    } else {
        gtk_popover_popdown(popover);
    }
}

// Configura el autocompletado para un AdwEntryRow
void setup_autocomplete(AdwEntryRow *entry_row) {
    GtkPopover *popover = GTK_POPOVER(gtk_popover_new());
    gtk_popover_set_has_arrow(popover, FALSE);
    gtk_popover_set_autohide(popover, FALSE);
    gtk_widget_set_halign(GTK_WIDGET(popover), GTK_ALIGN_START);
    gtk_widget_set_parent(GTK_WIDGET(popover), GTK_WIDGET(entry_row));

    AutocompleteData *data = g_new0(AutocompleteData, 1);
    data->popover = popover;
    data->entry_row = g_object_ref(entry_row);

    g_signal_connect(entry_row, "changed", G_CALLBACK(show_suggestions), popover);
}

// Configura el estilo CSS para las filas sin sombreado
void configure_custom_css() {
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(provider,
        ".custom-row {"
        "   box-shadow: none;"
        "   border: none;"
        "   background-color: transparent;"
        "}"
        ".custom-row:focus {"
        "   outline: none;"
        "   background-color: rgba(128, 128, 128, 0.1);" /* Fondo gris claro */
        "   border-radius: 4px;" /* Bordes redondeados */
        "}"
        ".custom-row:selected {"
        "   background-color: rgba(128, 128, 128, 0.2);" /* Fondo gris más notorio */
        "}");
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
}

// Maneja la activación de la aplicación
void on_application_activate(GtkApplication *app) {
    GtkApplicationWindow *window = GTK_APPLICATION_WINDOW(gtk_application_window_new(app));
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);

    GtkHeaderBar *header_bar = GTK_HEADER_BAR(gtk_header_bar_new());
    gtk_window_set_titlebar(GTK_WINDOW(window), GTK_WIDGET(header_bar));
    gtk_window_set_title(GTK_WINDOW(window), "Ejemplo de Autocompletado");

    GtkBox *container = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 10));
    gtk_widget_set_valign(GTK_WIDGET(container), GTK_ALIGN_CENTER);
    gtk_widget_set_halign(GTK_WIDGET(container), GTK_ALIGN_CENTER);

    AdwPreferencesGroup *preferences_group = ADW_PREFERENCES_GROUP(adw_preferences_group_new());
    AdwEntryRow *entry_row = ADW_ENTRY_ROW(adw_entry_row_new());
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(entry_row), "Sugerencias");

    setup_autocomplete(entry_row);
    configure_custom_css();

    adw_preferences_group_add(preferences_group, GTK_WIDGET(entry_row));
    gtk_box_append(container, GTK_WIDGET(preferences_group));

    gtk_window_set_child(GTK_WINDOW(window), GTK_WIDGET(container));
    gtk_window_present(GTK_WINDOW(window));
}

// Función principal
int main(int argc, char **argv) {
    AdwApplication *app = adw_application_new("org.example.autocomplete", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_application_activate), NULL);
    return g_application_run(G_APPLICATION(app), argc, argv);
}
