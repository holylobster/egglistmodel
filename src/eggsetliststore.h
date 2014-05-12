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

#ifndef __EGG_SET_LIST_STORE_H__
#define __EGG_SET_LIST_STORE_H__

#include <gio/gio.h>

#define EGG_TYPE_SET_LIST_STORE  (egg_set_list_store_get_type ())
#define EGG_SET_LIST_STORE(o)    (G_TYPE_CHECK_INSTANCE_CAST ((o), EGG_TYPE_SET_LIST_STORE, EggSetListStore))
#define EGG_IS_SET_LIST_STORE(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), EGG_TYPE_SET_LIST_STORE))

typedef struct _EggSetListStore EggSetListStore;

GType                   egg_set_list_store_get_type                     (void);

EggSetListStore *       egg_set_list_store_new                          (GType item_type);

gboolean                egg_set_list_store_add                          (EggSetListStore *set,
                                                                         gpointer         item);

gboolean                egg_set_list_store_remove                       (EggSetListStore *set,
                                                                         gpointer         item);

gboolean                egg_set_list_store_contains                     (EggSetListStore *set,
                                                                         gpointer         item);

void                    egg_set_list_store_set_sort_func                (EggSetListStore  *set,
                                                                         GCompareDataFunc  func,
                                                                         gpointer          user_data);

#endif
