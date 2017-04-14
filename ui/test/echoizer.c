#include "knob.h"

int main( int argc, char *argv[])
{
    GtkWidget* window;
    GtkWidget* layout;
    GtkWidget* image;
	InaudibleKnob* knobDelay;
    InaudibleKnob* knobFeedback;
    InaudibleKnob* knobBlend;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 150);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    layout = gtk_layout_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(window), layout);
    gtk_widget_show(layout);

    image = gtk_image_new_from_file("background.png");
    gtk_layout_put(GTK_LAYOUT(layout), image, 0, 0);

    knobDelay = (InaudibleKnob*)inaudible_knob_new();
	gtk_layout_put(GTK_LAYOUT(layout), (GtkWidget*)knobDelay, 55, 35);
    gtk_widget_set_size_request((GtkWidget*)knobDelay, 80, 80);
    inaudible_knob_set_hue(knobDelay, 20);

	knobFeedback = (InaudibleKnob*)inaudible_knob_new();
	gtk_layout_put(GTK_LAYOUT(layout), (GtkWidget*)knobFeedback, 174, 35);
    gtk_widget_set_size_request((GtkWidget*)knobFeedback, 80, 80);
    inaudible_knob_set_hue(knobFeedback, 20);

    knobBlend = (InaudibleKnob*)inaudible_knob_new();
	gtk_layout_put(GTK_LAYOUT(layout), (GtkWidget*)knobBlend, 293, 35);
    gtk_widget_set_size_request((GtkWidget*)knobBlend, 80, 80);
    inaudible_knob_set_hue(knobBlend, 20);


    g_signal_connect_swapped(G_OBJECT(window), "destroy",
    G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
