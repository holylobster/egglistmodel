
#include <glib.h>
#include "eggsetliststore.h"
#include "eggkeyedliststore.h"

#define g_assert_cmppointer(n1, cmp, n2)    do { gpointer __n1 = (n1), __n2 = (n2); \
                                                 if (__n1 cmp __n2) ; else \
                                                   g_assertion_message_cmpnum (G_LOG_DOMAIN, __FILE__, __LINE__, G_STRFUNC, \
                                                     #n1 " " #cmp " " #n2, GPOINTER_TO_SIZE(__n1), #cmp, GPOINTER_TO_SIZE(__n2), 'x'); } while (0)

typedef struct
{
  guint position, removed, added;
} ExpectedItemsChanged;

static GList *pending_expected;

static void
expect_items_changed (guint position,
                      guint removed,
                      guint added)
{
  ExpectedItemsChanged *expected;

  expected = g_new0 (ExpectedItemsChanged, 1);
  expected->position = position;
  expected->removed = removed;
  expected->added = added;

  pending_expected = g_list_append (pending_expected, expected);
}

static void
items_changed (GListModel *model,
               guint       position,
               guint       removed,
               guint       added,
               gpointer    user_data)
{
  ExpectedItemsChanged *expected;

  if (pending_expected == NULL)
    g_error ("unexpected items-changed (%d, %d, %d)", position, removed, added);

  expected = pending_expected->data;
  pending_expected = g_list_remove_link (pending_expected, pending_expected);

  g_assert_cmpuint (expected->position, ==, position);
  g_assert_cmpuint (expected->removed, ==, removed);
  g_assert_cmpuint (expected->added, ==, added);

  g_free (expected);
}

static void
eggsetliststore (void)
{
  EggSetListStore *set;
  GObject *a;
  GObject *b;
  GObject *c;

  a = g_object_new (G_TYPE_OBJECT, NULL);
  b = g_object_new (G_TYPE_OBJECT, NULL);
  c = g_object_new (G_TYPE_OBJECT, NULL);

  g_object_add_weak_pointer (a, (gpointer *) &a);
  g_object_add_weak_pointer (b, (gpointer *) &b);
  g_object_add_weak_pointer (c, (gpointer *) &c);

  set = egg_set_list_store_new (G_TYPE_OBJECT);
  g_signal_connect (set, "items-changed", G_CALLBACK (items_changed), NULL);

  expect_items_changed (0, 0, 1);
  g_assert_cmpint (egg_set_list_store_add (set, a), ==, TRUE);
  expect_items_changed (1, 0, 1);
  g_assert_cmpint (egg_set_list_store_add (set, b), ==, TRUE);
  expect_items_changed (2, 0, 1);
  g_assert_cmpint (egg_set_list_store_add (set, c), ==, TRUE);
  g_assert_cmpuint (g_list_model_get_n_items (G_LIST_MODEL (set)), ==, 3);
  g_assert_cmpuint (egg_set_list_store_contains (set, a), ==, TRUE);
  g_assert_cmpuint (egg_set_list_store_contains (set, b), ==, TRUE);
  g_assert_cmpuint (egg_set_list_store_contains (set, c), ==, TRUE);

  /* add same item for a second time */
  g_assert_cmpint (egg_set_list_store_add (set, a), ==, FALSE);
  g_assert_cmpuint (g_list_model_get_n_items (G_LIST_MODEL (set)), ==, 3);

  /* remove an item */
  expect_items_changed (1, 1, 0);
  g_assert_cmpint (egg_set_list_store_remove (set, b), ==, TRUE);
  g_assert_cmpuint (g_list_model_get_n_items (G_LIST_MODEL (set)), ==, 2);
  g_assert_cmpuint (egg_set_list_store_contains (set, b), ==, FALSE);

  /* ... again */
  g_assert_cmpint (egg_set_list_store_remove (set, b), ==, FALSE);
  g_assert_cmpuint (g_list_model_get_n_items (G_LIST_MODEL (set)), ==, 2);

  g_object_unref (a);
  g_object_unref (b);
  g_object_unref (c);
  g_object_unref (set);

  g_assert_null (a);
  g_assert_null (b);
  g_assert_null (c);
  g_assert_null (pending_expected);
}

static void
eggkeyedliststore (void)
{
  EggKeyedListStore *store;
  GObject *a;
  GObject *b;
  GObject *c;

  a = g_object_new (G_TYPE_OBJECT, NULL);
  b = g_object_new (G_TYPE_OBJECT, NULL);
  c = g_object_new (G_TYPE_OBJECT, NULL);

  g_object_add_weak_pointer (a, (gpointer *) &a);
  g_object_add_weak_pointer (b, (gpointer *) &b);
  g_object_add_weak_pointer (c, (gpointer *) &c);

  store = egg_keyed_list_store_new (G_TYPE_OBJECT);
  g_signal_connect (store, "items-changed", G_CALLBACK (items_changed), NULL);

  expect_items_changed (0, 0, 1);
  g_assert_cmpint (egg_keyed_list_store_insert (store, "foo", a), ==, TRUE);
  expect_items_changed (1, 0, 1);
  g_assert_cmpint (egg_keyed_list_store_insert (store, "bar", b), ==, TRUE);
  expect_items_changed (2, 0, 1);
  g_assert_cmpint (egg_keyed_list_store_insert (store, "baz", c), ==, TRUE);
  g_assert_cmpuint (g_list_model_get_n_items (G_LIST_MODEL (store)), ==, 3);
  g_assert_cmppointer (egg_keyed_list_store_lookup (store, "foo"), ==, a);
  g_assert_cmppointer (egg_keyed_list_store_lookup (store, "bar"), ==, b);
  g_assert_cmppointer (egg_keyed_list_store_lookup (store, "baz"), ==, c);

  /* add same item for a second time */
  g_assert_cmpint (egg_keyed_list_store_insert (store, "foo", a), ==, FALSE);
  g_assert_cmpuint (g_list_model_get_n_items (G_LIST_MODEL (store)), ==, 3);

  /* remove an item */
  expect_items_changed (1, 1, 0);
  g_assert_cmpint (egg_keyed_list_store_remove (store, "bar"), ==, TRUE);
  g_assert_cmpuint (g_list_model_get_n_items (G_LIST_MODEL (store)), ==, 2);
  g_assert_cmppointer (egg_keyed_list_store_lookup (store, "bar"), ==, NULL);

  /* ... again */
  g_assert_cmpint (egg_keyed_list_store_remove (store, "bar"), ==, FALSE);
  g_assert_cmpuint (g_list_model_get_n_items (G_LIST_MODEL (store)), ==, 2);

  g_object_unref (a);
  g_object_unref (b);
  g_object_unref (c);
  g_object_unref (store);

  g_assert_null (a);
  g_assert_null (b);
  g_assert_null (c);
  g_assert_null (pending_expected);
}

int main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/egghashstores/eggsetliststore", eggsetliststore);
  g_test_add_func ("/egghashstores/eggkeyedliststore", eggkeyedliststore);

  return g_test_run ();
}
