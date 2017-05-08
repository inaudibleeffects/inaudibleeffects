# INAUDIBLE EFFECTS (GTK3 branch)
Collection of experimental LV2 audio plugins.  
This branch is dedicated to the GTK3 plugins port.

** Disclaimer : you should not considerate using GTK3 to write audio plugins as they will not run on GTK2 hosts like Ardour ! This branch was solely made to show how to write a GTK3 LV2 plugin in C. **

## Plugins

### Echoizer

Simple delay processor with delay up to 2 seconds, feedback and blending control.
![Echoizer](echoizer.png)

## Dependencies

* Gtk3
* Gdk
* Cairo
* LV2
* Jack

## Installation

* git clone -b gtk3 https://github.com/inaudibleeffects/inaudibleeffects
* cd inaudibleeffects
* make
* sudo make install

## Run

Only use GTK3 hosts like Jalv :

` jalv.gtk3 http://inaudibleeffects.github.io/echoizer `
