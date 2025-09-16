#include <gio/gio.h>
#include <stdio.h>

int main()
{
    GError *error = NULL;
    // Instead of using standard C file paths and fopen(), GIO uses GFile.
    GFile *file = g_file_new_for_path("/tmp/test.txt");
    gboolean exists = g_file_query_exists(file, NULL);

    // The term g_print typically refers to a function in the GLib library, which is part of the GNOME project and widely used in C programming on Linux systems.
    if (exists)
    {
        g_print("File exists!\n");
        gchar *contents;
        gsize length;

        if (g_file_load_contents(file, NULL, &contents, &length, NULL, &error))
        {
            g_print("File contents:\n%s\n", contents);
            g_free(contents); // g_free() is GLib’s memory deallocation function, similar to standard C’s free().
            // It frees memory allocated by GLib functions like g_malloc(), or when GLib hands you ownership of allocated memory.
        }
        else
        {
            g_printerr("Error: %s\n", error->message);
            g_clear_error(&error);
        }
    }
    else
        g_print("File not found.\n");

    /*
    Why use g_print instead of printf?

        g_print handles UTF-8 strings properly, which is important for internationalization.

        It integrates better with GLib's logging and I/O system.

        It might have better portability across different platforms supported by GLib.
    */

    g_object_unref(file);
    return 0;
}

// gcc Gio_0.c -o /tmp/check_file `pkg-config --cflags --libs gio-2.0`