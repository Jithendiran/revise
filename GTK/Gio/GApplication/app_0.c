#include <gio/gio.h>
#include <stdio.h>

// Single instance enforcement
/*
When 1st instance is running we open second one, second one will be terminated
*/

static GMainLoop *loop = NULL;

static void on_startup(GApplication *app, gpointer user_data) {
    g_print("Startup: Initializing application...\n");
}

void on_activate(GApplication *app, gpointer user_data) {
    static int count = 0;
    count++;
    g_print("Application activated (%d times)\n", count);

    // Start main loop only once
    if (!loop) {
        loop = g_main_loop_new(NULL, FALSE);
        g_main_loop_run(loop);
    }
}
static void on_shutdown(GApplication *app, gpointer user_data) {
    g_print("Shutdown: Cleaning up before exit...\n");
}

static gboolean on_sigint(gpointer user_data) {
    /*
    When you press Ctrl+C, the default behavior is that the process receives a SIGINT signal. 
    But GApplication by itself doesn't automatically handle SIGINT to trigger shutdown. 
    It only emits shutdown when you explicitly call g_application_quit() or the main loop quits.
    */
    GApplication *app = G_APPLICATION(user_data);
    g_print("Caught SIGINT, quitting app...\n");

    if (loop)
        g_main_loop_quit(loop);

    g_application_quit(app); // call teh shutdown and exit

    // Remove the signal handler after this, no need to keep it
    return G_SOURCE_REMOVE;
}

int main(int argc, char **argv) {
    GApplication *app;
    // com.example.SingleInstanceApp is the unique ID of the app, if you give NULL there we can open multiple sessions
    /* limitation of passing NULL
        - Does not attempt to register on the session bus
        - Cannot enforce single-instance behavior
        - Will not respond to D-Bus activation
        - Is essentially standalone — not discoverable by other processes
    */
    app = g_application_new("com.example.SingleInstanceApp", G_APPLICATION_DEFAULT_FLAGS);
    // To allow multi instance, with app id "com.example.App.XXXX" add some unique ID at end
    /*
    G_APPLICATION_DEFAULT_FLAGS – minimal behavior

    G_APPLICATION_HANDLES_OPEN – allows file open handling via D-Bus

    G_APPLICATION_HANDLES_COMMAND_LINE – full control over command-line handling

    G_APPLICATION_NON_UNIQUE – allows multiple instances (skips D-Bus name claim)
    */

    g_signal_connect(app, "startup", G_CALLBACK(on_startup), NULL);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    g_signal_connect(app, "shutdown", G_CALLBACK(on_shutdown), NULL);

    // Setup SIGINT handler for clean shutdown on Ctrl+C
    g_unix_signal_add(SIGINT, on_sigint, app);

    // Run the app — this starts the main loop and keeps it alive
    int status = g_application_run(app, argc, argv);

    if (loop)
        g_main_loop_unref(loop);

    g_object_unref(app);
    return status;
}


//gcc app_0.c -o /tmp/myapp `pkg-config --cflags --libs gio-2.0`

/*
Terminal 1
$ /tmp/myapp 
Application activated (1 times)
Application activated (2 times)
Application activated (3 times)
Application activated (4 times)

Terminal 2
$ /tmp/myapp 
$ /tmp/myapp 
$ /tmp/myapp
*/
