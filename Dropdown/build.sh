#!/bin/bash

gcc `pkg-config --cflags --libs gtk4` main.c 
