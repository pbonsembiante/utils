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
#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_

#define PDICT_INITIAL_SIZE 20
#define KEYS_LEN 10

#include "pnode.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct pdict pdict;

typedef struct pdict_entry pdict_entry;
struct pdict_entry {
  void *value;
  char *key;
};

typedef struct pdict_entries pdict_entries;
struct pdict_entries {
  pdict_entry *entries;
  size_t count;
};

typedef void (*pdict_destroyer)(void *data);

typedef void (*pdict_closure)(char *key, void *value);

pdict *pdict_create();

void pdict_put(pdict *self, char *key, void *data);

void *pdict_get_value(pdict *self, char *key);

pdict_entry pdict_get(pdict *self, char *key);

pdict_entries pdict_get_all(pdict *self);

void *pdict_remove(pdict *self, char *key);

void pdict_remove_and_destroy(pdict *self, char *key, pdict_destroyer destroyer);

void pdict_iterate(pdict *self, pdict_closure closure);

void pdict_clean(pdict *self);

void pdict_clean_and_destroy_elements(pdict *self, pdict_destroyer destroyer);

bool pdict_has_key(pdict *self, char *key);

bool pdict_is_empty(pdict *self);

size_t pdict_size(pdict *self);

void pdict_destroy(pdict *self);

void pdict_destroy_all(pdict *self, pdict_destroyer destroyer);

#endif /* _DICTIONARY_H_ */
