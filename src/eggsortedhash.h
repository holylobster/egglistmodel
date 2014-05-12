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

#include <glib.h>

typedef struct _EggSortedHash EggSortedHash;

EggSortedHash *         egg_sorted_hash_new                             (GHashFunc      hash_func,
                                                                         GEqualFunc     equal_func,
                                                                         GDestroyNotify key_free_func,
                                                                         GDestroyNotify value_free_func);

void                    egg_sorted_hash_free                            (EggSortedHash *hash);

guint                   egg_sorted_hash_get_length                      (EggSortedHash *hash);

gpointer                egg_sorted_hash_get                             (EggSortedHash *hash,
                                                                         guint          position);

gpointer                egg_sorted_hash_lookup                          (EggSortedHash    *hash,
                                                                         gpointer          key);

void                    egg_sorted_hash_set_sort_func                   (EggSortedHash    *hash,
                                                                         GCompareDataFunc  func,
                                                                         gpointer          data);

gboolean                egg_sorted_hash_insert                          (EggSortedHash *hash,
                                                                         gpointer       key,
                                                                         gpointer       value,
                                                                         guint         *position);

gboolean                egg_sorted_hash_remove                          (EggSortedHash *hash,
                                                                         gpointer       key,
                                                                         guint         *position);
