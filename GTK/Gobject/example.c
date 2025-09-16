#include <gtk/gtk.h>

// Callback function called when button is clicked
void on_button_clicked(GtkWidget *widget, gpointer user_data) {
    g_print("Clicked!\n");
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv); // Initialize GTK

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);  // Create window, Creates a new top-level window (the main app window).
    // means it's not a child or popup.
    gtk_window_set_title(GTK_WINDOW(window), "Minimal GTK");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 100);

    GtkWidget *button = gtk_button_new_with_label("Click me"); // Create button

    // Connect the "clicked" signal of the button to the callback
    // clicked is predefined signal
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);

    // Close the window when "destroy" is emitted
    // destroy is a predefined signal
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    // gtk_main_quit is a redefined unction to quit GTK

    gtk_container_add(GTK_CONTAINER(window), button); // Add button to window

    gtk_widget_show_all(window); // Show everything
    gtk_main(); // Start GTK event loop
    // the app now waits for events (clicks, etc).
    // Exits only when gtk_main_quit() is called (like when the user closes the window).

    return 0;
}

//gcc example.c -o /tmp/my_app `pkg-config --cflags --libs gtk+-3.0`