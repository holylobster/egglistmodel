
#include <gtk/gtk.h>
#include "egg-app-info-model.h"

static GtkWidget *
create_row_widget (gpointer item,
                   gpointer user_data)
{
  GAppInfo *info = item;
  GtkWidget *label;

  label = gtk_label_new (g_app_info_get_display_name (info));
  gtk_widget_set_halign (label, GTK_ALIGN_START);

  return label;
}

int main (int argc, char **argv)
{
  GtkWidget *window;
  GtkWidget *scrolled_window;
  GtkWidget *listbox;
  GListModel *model;

  gtk_init (&argc, &argv);

  model = egg_app_info_model_new ();

  listbox = gtk_list_box_new ();
  gtk_list_box_bind_model (GTK_LIST_BOX (listbox), model, create_row_widget, NULL);

  scrolled_window = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
  gtk_container_add (GTK_CONTAINER (scrolled_window), listbox);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size (GTK_WINDOW (window), 300, 500);
  gtk_container_add (GTK_CONTAINER (window), scrolled_window);
  gtk_widget_show_all (window);

  g_signal_connect (window, "destroy", gtk_main_quit, NULL);

  gtk_main ();

  g_object_unref (model);
  return 0;
}
