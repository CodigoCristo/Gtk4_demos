#include <adwaita.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <ctype.h>
#include <string.h>

// Función para obtener el texto actual del GtkTextView
char* get_text_from_textview(GtkTextView *textview) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(textview);
    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);
    return gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
}

// Función para establecer texto en el GtkTextView
void set_text_in_textview(GtkTextView *textview, const char *text) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(textview);
    gtk_text_buffer_set_text(buffer, text, -1);
}

// Función para convertir el texto en mayúsculas
void on_mayus_button_clicked(GtkButton *button, gpointer user_data) {
    GtkTextView *textview = GTK_TEXT_VIEW(user_data);
    char *text = get_text_from_textview(textview);

    // Convertir a mayúsculas utilizando g_utf8_strup
    char *uppercase_text = g_utf8_strup(text, -1);

    // Establecer el texto modificado
    set_text_in_textview(textview, uppercase_text);
    
    g_free(text);
    g_free(uppercase_text);
}

// Función para convertir el texto en minúsculas
void on_minus_button_clicked(GtkButton *button, gpointer user_data) {
    GtkTextView *textview = GTK_TEXT_VIEW(user_data);
    char *text = get_text_from_textview(textview);

    // Convertir a minúsculas utilizando g_utf8_strdown
    char *lowercase_text = g_utf8_strdown(text, -1);

    // Establecer el texto modificado
    set_text_in_textview(textview, lowercase_text);
    
    g_free(text);
    g_free(lowercase_text);
}

// Función para convertir el texto en mayúsculas después de cada punto
void on_mayus_despues_punto_button_clicked(GtkButton *button, gpointer user_data) {
    GtkTextView *textview = GTK_TEXT_VIEW(user_data);
    char *text = get_text_from_textview(textview);

    // Convertir todo el texto a minúsculas
    char *lowercase_text = g_utf8_strdown(text, -1);

    // Crear un buffer para el texto final
    GString *result = g_string_new(NULL);
    gboolean capitalize_next = TRUE;

    // Recorrer el texto en minúsculas y capitalizar después de cada punto
    for (char *p = lowercase_text; *p != '\0'; p = g_utf8_next_char(p)) {
        gunichar ch = g_utf8_get_char(p);

        // Convertir a mayúscula si es el primer caracter después de un punto
        if (capitalize_next && g_unichar_isalpha(ch)) {
            ch = g_unichar_toupper(ch);
            capitalize_next = FALSE;
        }

        // Agregar el carácter al resultado
        g_string_append_unichar(result, ch);

        // Si el carácter es un punto, capitalizar el siguiente
        if (ch == '.') {
            capitalize_next = TRUE;
        }
    }

    // Establecer el texto modificado en el GtkTextView
    set_text_in_textview(textview, result->str);

    // Liberar la memoria utilizada
    g_free(text);
    g_free(lowercase_text);
    g_string_free(result, TRUE);
}


// Función para capitalizar la primera letra de cada palabra
void on_capi_text_clicked(GtkButton *button, gpointer user_data) {
    GtkTextView *textview = GTK_TEXT_VIEW(user_data);
    char *text = get_text_from_textview(textview);

    // Convertir el texto a minúsculas
    char *lowercase_text = g_utf8_strdown(text, -1);
    GString *result = g_string_new(NULL);

    gboolean capitalize_next = TRUE;

    // Recorrer el texto y capitalizar la primera letra de cada palabra
    for (char *p = lowercase_text; *p != '\0'; p = g_utf8_next_char(p)) {
        gunichar ch = g_utf8_get_char(p);

        // Capitalizar si es la primera letra de una palabra
        if (capitalize_next && g_unichar_isalpha(ch)) {
            ch = g_unichar_toupper(ch);
            capitalize_next = FALSE;
        }

        // Agregar el carácter al resultado
        g_string_append_unichar(result, ch);

        // Detectar espacios o caracteres de separación para capitalizar la próxima letra
        if (g_unichar_isspace(ch)) {
            capitalize_next = TRUE;
        }
    }

    // Establecer el texto modificado en el GtkTextView
    set_text_in_textview(textview, result->str);

    // Liberar la memoria utilizada
    g_free(text);
    g_free(lowercase_text);
    g_string_free(result, TRUE);
}


