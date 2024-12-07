void autocompletado(Adw.EntryRow ent) 
{
    var pop = new Gtk.Popover();
    pop.set_has_arrow(false);
    pop.set_autohide(false);
    pop.set_halign(Gtk.Align.START);
    pop.add_css_class("menu");
    pop.set_parent(ent);
    ent.changed.connect(() => 
    {
        autocomp_aux(ent, pop);
    });
}



void autocomp_aux(Adw.EntryRow ent, Gtk.Popover pop) {
    var text = ent.get_text();

    // Validar si el texto es nulo o está vacío
    if (text == null || text.length == 0) {
        pop.popdown();
        return;
    }

    var text_lower = text.down();
    string[] suggestions = {"GNOME",
                            "gnominious",
                            "Gnomonic projection",
                            "total",
                            "totally",
                            "toto"};

    var listbox = new Gtk.ListBox();
    int count = 0;

    foreach (var suggestion in suggestions) {
        if (suggestion.down().has_prefix(text_lower)) {
            count++;
            var row = new Gtk.ListBoxRow();
            var label = new Gtk.Label(suggestion);
            label.halign = Gtk.Align.START;
            row.set_child(label);
            listbox.append(row);
        }
        if (count > 9) break;
    }

    if (count > 0) {
        pop.set_child(listbox);
        pop.popup();
    } else {
        pop.popdown();
    }

    listbox.row_activated.connect(() => {
        var row = listbox.get_selected_row();
        if (row != null) {
            var label = row.get_child() as Gtk.Label;
            if (label != null) {
                ent.set_text(label.get_text());
                ent.set_position(-1);
                ent.grab_focus_without_selecting();
            }
        }
        pop.popdown();
    });
}



void activate(Gtk.Application app) 
{
    var window = new Gtk.ApplicationWindow(app);
    window.set_default_size(600, 400);
    var headerbar = new Gtk.HeaderBar();
    window.set_titlebar(headerbar);
    window.set_title("Ejemplo sugerencia Adw.EntryRow");

    var vbox = new Gtk.Box(Gtk.Orientation.VERTICAL, 10);
    var pg = new Adw.PreferencesGroup();
    vbox.set_valign(Gtk.Align.CENTER);
    vbox.set_halign(Gtk.Align.CENTER);

    var ent = new Adw.EntryRow();
    ent.set_title("Ejemplo");
    autocompletado(ent);
    pg.add(ent);
    vbox.append(pg);

    window.set_child(vbox);
    window.present();
}

int main(string[] args) 
{
    var app = new Adw.Application("org.sugerencia.entryrow", ApplicationFlags.DEFAULT_FLAGS); // Cambiado a DEFAULT_FLAGS
    app.activate.connect(() => { activate(app); });
    return app.run(args);
}
