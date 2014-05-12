
#include "eggsortedhash.h"

struct _EggSortedHash
{
  GSequence *items;
  GHashTable *iters;

  GCompareDataFunc sort_func;
  gpointer sort_func_data;

  GDestroyNotify key_free_func;
  GDestroyNotify value_free_func;
};

EggSortedHash *
egg_sorted_hash_new (GHashFunc      hash_func,
                     GEqualFunc     equal_func,
                     GDestroyNotify key_free_func,
                     GDestroyNotify value_free_func)
{
  EggSortedHash *hash;

  hash = g_new0 (EggSortedHash, 1);

  hash->items = g_sequence_new (value_free_func);
  hash->iters = g_hash_table_new_full (hash_func, equal_func, key_free_func, NULL);
  hash->key_free_func = key_free_func;
  hash->value_free_func = value_free_func;

  return hash;
}

void
egg_sorted_hash_free (EggSortedHash *hash)
{
  g_hash_table_unref (hash->iters);
  g_sequence_free (hash->items);

  g_free (hash);
}

guint
egg_sorted_hash_get_length (EggSortedHash *hash)
{
  return g_hash_table_size (hash->iters);
}

gpointer
egg_sorted_hash_lookup (EggSortedHash    *hash,
                        gpointer          key)
{
  GSequenceIter *it;

  it = g_hash_table_lookup (hash->iters, key);
  return it ? g_sequence_get (it) : NULL;
}

gpointer
egg_sorted_hash_get (EggSortedHash *hash,
                     guint          position)
{
  GSequenceIter *it;

  it = g_sequence_get_iter_at_pos (hash->items, position);
  return !g_sequence_iter_is_end (it) ? g_sequence_get (it) : NULL;
}

void
egg_sorted_hash_set_sort_func (EggSortedHash    *hash,
                               GCompareDataFunc  func,
                               gpointer          data)
{
  hash->sort_func = func;
  hash->sort_func_data = data;

  if (func)
    g_sequence_sort (hash->items, func, data);
}

gboolean
egg_sorted_hash_insert (EggSortedHash    *hash,
                        gpointer          key,
                        gpointer          value,
                        guint            *position)
{
  GSequenceIter *it;

  if (g_hash_table_contains (hash->iters, key))
    {
      if (hash->key_free_func)
        hash->key_free_func (key);
      if (hash->value_free_func)
        hash->value_free_func (value);
      return FALSE;
    }

  if (hash->sort_func)
    it = g_sequence_insert_sorted (hash->items, value, hash->sort_func, hash->sort_func_data);
  else
    it = g_sequence_append (hash->items, value);

  g_hash_table_insert (hash->iters, key, it);

  if (position)
    *position = g_sequence_iter_get_position (it);

  return TRUE;
}

gboolean
egg_sorted_hash_remove (EggSortedHash *hash,
                        gpointer       key,
                        guint         *position)
{
  GSequenceIter *it;

  it = g_hash_table_lookup (hash->iters, key);
  if (it)
    {
      if (position)
        *position = g_sequence_iter_get_position (it);

      g_sequence_remove (it);
      g_hash_table_remove (hash->iters, key);

      return TRUE;
    }

  return FALSE;
}
