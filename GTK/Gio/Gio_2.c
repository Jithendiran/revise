/*
A bus is a central communication channel that allows different programs (processes) to talk to each other in a structured way.
a software bus allows apps to broadcast, listen, send, and receive messages.
In Linux,the most common message bus is: D-Bus

Types:
* Session Bus:
    Who use: Apps in a user session
    Example: Music player, browser, GUI apps
    It's private to your user — other users and system services can't access it.
* System Bus:
    Who use: System-wide services
    Example: NetworkManager, Bluetooth daemon
    the system bus is "public" in the sense that: It's shared by all users.
    "public" doesn't mean open to all without control — it means:
    - Any process can try to connect to the system bus.
    - But only authorized processes can use certain services or methods on it.

Roles:
* Client:
    Asks for something (calls a method)
* Service:
    Provides functionality (offers methods/signals)

named pipes, signals, shared memory, message queues, sockets, etc., are all valid IPC mechanisms. So why does D-Bus exist on top of all that?

D-Bus (Desktop Bus) is a higher-level IPC system designed to simplify communication between processes, especially in desktop environments (but also useful in system services).
Internally, D-Bus itself may use:Unix domain sockets, Message passing, shared memory optimizations

D-Bus is a message bus for applications to talk to each other, and GIO provides the GDBus API to work with it easily.
*/

/*
org.freedesktop.DBus is a special, well-known service name on the D-Bus system. think of it as the “controller” or “manager” of the bus

This is a daemon, where clients can 
* Register a name on the bus
* Query which services are connected
* Subscribe to notifications (like when a service connects/disconnects)
* Request name ownership (e.g., become the handler for org.myapp.Service)
* Monitor the bus
* Send messages to all listeners (broadcast)

Path: /org/freedesktop/DBus
We can call the methods like
* ListNames() : List all currently connected names (services) on the bus
* NameHasOwner("some.name") : Check if a service name is currently taken
* RequestName("your.service.name", flags) : Ask the bus to give your app a name
* ReleaseName("your.service.name") : Give up a name

Cmdline: 
dbus-send \
  --session \                       # session or system
  --dest=org.freedesktop.DBus \     # bus name
  --type=method_call \              # type:- method_call (default), signal (send a broadcast signal), method_return (rare for users), error (used internally) 
  --print-reply \                   # Tells dbus-send to wait and print the response, -reply-timeout=MS
  /org/freedesktop/DBus \           # The object path to send the method call to
  org.freedesktop.DBus.ListNames    # The interface.method being called


*/
#include <gio/gio.h>

int main() {
    GError *error = NULL;

    // 1. Connect to the session bus
    GDBusConnection *connection = g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, &error);
    if (!connection) {
        g_printerr("Failed to connect to session bus: %s\n", error->message);
        g_error_free(error);
        return 1;
    }

    /*
    * GVariant is a type-safe container for values — used to pass data over D-Bus or store structured data.
    * A smart box that can hold a single value (like an int, string) or a structured group (like arrays, tuples).
    * It knows its type (e.g., string "s", int "i", array "as", etc.).
    * It is immutable — once created, it doesn’t change.
    | D-Bus Type | C Type             | Meaning                   |
    | ---------- | ------------------ | ------------------------- |
    | "s"        | const gchar *      | String                    |
    | "i"        | gint32             | 32-bit integer            |
    | "as"       | Array of strings   | Like ["vlc", "firefox"]   |
    | "(is)"     | Tuple of int + str | Like (42, "hello")        |
    | "(as)"     | Tuple contains     | Like ("a", "vlc")         |
    |            |    array of string |                           | 

    */

    // 2. Call ListNames method on the D-Bus daemon
    GVariant *result = g_dbus_connection_call_sync(
        connection,
        "org.freedesktop.DBus",              // Destination (the D-Bus daemon)
        "/org/freedesktop/DBus",             // Object path
        "org.freedesktop.DBus",              // Interface
        "ListNames",                         // Method
        NULL,                                // No input parameters
        G_VARIANT_TYPE("(as)"),              // Expect one output: array of strings
        G_DBUS_CALL_FLAGS_NONE,
        -1,                                  // Default timeout
        NULL,                                // No cancellable
        &error
    );

    if (!result) {
        g_printerr("Failed to call ListNames: %s\n", error->message);
        g_error_free(error);
        g_object_unref(connection);
        return 1;
    }
    /*
    GVariantIter is a helper to loop over items in a GVariant, especially when it's an array or dictionary.
    A for-loop tool for GVariant.
    Lets you iterate over each element in an array or each key-value pair in a dictionary.
    */
    // 3. Extract and print the array of names
    GVariantIter *iter;
    gchar *name;
    g_variant_get(result, "(as)", &iter);

    g_print("Names on the session bus:\n");
    while (g_variant_iter_next(iter, "s", &name)) {
        g_print("  %s\n", name);
        g_free(name);
    }

    // Cleanup
    g_variant_iter_free(iter);
    g_variant_unref(result);
    g_object_unref(connection);

    return 0;
}

// gcc Gio_2.c -o /tmp/pgm `pkg-config --cflags --libs gio-2.0`