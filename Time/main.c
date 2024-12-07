#include <gtk/gtk.h>
#include <adwaita.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

GtkWidget *label;

// Función que se ejecuta cada segundo para actualizar la hora
static gboolean
update_time(gpointer data)
{
    // Definir la zona horaria en una variable
    const char *timezone = "America/Lima";

    // Crear el comando para obtener la hora con la zona horaria especificada
    char command[200];
    snprintf(command, sizeof(command), "TZ=\"%s\" date +\"%%I:%%M:%%S %%p\"", timezone);

    // Ejecutar el comando para obtener la hora
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        g_print("Error al obtener la hora\n");
        return TRUE;
    }

    // Leer la hora desde la salida del comando
    char time_str[100];
    fgets(time_str, sizeof(time_str), fp);
    fclose(fp);

    // Eliminar el salto de línea
    time_str[strcspn(time_str, "\n")] = '\0';

    // Actualizar el texto del GtkLabel
    gtk_label_set_text(GTK_LABEL(label), time_str);

    return TRUE; // Continúa llamando a esta función cada 1000 ms
}

// Función que se ejecuta cuando se activa la aplicación
static void
activate_cb(GtkApplication *app)
{
    GtkWidget *window;  // Declaración de la ventana principal
    GtkWidget *box;     // Declaración de una caja para contener los widgets

    // Crear la ventana principal
    window = gtk_application_window_new(app);

    // Configura el título de la ventana
    gtk_window_set_title(GTK_WINDOW(window), "Hora Actualizada");

    // Establece el tamaño predeterminado de la ventana
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

    // Crear una caja con orientación vertical para organizar los widgets
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    // Centra horizontal y verticalmente la caja dentro de la ventana
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);

    // Agregar la caja como el widget principal de la ventana
    gtk_window_set_child(GTK_WINDOW(window), box);

    // Crear un GtkLabel para mostrar la hora
    label = gtk_label_new("Cargando...");

    // Añadir el GtkLabel a la caja
    gtk_box_append(GTK_BOX(box), label);

    // Actualizar la hora cada segundo
    g_timeout_add(1000, update_time, NULL);

    // Mostrar la ventana en pantalla
    gtk_window_present(GTK_WINDOW(window));
}

int
main(int argc, char *argv[])
{
    g_autoptr(AdwApplication) app = NULL;
    const gchar *appID = "com.ejemplo.HoraActualizada"; // ID único de la aplicación

    app = adw_application_new(appID, G_APPLICATION_DEFAULT_FLAGS);

    // Conectar la señal de activación de la aplicación con la función de creación de la ventana
    g_signal_connect(app, "activate", G_CALLBACK(activate_cb), NULL);

    // Ejecutar la aplicación
    return g_application_run(G_APPLICATION(app), argc, argv);
}
