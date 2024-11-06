# Ejemplo sacado de:
# https://github.com/SpikedPaladin/ValaGtkTemplate/tree/main

project(
    'Example',
    'c',
    version: '1.0',
    meson_version: '>= 0.60',
)

add_global_arguments([
	'-Wno-unused',
	'-Wno-discarded-qualifiers'
], language: 'c')

gnome = import('gnome')

name = meson.project_name()
version = meson.project_version()
output = name.to_lower()

subdir('data')
subdir('src')
