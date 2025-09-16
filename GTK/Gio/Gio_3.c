/*
GAction represents a named operation your application can perform — like Play, Pause, Quit, Save, etc.

GSimpleAction is concrete implementation of GAction.
If GAction is the interface (abstract), then GSimpleAction is a ready-to-use class
We use it to define, enable/disable, and handle activation of actions.

*/

#include <gio/gio.h>
#include <stdio.h>

// callback for custom action
static void on_quit_action(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_print("Quit action triggered!\n");
    GMainLoop *loop = user_data;
    g_main_loop_quit(loop);
}

static gboolean on_timeout_trigger_action(gpointer user_data) {
    GAction *action = G_ACTION(user_data);
    g_print("Triggering action programmatically...\n");
    g_action_activate(action, NULL); // activate the custom action
    return G_SOURCE_REMOVE; // Don't call again
}

int main() {
    // what is GApplication?
    /*
    It simplifies common tasks like:
    Starting up and shutting down cleanly
    Handling actions (like menus, shortcuts)
    Receiving command-line arguments
    Using D-Bus to talk to other apps or register your app globally
    */
    // 1. Create the application
    GApplication *app = g_application_new("com.example.MyApp", G_APPLICATION_DEFAULT_FLAGS);

    // 2. Create a simple action
    // quit is userdefine
    GSimpleAction *quit_action = g_simple_action_new("quit", NULL);

    // 3. Connect to the "activate" signal
    g_signal_connect(quit_action, "activate", G_CALLBACK(on_quit_action), NULL);

    // 4. Add the action to the app
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(quit_action));

    // 5. Simulate running
    g_print("App started. Triggering 'quit' after 2 seconds...\n");

    // Trigger the action manually after 2 seconds
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    g_timeout_add_seconds(2, on_timeout_trigger_action, quit_action);


    g_main_loop_run(loop);

    // Cleanup
    g_main_loop_unref(loop);
    g_object_unref(quit_action);
    g_object_unref(app);

    return 0;
}


// gcc Gio_3.c -o /tmp/pgm `pkg-config --cflags --libs gio-2.0`