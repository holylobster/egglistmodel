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

#include "eggsetliststore.h"
#include "eggsortedhash.h"

typedef GObjectClass EggSetListStoreClass;

struct _EggSetListStore
{
  GObject parent;

  GType item_type;
  EggSortedHash *items;
};

static void egg_set_list_store_object_list_iface_init (GListModelInterface *iface);

G_DEFINE_TYPE_WITH_CODE (EggSetListStore, egg_set_list_store, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (G_TYPE_LIST_MODEL, egg_set_list_store_object_list_iface_init));

static void
egg_set_list_store_init (EggSetListStore *store)
{
  store->items = egg_sorted_hash_new (g_direct_hash, g_direct_equal, NULL, g_object_unref);
}

static void
egg_set_list_store_dispose (GObject *object)
{
  EggSetListStore *store = EGG_SET_LIST_STORE (object);

  g_clear_pointer (&store->items, egg_sorted_hash_free);

  G_OBJECT_CLASS (egg_set_list_store_parent_class)->dispose (object);
}

static void
egg_set_list_store_class_init (EggSetListStoreClass *class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (class);

  object_class->dispose = egg_set_list_store_dispose;
}

static GType
egg_set_list_store_get_item_type (GListModel *list)
{
  EggSetListStore *store = EGG_SET_LIST_STORE (list);

  return store->item_type;
}

static guint
egg_set_list_store_get_n_items (GListModel *list)
{
  EggSetListStore *store = EGG_SET_LIST_STORE (list);

  return egg_sorted_hash_get_length (store->items);
}

gpointer
egg_set_list_store_get_item (GListModel *list,
                             guint       position)
{
  EggSetListStore *store = EGG_SET_LIST_STORE (list);

  return egg_sorted_hash_get (store->items, position);
}

static void
egg_set_list_store_object_list_iface_init (GListModelInterface *iface)
{
  iface->get_item_type = egg_set_list_store_get_item_type;
  iface->get_n_items = egg_set_list_store_get_n_items;
  iface->get_item = egg_set_list_store_get_item;
}

EggSetListStore *
egg_set_list_store_new (GType item_type)
{
  EggSetListStore *store;

  store = g_object_new (EGG_TYPE_SET_LIST_STORE, NULL);
  store->item_type = item_type;

  return store;
}

/* Returns %TRUE if @key did not exist yet.
 */
gboolean
egg_set_list_store_add (EggSetListStore *store,
                        gpointer         item)
{
  guint position;

  g_return_val_if_fail (EGG_IS_SET_LIST_STORE (store), FALSE);
  g_return_val_if_fail (G_IS_OBJECT (item), FALSE);

  if (egg_sorted_hash_insert (store->items, item, g_object_ref (item), &position))
    {
      g_list_model_items_changed (G_LIST_MODEL (store), position, 0, 1);
      return TRUE;
    }

  return FALSE;
}

gboolean
egg_set_list_store_remove (EggSetListStore *store,
                           gpointer         item)
{
  guint position;

  g_return_val_if_fail (EGG_IS_SET_LIST_STORE (store), FALSE);
  g_return_val_if_fail (item != NULL, FALSE);

  if (egg_sorted_hash_remove (store->items, item, &position))
    {
      g_list_model_items_changed (G_LIST_MODEL (store), position, 1, 0);
      return TRUE;
    }

  return FALSE;
}

gboolean
egg_set_list_store_contains (EggSetListStore *store,
                             gpointer         item)
{
  g_return_val_if_fail (EGG_IS_SET_LIST_STORE (store), NULL);
  g_return_val_if_fail (item != NULL, NULL);

  return egg_sorted_hash_lookup (store->items, item) != NULL;
}

void
egg_set_list_store_set_sort_func (EggSetListStore  *store,
                                  GCompareDataFunc  func,
                                  gpointer          user_data)
{
  guint length;

  g_return_if_fail (EGG_IS_SET_LIST_STORE (store));

  egg_sorted_hash_set_sort_func (store->items, func, user_data);

  length = egg_sorted_hash_get_length (store->items);
  g_list_model_items_changed (G_LIST_MODEL (store), 0, length, length);
}
