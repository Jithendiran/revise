#include <gtk/gtk.h>

static void
on_row_collapsed(GtkTreeView *tree_view,
                 GtkTreeIter *iter,
                 GtkTreePath *path,
                 gpointer user_data)
{
    gchar *path_str = gtk_tree_path_to_string(path);
    g_print("Row collapsed at path: %s\n", path_str);
    g_free(path_str);
}

static void
on_row_expanded(GtkTreeView *tree_view,
                GtkTreeIter *iter,
                GtkTreePath *path,
                gpointer user_data)
{
    gchar *path_str = gtk_tree_path_to_string(path);
    g_print("Row expanded at path: %s\n", path_str);
    g_free(path_str);
}

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *tree_view;
    GtkTreeStore *store;
    GtkTreeIter iter, child;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    gtk_init(&argc, &argv);

    // Create main window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GTK TreeView Example");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create tree store with one string column
    store = gtk_tree_store_new(1, G_TYPE_STRING);

    // Add parent and child rows
    gtk_tree_store_append(store, &iter, NULL); // Parent
    gtk_tree_store_set(store, &iter, 0, "Parent Row", -1);

    gtk_tree_store_append(store, &child, &iter); // Child
    gtk_tree_store_set(store, &child, 0, "Child Row", -1);

    // Create tree view
    tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    g_object_unref(store);

    // Create a column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Title", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    // Connect signals for expand/collapse
    g_signal_connect(tree_view, "row-collapsed", G_CALLBACK(on_row_collapsed), NULL);
    g_signal_connect(tree_view, "row-expanded", G_CALLBACK(on_row_expanded), NULL);

    // Add tree view to window
    gtk_container_add(GTK_CONTAINER(window), tree_view);
    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

// clear && g++ tree_collapse.cpp -o /tmp/build-tmp/tree.out `pkg-config --cflags --libs gtk4`