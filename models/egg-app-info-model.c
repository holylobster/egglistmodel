
#include "egg-app-info-model.h"
#include "eggkeyedliststore.h"
#include <string.h>

void
egg_app_info_model_changed (GAppInfoMonitor *monitor,
                            gpointer         user_data)
{
  EggKeyedListStore *store = user_data;
  GHashTable *old_app_ids;
  GList *new_apps;
  GList *it;
  guint n_items;
  gint i;
  GHashTableIter iter;
  const gchar *id;

  old_app_ids = g_hash_table_new_full (g_str_hash, g_str_equal, NULL, g_object_unref);
  n_items = g_list_model_get_n_items (G_LIST_MODEL (store));
  for (i = 0; i < n_items; i++)
    {
      GAppInfo *info;

      info = g_list_model_get_item (G_LIST_MODEL (store), i);
      g_hash_table_insert (old_app_ids, (gpointer) g_app_info_get_id (info), info);
    }

  new_apps = g_app_info_get_all ();
  for (it = new_apps; it; it = it->next)
    {
      GAppInfo *info = it->data;

      if (!g_app_info_should_show (info))
        continue;

      id = g_app_info_get_id (info);

      /* only add the application if we didn't have it before */
      if (!g_hash_table_remove (old_app_ids, id))
        egg_keyed_list_store_insert (store, id, info);
    }

  /* remove app ids that aren't in new_apps */
  g_hash_table_iter_init (&iter, old_app_ids);
  while (g_hash_table_iter_next (&iter, (gpointer) &id, NULL))
    egg_keyed_list_store_remove (store, id);

  g_list_free_full (new_apps, g_object_unref);
  g_hash_table_destroy (old_app_ids);
}

static gint
egg_compare_app_infos (gconstpointer a,
                       gconstpointer b,
                       gpointer      user_data)
{
  return strcmp (g_app_info_get_display_name (G_APP_INFO (a)), g_app_info_get_display_name (G_APP_INFO (b)));
}

GListModel *
egg_app_info_model_new ()
{
  EggKeyedListStore *store;
  GAppInfoMonitor *monitor;

  store = egg_keyed_list_store_new (G_TYPE_APP_INFO);
  egg_keyed_list_store_set_sort_func (store, egg_compare_app_infos, NULL);

  monitor = g_app_info_monitor_get ();
  g_signal_connect (monitor, "changed", G_CALLBACK (egg_app_info_model_changed), store);
  egg_app_info_model_changed (monitor, store);

  g_object_set_data_full (G_OBJECT (store), "egg-app-info-model-monitor", monitor, g_object_unref);

  return G_LIST_MODEL (store);
}
