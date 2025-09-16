// my_object_signal.c
#include <glib-object.h>

typedef struct _MyObject {
    GObject parent_instance;
} MyObject;

typedef struct _MyObjectClass {
    GObjectClass parent_class;
} MyObjectClass;

G_DEFINE_TYPE(MyObject, my_object, G_TYPE_OBJECT)

enum {
    SIGNAL_HELLO,
    N_SIGNALS
};

static guint signals[N_SIGNALS];

static void my_object_class_init(MyObjectClass *klass) {

signals[SIGNAL_HELLO] = g_signal_new(
    "hello",                    // 1. Signal name
    G_TYPE_FROM_CLASS(klass),   // 2. Signal's owner type
    G_SIGNAL_RUN_FIRST,         // 3. Signal flags, These flags control how the signal is emitted and handled.
    /*
    G_SIGNAL_RUN_FIRST: Default handlers run before user handlers.
    G_SIGNAL_RUN_LAST: Default handlers run after user handlers.
    G_SIGNAL_RUN_CLEANUP: Cleanup phase.
    G_SIGNAL_ACTION: Action signal (can be triggered via g_signal_emit_by_name()).
    G_SIGNAL_NO_RECURSE: Disallows recursive emissions.
    G_SIGNAL_DETAILED: Allows detailed signal names (e.g., "changed::property").
    */
    0,                          // 4. Class handler offset It tells GObject where to find the default virtual function to call when the signal is emitted., If you’re not using a default class handler
    NULL,                       // 5. Accumulator function
    NULL,                       // 6. Accumulator data
    NULL,                       // 7. C marshaller
    G_TYPE_NONE,                // 8. Return type
    0                           // 9. Number of parameters
);

}

static void my_object_init(MyObject *self) {}

//This is the function that will run when your "hello" signal is emitted. It just prints a message.
static void on_hello(MyObject *self, gpointer user_data) {
    g_print("Hello signal received!\n");
}

int main() {
    MyObject *obj = g_object_new(my_object_get_type(), NULL);

    g_signal_connect(obj, "hello", G_CALLBACK(on_hello), NULL);

    g_signal_emit_by_name(obj, "hello");

    g_object_unref(obj);
    return 0;
}

//gcc example.c -o /tmp/my_app `pkg-config --cflags --libs gtk+-3.0`