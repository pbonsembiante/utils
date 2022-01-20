/***************************************************************************
 * Copyright (C) 2016 - 2022 Patricio Bonsembiante. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include "putils/pdict.h"
#include <stdlib.h>
#include <string.h>

struct pdict {
  pdict_node **elements;
  size_t table_max_size;
  size_t table_current_size;
  size_t elements_count;
};

static size_t pdict_hash(unsigned char const *str);

static void pdict_resize(pdict *self, size_t new_max_size);

static pdict_node *pdict_create_element(char *key, size_t key_hash, size_t key_len, void *data);

static pdict_node *pdict_get_element(pdict *self, char *key);

static void *pdict_remove_element(pdict *self, char *key);

static void pdict_destroy_element(pdict_node *element, pdict_destroyer destroyer);

static void
internal_dictionary_clean_and_destroy_elements(pdict *self, pdict_destroyer destroyer);

#if defined __USE_XOPEN2K8 || __GLIBC_USE (LIB_EXT2) || __GLIBC_USE (ISOC2X)
#define pdict_strndup strndup
#else
#define pdict_strndup(s, n) \
  {                                                                            \
    const char *__old = (s);						       \
    size_t __len = strnlen(__old, (n));				               \
    char *__new = (char *)__builtin_alloca(__len + 1);		               \
    __new[__len] = '\0';						       \
    (char *)memcpy(__new, __old, __len);\
  }
#endif

pdict *pdict_create() {
  pdict *self = calloc(1, sizeof(pdict));
  self->table_max_size = PDICT_INITIAL_SIZE;
  self->elements = calloc(self->table_max_size, sizeof(pdict_node *));
  self->table_current_size = 0;
  self->elements_count = 0;
  return self;
}

void pdict_put(pdict *self, char *key, void *data) {
  size_t key_hash = pdict_hash((unsigned char *)key);
  size_t key_len = strlen(key);
  size_t index = key_hash % self->table_max_size;

  pdict_node *new_element =
    pdict_create_element(pdict_strndup(key, key_len), key_hash, key_len, data);

  pdict_node *element = self->elements[index];

  if (!element) {
    self->elements[index] = new_element;
    self->table_current_size++;

    if (self->table_current_size >= self->table_max_size) {
      pdict_resize(self, self->table_max_size * 2);
    }
  } else {
    while (element->next) {
      element = element->next;
    }

    element->next = new_element;
  }

  self->elements_count++;
}

void *pdict_get_value(pdict *self, char *key) {
  pdict_node *element = pdict_get_element(self, key);
  return element ? element->data : 0;
}

void *pdict_remove(pdict *self, char *key) {
  void *data = pdict_remove_element(self, key);

  if (data != 0) {
    self->elements_count--;
  }

  return data;
}

void pdict_remove_and_destroy(pdict *self, char *key, pdict_destroyer destroyer) {
  if (!self || !key || !destroyer)
    return;

  void *data = pdict_remove_element(self, key);

  if (data != 0) {
    self->elements_count--;
    destroyer(data);
  }
}

void pdict_iterate(pdict *self, pdict_closure closure) {
  for (size_t index = 0; index < self->table_max_size; ++index) {
    pdict_node *element = self->elements[index];

    while (element != 0) {
      closure(element->key, element->data);
      element = element->next;
    }
  }
}

void pdict_clean(pdict *self) {
  internal_dictionary_clean_and_destroy_elements(self, 0);
}

void pdict_clean_and_destroy_elements(pdict *self, pdict_destroyer destroyer) {
  internal_dictionary_clean_and_destroy_elements(self, destroyer);
}

bool pdict_has_key(pdict *self, char *key) {
  return pdict_get_element(self, key) != 0;
}

bool pdict_is_empty(pdict *self) { return self->elements_count == 0; }

size_t pdict_size(pdict *self) { return self->elements_count; }

void pdict_destroy(pdict *self) {
  pdict_clean(self);
  free(self->elements);
  free(self);
}

void pdict_destroy_all(pdict *self, pdict_destroyer destroyer) {
  pdict_clean_and_destroy_elements(self, destroyer);
  free(self->elements);
  free(self);
}

pdict_entry pdict_get(pdict *self, char *key) {
  pdict_node *element = pdict_get_element(self, key);
  return (pdict_entry) {
    .key = element->key,
    .value = element->data,
  };
}

pdict_entries pdict_get_all(pdict *self) {
  if (!self) return (pdict_entries) {0};
  pdict_entries entries = {
    .count = self->elements_count,
    .entries = calloc(self->elements_count, sizeof(pdict_entries))
  };

  for (size_t i = 0, j = 0; i < self->elements_count; ++j) {
    pdict_node *element = self->elements[j];
    if (!element) continue;

    entries.entries[i].key = element->key;
    entries.entries[i++].value = element->data;

    while (element->next) {
      element = element->next;
      entries.entries[i].key = element->key;
      entries.entries[i++].value = element->data;
    }
  }

  return entries;
}

static void pdict_resize(pdict *self, size_t new_max_size) {
  pdict_node **new_table = calloc(new_max_size, sizeof(pdict_node *));
  pdict_node **old_table = self->elements;
  self->table_current_size = 0;
  size_t table_index;

  for (table_index = 0; table_index < self->table_max_size; table_index++) {
    pdict_node *old_element = old_table[table_index];
    pdict_node *next_element;

    while (old_element != 0) {
      // new position
      size_t new_index = old_element->hashcode % new_max_size;
      pdict_node *new_element = new_table[new_index];

      if (new_element == 0) {
        new_table[new_index] = old_element;
        self->table_current_size++;
      } else {
        while (new_element->next != 0) {
          new_element = new_element->next;
        }

        new_element->next = old_element;
      }

      next_element = old_element->next;
      old_element->next = 0;
      old_element = next_element;
    }
  }

  self->elements = new_table;
  self->table_max_size = new_max_size;
  free(old_table);
}

static void
internal_dictionary_clean_and_destroy_elements(pdict *self, pdict_destroyer destroyer) {
  for (size_t table_index = 0; table_index < self->table_max_size;
       table_index++) {

    pdict_node *element = self->elements[table_index];

    while (element != 0) {
      pdict_node *next_element = element->next;
      pdict_destroy_element(element, destroyer);
      element = next_element;
    }

    self->elements[table_index] = 0;
  }

  self->table_current_size = 0;
  self->elements_count = 0;
}

static pdict_node *pdict_create_element(char *key, size_t key_hash,
                                        size_t key_len, void *data) {
  pdict_node *element = malloc(sizeof(pdict_node));
  element->key = key;
  element->key_len = key_len;
  element->hashcode = key_hash;
  element->data = data;
  element->next = 0;
  return element;
}

static pdict_node *pdict_get_element(pdict *self, char *key) {
  size_t input_key_hash = pdict_hash((unsigned char *)key);
  size_t index = input_key_hash % self->table_max_size;
  pdict_node *element = self->elements[index];

  if (!element) {
    return 0;
  }

  #ifdef __USE_XOPEN2K8
  size_t smallest_key_len = strnlen(key, element->key_len);
  #else
  size_t key_len = strlen(key);
  size_t smallest_key_len = key_len < element->key_len
                            ? key_len
                            : element->key_len;
  #endif
  do {
    if (element->hashcode == input_key_hash
        && strncmp(element->key, key, smallest_key_len) == 0) {
      return element;
    }
  } while ((element = element->next) != 0);

  return 0;
}

static void *pdict_remove_element(pdict *self, char *key) {
  unsigned long key_hash = pdict_hash((unsigned char *)key);
  size_t index = key_hash % self->table_max_size;
  pdict_node *element = self->elements[index];

  if (!element) {
    return 0;
  }

  if (element->hashcode == key_hash) {
    void *data = element->data;
    self->elements[index] = element->next;

    if (self->elements[index] == 0) {
      self->table_current_size--;
    }

    free(element->key);
    free(element);
    return data;
  }

  while (element->next != 0) {
    if (element->next->hashcode == key_hash) {
      void *data = element->next->data;
      pdict_node *aux = element->next;
      element->next = element->next->next;
      free(aux->key);
      free(aux);
      return data;
    }

    element = element->next;
  }

  return 0;
}

static void pdict_destroy_element(pdict_node *element, pdict_destroyer destroyer) {
  if (destroyer) {
    destroyer(element->data);
  }

  if (element->key) free(element->key);
  free(element);
}

static size_t pdict_hash(unsigned char const *str) {
  size_t hash = 5381;
  int c;

  while ((c = *str++)) {
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  }

  return hash;
}
