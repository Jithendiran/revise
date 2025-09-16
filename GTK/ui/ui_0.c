#include <gtk/gtk.h>

// 🧠 Callback for the button
void on_button_clicked(GtkButton *button, gpointer user_data) {
    g_print("Clicked!\n");
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Load the .ui file
    GtkBuilder *builder = gtk_builder_new_from_file("ui_0.ui");

    // Connect signals from .ui to your C functions
    gtk_builder_connect_signals(builder, NULL);


    // Get the window object by its ID
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    // gtk_builder_get_object is a predefined function which will get ui object based on the id
    
    gtk_widget_show_all(window);  // Show the window
    gtk_main();                   // Start event loop

    g_object_unref(builder);      // Cleanup

    return 0;
}

//gcc ui_0.c -o /tmp/my_app `pkg-config --cflags --libs gtk+-3.0` -rdynamic
/*
The -rdynamic flag is critical when using gtk_builder_connect_signals() with signal handler names defined in .ui files.
*/