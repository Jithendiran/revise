#include <gio/gio.h>

// Called when the async file read operation finishes.
/*
GObject *source: The object that started the async operation (here, a GFile).
GAsyncResult *res: Holds the result of the async operation.
gpointer user_data: User data passed when starting the async call; here, it’s the main loop.
*/
static void on_file_read(GObject *source, GAsyncResult *res, gpointer user_data) {
    GError *error = NULL;
    gchar *contents;
    gsize length;

    //g_file_load_contents_finish, This finishes the async call, extracting results:
    if (g_file_load_contents_finish(G_FILE(source), res, &contents, &length, NULL, &error)) {
        g_print("Async contents:\n%s\n", contents);
        g_free(contents);
    } else {
        g_printerr("Error: %s\n", error->message);
        g_clear_error(&error);
    }

    // Exit the loop if using one
    g_main_loop_quit((GMainLoop *)user_data); // This exits the main loop, allowing the program to continue and eventually exit.
}

int main() {
    GFile *file = g_file_new_for_path("/tmp/test.txt");
    // what is g_main_loop_new
    GMainLoop *loop = g_main_loop_new(NULL, FALSE); // Create a new GMainLoop — a loop that processes events, timers, I/O, etc.

    /*
    g_file_load_contents_async(GFile          *file,
                           GCancellable   *cancellable, // It is an optional object that allows you to cancel an asynchronous operation (like reading a file) before it completes.
                           GAsyncReadyCallback callback,
                           gpointer        user_data);

    */
   /*
   GMainLoop in GLib is conceptually similar to the event loop in Node.js.
   They both:
        Wait for events (timeouts, I/O, signals, etc.)
        Dispatch callbacks when events occur
        Keep running until told to stop
   */
    g_file_load_contents_async(file, NULL, on_file_read, loop); // Start an asynchronous read of the file contents
    // This tells GIO: "Load the file contents asynchronously, and when done, call on_file_read and pass loop as user_data."
    
    g_main_loop_run(loop); // runs the event loop — your program waits here while GTK/GIO processes events (including your async file read).

    g_main_loop_unref(loop);
    g_object_unref(file);
    return 0;
}

/*
Doubts
When content will start read?
The content starts being read asynchronously, right after this call: g_file_load_contents_async(file, NULL, on_file_read, loop);

If my program has infinity loop instead of GMainLoop, will that work?
Short answer: No, Think of g_file_load_contents_async as saying: "Hey GLib, when you're running your event loop, please read this file in the background, and let me know when you're done by calling on_file_read()."
g_file_load_contents_async() → Schedules the async task.
g_main_loop_run() → Actually processes it.
When the read finishes, on_file_read() is called within the main loop.
*/


// gcc Gio_1.c -o /tmp/pgm `pkg-config --cflags --libs gio-2.0`