#include "knob.h"

int main( int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *layout;
    GtkWidget *image;
	GtkWidget* knob;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 150);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    layout = gtk_layout_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER (window), layout);
    gtk_widget_show(layout);

    image = gtk_image_new_from_file("background.png");
    gtk_layout_put(GTK_LAYOUT(layout), image, 0, 0);

	knob = inaudible_knob_new();
	gtk_layout_put(GTK_LAYOUT(layout), knob, 174, 35);
    gtk_widget_set_size_request(knob, 80, 80);


    g_signal_connect_swapped(G_OBJECT(window), "destroy",
    G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
