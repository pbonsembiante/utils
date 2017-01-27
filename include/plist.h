/***************************************************************************
 * Copyright (C) Patricio Bonsembiante - Phanatos. All rights reserved.
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

#ifndef PLISTS_H
#define PLISTS_H

#include <stdbool.h>
#include <stdlib.h>
#include "pnode.h"

typedef struct plist_list plist_list;

plist_list *plist_create();
void plist_detroy(plist_list *);
void plist_destroy_all(plist_list *,
								  void(*element_destroyer)(void *));
int plist_append(plist_list *, void *element);
void plist_add(plist_list *, size_t index, void *element);
void plist_merge(plist_list *, plist_list *other);
void *plist_get(plist_list *, size_t index);
plist_list *plist_get_elements(plist_list *, size_t count);
plist_list *plist_get_removing_elements(plist_list *, size_t count);
plist_list *plist_filter(plist_list *, bool(*condition)(void *));
plist_list *plist_map(plist_list *, void *(*transformer)(void *));
void *plist_replace(plist_list *, size_t index, void *element);
void plist_replace_and_destroy(plist_list *, int index, void *element,
									void(*element_destroyer)(void *));
void *plist_remove(plist_list *, size_t index);
void plist_remove_and_destroy(plist_list *, size_t index,
								   void(*element_destroyer)(void *));
void *plist_remove_selected(plist_list *, bool(*condition)(void *));
void plist_remove_destroying_selected(plist_list *, bool(*condition)(void *),
									   void(*element_destroyer)(void *));
void plist_clean(plist_list *);
void plist_clean_destroying_data(plist_list *self,
								   void(*data_destroyer)(void *));
void plist_iterate(plist_list *, void(*closure)(void *));
void *plist_find(plist_list *, bool(*closure)(void *));
size_t plist_size(plist_list *);
size_t plist_isEmpty(plist_list *);
void plist_sort(plist_list *, _Bool (*comparator)(const void *, const void *));
int plist_countMatching(plist_list *self, bool(*condition)(void *));
bool plist_any_match(plist_list *self, bool(*condition)(void *));
bool list_all_match(plist_list *self, bool(*condition)(void *));

#endif // PLISTS_H
