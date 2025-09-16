#include <gtk/gtk.h>
#include <string>
#include <sstream> // For formatting the float
#include <iomanip> // For std::fixed and std::setprecision
#include <functional>

static void
get_title(GtkTreeView *tree_view,
          GtkTreeIter *iter,
          gchar & value)
{
    GtkTreeModel *model = gtk_tree_view_get_model(tree_view);
    GtkTreePath *path = gtk_tree_model_get_path(model, iter);
    
    if (gtk_tree_view_row_expanded(tree_view, path)) {
        // only return it's title
        gchar *tmp;
        gtk_tree_model_get(model, iter, 0, &tmp, -1);
        printf(" Value : %s\n", tmp);
        // strcat(tmp, " + ");
        strcat(&value, tmp);
        return ;
    } else {
        // return concat of it's and child's title
        // gchar *value;
        gchar *tmp;
        gtk_tree_model_get(model, iter, 0, &tmp, -1);
        strcat(tmp, " + ");
        strcat(&value, tmp);
        if(gtk_tree_model_iter_has_child(model, iter)){
            GtkTreeIter child_iter;
            gtk_tree_model_iter_children(model, &child_iter, iter);
            get_title(tree_view, &child_iter, value);
            // strcat(tmp, );
            return ;
        }
    }
} 

static void
on_row_collapsed(GtkTreeView *tree_view,
                 GtkTreeIter *iter,
                 GtkTreePath *path,
                 gpointer user_data)
{
    // title must be p+c+....
    gchar *value;
    GtkTreeModel *model = gtk_tree_view_get_model(tree_view);
    gchar *path_str = gtk_tree_path_to_string(path);
    g_print("Row collapsed at path: %s\n", path_str);

    gtk_tree_model_get(model, iter, 0, &value, -1);
    g_print("Value at iter: %s\n", value);

    // GtkTreePath *path = gtk_tree_model_get_path(model, iter);
    // gboolean gtk_tree_view_row_expanded(GtkTreeView *tree_view, GtkTreePath *path);

    // GtkTreeIter child_iter;
    // if (gtk_tree_model_iter_has_child(model, &iter)) {
    //     gtk_tree_model_iter_children(model, &child_iter, &iter);
        
    // }

    g_free(value);
}

static void
on_row_expanded(GtkTreeView *tree_view,
                GtkTreeIter *iter,
                GtkTreePath *path,
                gpointer user_data)
{
    // Title Must be p -> c
    gchar *path_str = gtk_tree_path_to_string(path);
    g_print("Row expanded at path: %s\n", path_str);
    g_free(path_str);
}

// Function to create and display the window with the TreeView
static void create_window(GtkWidget *widget, gpointer data)
{
    // Create a new window
    GtkWidget *window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(window), "GtkTreeView Example (GTK 4 with GtkTreeStore)");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    GtkTreeStore *store; 
    // GtkTreeStore is data model which have data in tree structure, Each data in tree is consider as row each row can have parent
    GtkTreeIter iter1, iter2;
    // GtkTreeIter is a place holder to add data in a tree GtkTreeStore

    store = gtk_tree_store_new(1, G_TYPE_STRING); // One column (string)

    // ===== Fruits → Citrus → Orange =====

    // Level 0: Fruits
    gtk_tree_store_append(store, &iter1, NULL); // top-level
    // gtk_tree_store_append this is adding a new row, it's parent is null, so it is top entry, after adding row it point the iter1 to for new data
    // summary: new row is added without parent and it's data is pointed to iter1, as of now iter1 is empty
    gtk_tree_store_set(store, &iter1, 0, "Fruits", -1);
    // It will set iter1's data

    // Level 1: Citrus under Fruits
    gtk_tree_store_append(store, &iter2, &iter1); // iter1 is pointed to last inserted record, iter2 is the record we are going to insert
    gtk_tree_store_set(store, &iter2, 0, "Citrus", -1);

    // Level 2: Orange under Citrus
    gtk_tree_store_append(store, &iter1, &iter2); // reuse iter1
    gtk_tree_store_set(store, &iter1, 0, "Orange", -1);

    // ===== Vegetables → Leafy → Lettuce =====

    // Level 0: Vegetables
    gtk_tree_store_append(store, &iter1, NULL); // top-level again
    gtk_tree_store_set(store, &iter1, 0, "Vegetables", -1);

    // Level 1: Leafy under Vegetables
    gtk_tree_store_append(store, &iter2, &iter1);
    gtk_tree_store_set(store, &iter2, 0, "Leafy", -1);

    // Level 2: Lettuce under Leafy
    gtk_tree_store_append(store, &iter1, &iter2); // reuse iter1 again
    gtk_tree_store_set(store, &iter1, 0, "Lettuce", -1);

    // Create a TreeView and set the model to the TreeStore
    GtkWidget *treeview = gtk_tree_view_new();
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));

    // Create cell renderer and column
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes(
        "Items", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    ///
    GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
    GtkTreeStore *store1 = GTK_TREE_STORE(model);
    GtkTreeIter parent1;
    GtkTreeIter child_iter;

    std::function<void(GtkTreeIter&)> calc_summary;

    gchar value;
    calc_summary = [&treeview, &model, &calc_summary, &value](GtkTreeIter &iter) {
        GtkTreeIter child_iter;
        // take out value
        // gtk_tree_model_get(model, &iter, 0, &value, -1);
        get_title((GtkTreeView*)treeview, &iter, value);
        printf("Title : %s\n", (char *)&value);

        // child
        if (gtk_tree_model_iter_has_child(model, &iter)) {
            gtk_tree_model_iter_children(model, &child_iter, &iter);
            strcpy(&value, nullptr);
            calc_summary(child_iter);
        }

        // next sib
         // recerssive check
        gboolean valid = gtk_tree_model_iter_next(model, &iter);
        if (!valid) return;
        else calc_summary(iter);
    };
    gboolean valid = gtk_tree_model_get_iter_first(model, &parent1);
    if(valid) calc_summary(parent1);
    // g_free(value);


    // Add the TreeView to the window
    gtk_window_set_child(GTK_WINDOW(window), treeview);

      // Connect signals for expand/collapse
    g_signal_connect(treeview, "row-collapsed", G_CALLBACK(on_row_collapsed), NULL);
    g_signal_connect(treeview, "row-expanded", G_CALLBACK(on_row_expanded), NULL);


    // Show all widgets in the window
    gtk_widget_set_visible(window, TRUE);

}

int main(int argc, char *argv[])
{
    // Initialize GTK
    gtk_init();

    // Call the function to create the window
    create_window(NULL, NULL);

    // Run the GTK main loop
    g_main_loop_run(g_main_loop_new(NULL, FALSE));

    return 0;
}

// clear && g++ -g -O0 tree.cpp -o /tmp/tree.out `pkg-config --cflags --libs gtk4`