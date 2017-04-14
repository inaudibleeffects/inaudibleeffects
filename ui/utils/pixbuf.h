#include <gdk/gdk.h>

static GdkPixbuf*
inaudible_pixbuf_new_from_data(const char* start,
                               const char* end)
{
    GdkPixbuf* pixbuf;
    GdkPixbufLoader* loader = gdk_pixbuf_loader_new_with_type("png", NULL);
    gsize size = (gsize)(end - start);

    gdk_pixbuf_loader_write(loader, start, size, NULL);
    pixbuf = gdk_pixbuf_loader_get_pixbuf(loader);
    gdk_pixbuf_loader_close(loader, NULL);

    return pixbuf;
}

/*
    Change contrast.
*/
static void
inaudible_pixbuf_set_contrast(GdkPixbuf* pixbuf,
                              float factor)
{
    int width   = gdk_pixbuf_get_width(pixbuf);
    int height  = gdk_pixbuf_get_height(pixbuf);
    int alpha   = gdk_pixbuf_get_has_alpha(pixbuf) == TRUE ? 4 : 3;
    int length  = width * height * alpha - alpha - 1;
    int step    = alpha - 2;

    guchar* pixels = gdk_pixbuf_get_pixels(pixbuf);

    for (int i = 0; i < length; i += step)
    {
        pixels[i] *= factor;
        pixels[++i] *= factor;
        pixels[++i] *= factor;
    }
}

/*
    Change the hue of a pixel.
    See : http://stackoverflow.com/questions/8507885/shift-hue-of-an-rgb-color
*/
static void
inaudible_pixbuf_set_hue(GdkPixbuf* pixbuf,
                         guchar hue)
{
    float H     = (hue + 128) / 255 * 360;
    float U     = cos(hue * M_PI / 180);
    float W     = sin(hue * M_PI / 180);

    int width   = gdk_pixbuf_get_width(pixbuf);
    int height  = gdk_pixbuf_get_height(pixbuf);

    // Bytes per pixel.
    int alpha   = gdk_pixbuf_get_has_alpha(pixbuf) == TRUE ? 4 : 3;
    // Number of pixel to process.
    int length  = width * height * alpha - alpha - 1;
    // Bytes to ignore at each iteration.
    int step    = alpha - 2;

    guchar* pixels = gdk_pixbuf_get_pixels(pixbuf);

    for (int i = 0; i < length; i += step)
    {
        guchar r = pixels[i];
        guchar g = pixels[i + 1];
        guchar b = pixels[i + 2];

        pixels[i]   = (0.299 + 0.701 * U + 0.168 * W) * r
                    + (0.587 - 0.587 * U + 0.330 * W) * g
                    + (0.114 - 0.114 * U - 0.497 * W) * b;
        pixels[++i] = (0.299 - 0.299 * U - 0.328 * W) * r
                    + (0.587 + 0.413 * U + 0.035 * W) * g
                    + (0.114 - 0.114 * U + 0.292 * W) * b;
        pixels[++i] = (0.299 - 0.3   * U + 1.25  * W) * r
                    + (0.587 - 0.588 * U - 1.05  * W) * g
                    + (0.114 + 0.886 * U - 0.203 * W) * b;
    }
}
