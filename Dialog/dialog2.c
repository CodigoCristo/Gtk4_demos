
#include <gtk/gtk.h>

static gboolean destroy_main_window(gpointer user_data) {
    GtkWindow *window = GTK_WINDOW(user_data);
    if (G_IS_OBJECT(window)) {
        gtk_window_destroy(window);
    }
    return G_SOURCE_REMOVE;
}

static void on_dialog_response(GtkAlertDialog *dialog, GAsyncResult *result, gpointer user_data) {
    int response = gtk_alert_dialog_choose_finish(dialog, result, NULL);
    GtkWindow *window = GTK_WINDOW(user_data);
    
    if (response == 0) {
        g_idle_add(destroy_main_window, window);
    }

    g_object_unref(dialog);
}

static void show_question_dialog(GtkWindow *parent) {
    GtkAlertDialog *alert_dialog = gtk_alert_dialog_new(
        "¿Está seguro de que desea continuar?",
        NULL
    );

    gtk_alert_dialog_set_detail(alert_dialog, "Esta acción no se puede deshacer.");
    gtk_alert_dialog_set_buttons(alert_dialog, (const char*[]){"OK", "Cancelar", NULL});
    gtk_alert_dialog_set_default_button(alert_dialog, 0);
    gtk_alert_dialog_set_cancel_button(alert_dialog, 1);
    
    gtk_alert_dialog_choose(alert_dialog, GTK_WINDOW(parent), NULL, (GAsyncReadyCallback)on_dialog_response, parent);
}

static void header_button_clicked(GtkWidget *button, gpointer user_data) {
    GtkWindow *window = GTK_WINDOW(user_data);
    show_question_dialog(window);
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *header_bar;
    GtkWidget *button;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Ejemplo de GtkAlertDialog");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);

    header_bar = gtk_header_bar_new();
    gtk_header_bar_set_title_widget(GTK_HEADER_BAR(header_bar), gtk_label_new("Aplicación con HeaderBar"));
    gtk_header_bar_set_decoration_layout(GTK_HEADER_BAR(header_bar), "");

    gtk_window_set_titlebar(GTK_WINDOW(window), header_bar);

    button = gtk_button_new_with_label("Salir");
    gtk_widget_add_css_class(button, "destructive-action");
    g_signal_connect(button, "clicked", G_CALLBACK(header_button_clicked), window);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), button);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.alertdialogexample", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

