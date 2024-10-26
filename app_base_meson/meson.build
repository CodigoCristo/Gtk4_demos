project('my_gtk_app', 'c', version: '0.1')

gtkdep = dependency('gtk4')
glibdep = dependency('glib-2.0')

gnome = import('gnome')

resources = gnome.compile_resources('myapp-resources',
  './data/gresource.xml',
  c_name: 'myapp'
)

executable('my_gtk_app',
  'src/main.c',
  resources,
  dependencies: [gtkdep, glibdep],
  install: true
)
