# Gtk4_demos

Para compilar apps en GTK4 usa el comando:

```gcc `pkg-config --cflags --libs gtk4` main.c```

Para compilar apps en GTK4 y LibAdwaita usa el comando:

```gcc `pkg-config --cflags --libs gtk4 libadwaita-1` main.c```

Para compilar con Meson usa:

```
meson setup build
meson compile -C build
./build/my_gtk_app
```
