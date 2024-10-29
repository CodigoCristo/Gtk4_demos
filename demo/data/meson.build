# Compilar los recursos
gresource = gnome.compile_resources(
    'myapp_resources',
    'gresource.xml',
    source_dir: meson.current_source_dir()
)

# Archivos a instalar
data_files = files('builder.ui', 'gresource.xml')
install_data(data_files, install_dir: get_option('datadir') / 'my_project')
