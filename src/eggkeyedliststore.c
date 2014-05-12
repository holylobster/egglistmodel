/*
 * Copyright © 2014 Lars Uebernickel
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: Lars Uebernickel <lars@uebernic.de>
 */

#include "eggkeyedliststore.h"
#include "eggsortedhash.h"

typedef GObjectClass EggKeyedListStoreClass;

struct _EggKeyedListStore
{
  GObject parent;

  GType item_type;
  EggSortedHash *items;
};

static void egg_keyed_list_store_object_list_iface_init (GListModelInterface *iface);

G_DEFINE_TYPE_WITH_CODE (EggKeyedListStore, egg_keyed_list_store, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (G_TYPE_LIST_MODEL, egg_keyed_list_store_object_list_iface_init));

static void
egg_keyed_list_store_init (EggKeyedListStore *store)
{
  store->items = egg_sorted_hash_new (g_str_hash, g_str_equal, g_free, g_object_unref);
}

static void
egg_keyed_list_store_dispose (GObject *object)
{
  EggKeyedListStore *store = EGG_KEYED_LIST_STORE (object);

  g_clear_pointer (&store->items, egg_sorted_hash_free);

  G_OBJECT_CLASS (egg_keyed_list_store_parent_class)->dispose (object);
}

static void
egg_keyed_list_store_class_init (EggKeyedListStoreClass *class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (class);

  object_class->dispose = egg_keyed_list_store_dispose;
}

static GType
egg_keyed_list_store_get_item_type (GListModel *list)
{
  EggKeyedListStore *store = EGG_KEYED_LIST_STORE (list);

  return store->item_type;
}

static guint
egg_keyed_list_store_get_n_items (GListModel *list)
{
  EggKeyedListStore *store = EGG_KEYED_LIST_STORE (list);

  return egg_sorted_hash_get_length (store->items);
}

gpointer
egg_keyed_list_store_get_item (GListModel *list,
                               guint       position)
{
  EggKeyedListStore *store = EGG_KEYED_LIST_STORE (list);

  return egg_sorted_hash_get (store->items, position);
}

static void
egg_keyed_list_store_object_list_iface_init (GListModelInterface *iface)
{
  iface->get_item_type = egg_keyed_list_store_get_item_type;
  iface->get_n_items = egg_keyed_list_store_get_n_items;
  iface->get_item = egg_keyed_list_store_get_item;
}

EggKeyedListStore *
egg_keyed_list_store_new (GType item_type)
{
  EggKeyedListStore *store;

  store = g_object_new (EGG_TYPE_KEYED_LIST_STORE, NULL);
  store->item_type = item_type;

  return store;
}

/* Returns %TRUE if @key did not exist yet.
 */
gboolean
egg_keyed_list_store_insert (EggKeyedListStore *store,
                             const gchar       *key,
                             gpointer           item)
{
  guint position;

  g_return_val_if_fail (EGG_IS_KEYED_LIST_STORE (store), FALSE);
  g_return_val_if_fail (key != NULL, FALSE);
  g_return_val_if_fail (G_IS_OBJECT (item), FALSE);

  if (egg_sorted_hash_insert (store->items, g_strdup (key), g_object_ref (item), &position))
    {
      g_list_model_items_changed (G_LIST_MODEL (store), position, 0, 1);
      return TRUE;
    }

  return FALSE;
}

gboolean
egg_keyed_list_store_remove (EggKeyedListStore *store,
                             const gchar       *key)
{
  guint position;

  g_return_val_if_fail (EGG_IS_KEYED_LIST_STORE (store), FALSE);
  g_return_val_if_fail (key != NULL, FALSE);

  if (egg_sorted_hash_remove (store->items, (gpointer) key, &position))
    {
      g_list_model_items_changed (G_LIST_MODEL (store), position, 1, 0);
      return TRUE;
    }

  return FALSE;
}

gpointer
egg_keyed_list_store_lookup (EggKeyedListStore *store,
                             const gchar       *key)
{
  g_return_val_if_fail (EGG_IS_KEYED_LIST_STORE (store), NULL);
  g_return_val_if_fail (key != NULL, NULL);

  return egg_sorted_hash_lookup (store->items, (gpointer) key);
}

void
egg_keyed_list_store_set_sort_func (EggKeyedListStore *store,
                                    GCompareDataFunc   func,
                                    gpointer           user_data)
{
  guint length;

  g_return_if_fail (EGG_IS_KEYED_LIST_STORE (store));

  egg_sorted_hash_set_sort_func (store->items, func, user_data);

  length = egg_sorted_hash_get_length (store->items);
  g_list_model_items_changed (G_LIST_MODEL (store), 0, length, length);
}
