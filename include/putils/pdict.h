#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_
/***************************************************************************
 * Copyright (C) 2016 - 2017 Patricio Bonsembiante. All rights reserved.
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

#define PDICT_INITIAL_SIZE 20

#include "pnode.h"
#include <stdbool.h>


typedef struct pdict pdict;

pdict *pdict_create();

void pdict_put(pdict *self, char *key, void *data);

void *pdict_get(pdict *self, char *key);

void *pdict_remove(pdict *self, char *key);

void pdict_remove_and_destroy(pdict *self, char *key,
                              void(*data_destroyer)(void *));

void pdict_iterator(pdict *self, void(*closure)(char *, void *));

void pdict_clean(pdict *self);

void pdict_clean_and_destroy_elements(pdict *self,
                                      void(*destroyer)(void *));

bool pdict_has_key(pdict *self, char *key);

bool pdict_is_empty(pdict *self);

size_t pdict_size(pdict *self);

void pdict_destroy(pdict *self);

void pdict_destroy_and_destroy_elements(pdict *self,
                                        void(*destroyer)(void *));

#endif /* _DICTIONARY_H_ */