// Función de callback para el botón "Copiar Texto"
void on_copiar_texto_button_clicked(GtkButton *button, gpointer user_data) {
    GtkTextView *textview = GTK_TEXT_VIEW(user_data);

    // Obtener el texto del GtkTextView
    char *text = get_text_from_textview(textview);

    // Acceder al portapapeles predeterminado en GTK4
    GdkDisplay *display = gdk_display_get_default();
    GdkClipboard *clipboard = gdk_display_get_clipboard(display);

    // Copiar el texto al portapapeles
    gdk_clipboard_set_text(clipboard, text);

    // Liberar la memoria utilizada
    g_free(text);
}


static void
show_toast (AdwToastOverlay *overlay)
{
  AdwToast *toast = adw_toast_new ("Texto Copiado");
  adw_toast_set_timeout (toast, 3); // Duración de 3 segundos
  adw_toast_overlay_add_toast (overlay, toast);
}

static void activate(GtkApplication *app, gpointer user_data) {
    // Usa GtkBuilder para cargar la interfaz desde el recurso empaquetado
    GtkBuilder *builder = gtk_builder_new_from_resource("/org/gtk/Example/window.ui");
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    //AdwToastOverlay
    GtkWidget *toolbar_view = GTK_WIDGET(gtk_builder_get_object(builder, "toolbar_view"));
    GtkWidget *overlay = GTK_WIDGET(gtk_builder_get_object(builder, "overlay"));
    GtkWidget *adwstatus = GTK_WIDGET(gtk_builder_get_object(builder, "adwstatus"));

    
    GtkBuilder *boxview = gtk_builder_new_from_resource("/org/gtk/Example/texview.ui");
    GtkWidget *box = GTK_WIDGET(gtk_builder_get_object(boxview, "box"));

    //adw_toolbar_view_set_content (ADW_TOOLBAR_VIEW (toolbar_view), overlay);
    //adw_toast_overlay_set_child (ADW_TOAST_OVERLAY (overlay), adwstatus);
    adw_status_page_set_child(ADW_STATUS_PAGE(adwstatus), box);


    GtkTextView *textview = GTK_TEXT_VIEW(gtk_builder_get_object(boxview, "textview"));
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    GtkButton *button_mayus = GTK_BUTTON(gtk_builder_get_object(boxview, "mayus"));
    GtkButton *button_minus = GTK_BUTTON(gtk_builder_get_object(boxview, "minus"));
    GtkButton *button_mayus_despues_punto = GTK_BUTTON(gtk_builder_get_object(boxview, "mayus_despues_punto"));
    GtkButton *capi_text = GTK_BUTTON(gtk_builder_get_object(boxview, "capi_text"));
    GtkButton *copiar_texto = GTK_BUTTON(gtk_builder_get_object(boxview, "copiar_texto"));
    

    // Conectar las señales
    g_signal_connect(button_mayus, "clicked", G_CALLBACK(on_mayus_button_clicked), textview);
    g_signal_connect(button_minus, "clicked", G_CALLBACK(on_minus_button_clicked), textview);
    g_signal_connect(button_mayus_despues_punto, "clicked", G_CALLBACK(on_mayus_despues_punto_button_clicked), textview);
    g_signal_connect(capi_text, "clicked", G_CALLBACK(on_capi_text_clicked), textview);
    g_signal_connect(copiar_texto, "clicked", G_CALLBACK(on_copiar_texto_button_clicked), textview);
    g_signal_connect_swapped (copiar_texto, "clicked", G_CALLBACK (show_toast), overlay);
    
    gtk_window_set_application(GTK_WINDOW(window), app);
    gtk_window_present(GTK_WINDOW(window));

    g_object_unref(builder); // Liberar el objeto builder después de su uso
}

int main(int argc, char *argv[]) {
    g_autoptr (AdwApplication) app = adw_application_new("org.gtk.Example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    return g_application_run(G_APPLICATION(app), argc, argv);
}
