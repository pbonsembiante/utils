/*
 * Copyright (C) 2012 Sistemas Operativos - UTN FRBA. All rights reserved.
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
 */

#ifndef DICTIONARY_H_
#define DICTIONARY_H_

#define PDICT_INITIAL_SIZE 20

#include "pnode.h"
#include <stdbool.h>


typedef struct pdict_dictionary pdict_dictionary;

pdict_dictionary *pdict_create();

void pdict_put(pdict_dictionary *self, char *key, void *data);

void *pdict_get(pdict_dictionary *self, char *key);

void *pdict_remove(pdict_dictionary *self, char *key);

void pdict_remove_and_destroy(pdict_dictionary *self, char *key,
							  void(*data_destroyer)(void *));

void pdict_iterator(pdict_dictionary *self, void(*closure)(char *, void *));

void pdict_clean(pdict_dictionary *self);

void pdict_clean_and_destroy_elements(pdict_dictionary *self,
									  void(*destroyer)(void *));

bool pdict_has_key(pdict_dictionary *self, char *key);

bool pdict_is_empty(pdict_dictionary *self);

int pdict_size(pdict_dictionary *self);

void pdict_destroy(pdict_dictionary *self);

void pdict_destroy_and_destroy_elements(pdict_dictionary *self,
										void(*destroyer)(void *));

#endif /* DICTIONARY_H_ */
