#include <glib-object.h>

// === Define MyObject and MyObjectClass ===
typedef struct _MyObject {
    GObject parent_instance;
    gint some_data;  // Our property-backed field
} MyObject;

typedef struct _MyObjectClass {
    GObjectClass parent_class;
} MyObjectClass;

// === GObject boilerplate ===
G_DEFINE_TYPE(MyObject, my_object, G_TYPE_OBJECT)

// === Property enum and storage ===
/*
GObject identifies properties using integer IDs behind the scenes.
*/
enum {
    PROP_0, //PROP_0 is always the first entry (value 0) and reserved by convention. It's not used for actual properties.
    PROP_SOME_DATA, // our cutom data property "some_data"
    // 1 .. n is custom properties
    N_PROPERTIES // no.of.properties
};

// what is GParamSpec?
/**
GParamSpec is meta-information about a property. Think of it as a description of a property, including:
    * The name ("some-data")
    * Human-readable name and description
    * Type (e.g. int, string)
    * Min/max/default values
    * Access flags (G_PARAM_READWRITE)
define this once, then GObject uses it to manage the property.
 */
static GParamSpec *obj_properties[N_PROPERTIES] = { NULL };

// === Set property ===
static void my_object_set_property(GObject *object,
                                   guint property_id,
                                   const GValue *value,
                                   GParamSpec *pspec) {
    MyObject *self = (MyObject *)object;

    switch (property_id) {
        case PROP_SOME_DATA:
            self->some_data = g_value_get_int(value);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}

// === Get property ===
static void my_object_get_property(GObject *object,
                                   guint property_id,
                                   GValue *value,
                                   GParamSpec *pspec) {
    MyObject *self = (MyObject *)object;

    switch (property_id) {
        case PROP_SOME_DATA:
            g_value_set_int(value, self->some_data);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}

// === Class init ===
static void my_object_class_init(MyObjectClass *klass) {
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass); // get GObjectClass from MyObjectClass
    // GObjectClass has getter, setter, constructor,..

    gobject_class->set_property = my_object_set_property;
    gobject_class->get_property = my_object_get_property;

    /*
    In GObject, property names are always written in "dash-case", not "snake_case".
    | In C Struct | In Property Registration | In GObject API |
    | ----------- | ------------------------ | -------------- |
    | some_data   | "some-data"              | "some-data"    |

    */
    obj_properties[PROP_SOME_DATA] = // bridiging
        g_param_spec_int("some_data",       // property name (dash-separated!) or some-data
                         "Some Data",       // nick name
                         "An integer field",// description
                         0,                 // min value
                         1000,              // max value
                         42,                // default value
                         G_PARAM_READWRITE  // read/write
        );

    g_object_class_install_property(gobject_class,
                                    PROP_SOME_DATA,
                                    obj_properties[PROP_SOME_DATA]);
}

// === Instance init ===
static void my_object_init(MyObject *self) {
    // No need to set default — it's handled by GParamSpec
}

// === Main ===
int main() {
    MyObject *obj = g_object_new(my_object_get_type(), NULL);

    // Read default value
    gint value = 0;
    g_object_get(obj, "some-data", &value, NULL);
    g_print("Initial some-data: %d\n", value);  // Should print 42

    // Change value
    g_object_set(obj, "some-data", 99, NULL);

    // Read again
    g_object_get(obj, "some-data", &value, NULL);
    g_print("Updated some-data: %d\n", value);  // Should print 99

    g_object_unref(obj);
    return 0;
}