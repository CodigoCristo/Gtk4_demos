#!/bin/bash
meson setup build 
meson setup build --reconfigure
meson compile -C build
./build/src/example
