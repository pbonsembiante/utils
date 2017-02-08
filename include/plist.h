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
/*!
 * \file plist.h
 * \brief Header file for List handling library
 *
 * Detail:
 *
 * This file contains all the requeried definitions to implement and use the
 * library. No internal or implementation details are provided in this headers.
 *
 */

#ifndef PLISTS_H
#define PLISTS_H

#include <stdbool.h>
#include <stdlib.h>
#include "pnode.h"

/*!
 * \typedef plist_list
 * \brief Type definition for abstract list handler.
 *
 * __Detail:__
 *
 * Forward declaration for list handling. No need to reveal structure internals
 * since they are implementation dependent and can change at any time.
 *
 * All functions provided know how to handle the structure properly.
 *
 */
typedef struct plist_list plist_list;

/*!
 * \brief Initialize the list pointer.
 * \return A pointer to the newly created list.
 *
 * __Detail:__
 *
 * This will allocate the space required for the list handler and
 * initialize an empty list.
 *
 * This function should always have a corresponding call to plist_destroy*
 * to free the allocated memory after the object is no longer required
 *
 * *Or any similar destroyer functions.
 *
 */
plist_list *plist_create(void);

/*!
 * \brief Frees and destroys the given list.
 * \param self: A pointer to the list to be freed.
 *
 * __Detail:__
 *
 * The given list will be destroyed, freeing any node.
 * If the internal data of the list needs to be freed too, [plist_detroy_all]
 * (@ref plist_destroy_all) shoud be used instead.
 *
 */
void plist_destroy(plist_list *self);

/*!
 * \brief Frees and destroys the given list and internal data.
 * \param self: A pointer to the list to be freed.
 * \param element_destroyer: A function meant to free the data pointer of the
 * list. This is specific of the data stored in the list and should be able to
 * handle the data in the pointer.
 *
 * __Detail:__
 *
 * The given list will be destroyed, freeing every node in the list as well as
 * the data in these nodes. The \element_destroyer element_destroyer function is
 * applied to every element in the list.
 *
 * The element_destroyer function is of the form:
 * ~~~~~~~~~~~~~~~{.c}
 * void element_destroyer(void *element);
 * ~~~~~~~~~~~~~~~
 *
 */
void plist_destroy_all(plist_list *self,
					   void(*element_destroyer)(void *));

/*!
 * \brief Add the given data as a new node to the end of the list.
 * \param self: A pointer to the list.
 * \param data: A pointer to the element to be appended.
 * \return the new current quantity of elements in the list.
 *
 * Creates a new node containing the data received and adds it to the end of the
 * list. If the list is empty, the new node becomes the head of the list.
 *
 */
size_t plist_append(plist_list *self, void *data);

/*!
 * \brief Adds \data data at \index index position on the list.
 * \param self: A pointer to the list.
 * \param index: The position in the given list where the data will be added.
 * \param data: A pointer to the data to be added.
 *
 * __Detail:__
 *
 * Adds the given element in the position indicated by \index index to the
 * desired list.
 *
 * A node will be automatically created containing the data.
 *
 */
void plist_add(plist_list *self, size_t index, void *data);

/*!
 * \brief Merges two lists.
 * \param self: A pointer to the list that will receive the contents of the
 * other.
 * \param other: A pointer to the list to be merged.
 *
 * __Detail:__
 *
 * This function will merge the \other other list into the one pointet by \self
 * self. Here _merge_ means append \other other to \self self, without
 * performing any sorting.
 *
 * If the resulting list needs to be sorted, [@ref plist_sort] should be used
 * after the merging occurs.
 *
 */
void plist_merge(plist_list *self, plist_list *other);

/*!
 * \brief Gets the data of the index-th position of the given list.
 * \param self: The list to retrieve the data from.
 * \param index: The index of the data element in the list.
 * \return: A pointer to the data element in the index-th position in the list
 * or null.
 *
 * __Detail:__
 *
 * Not to much to detail, get the index-th element of the list. If the list is
 * empty, or there is no element at the given position, the function will return
 * null.
 */
void *plist_get(plist_list *self, size_t index);

/*!
 * \brief Returns a list of the first \count n elements in the list.
 * \param self: A pointer to a list object.
 * \param count: The amount of elements to retrieve.
 * \return: A pointer to a newly allocated list, containing \count count elements
 * from \self self.
 *
 * __Detail:__
 *
 * This function will create a new list, append the first \count count
 * elements from \self self list and return a pointer to the new list.
 *
 * Each new node will be allocated then the resulting list will be independent
 * of the original.
 *
 * If memory allocation fails, null is returned.
 *
 */
plist_list *plist_get_elements(plist_list *self, size_t count);

/*!
 * \brief Returns a list of the first \count n elements in the list and removes
 * those elements from it.
 * \param self: A pinter to the original list
 * \param count: The amount of elements to retrieve.
 * \return: A pointer to a newly allocated list, with \count count elements form
 * \self self.
 *
 * __Detail:__
 *
 * Same behaviour as [@ref plist_get_elements] but removes the selected elements
 * from the original list.
 *
 */
plist_list *plist_get_removing_elements(plist_list *self, size_t count);

/*!
 * \brief: Creates a new list with the elements that match a given condition.
 * \param self: A pointer to the list to filter.
 * \param condition: The condition that should be matched against.
 * \return: A newly allocated list, containing the elements of \self self that
 * matched the \condition condition.
 *
 * __Detail:__
 *
 *
 */
plist_list *plist_filter(plist_list *self, _Bool(*condition)(void *));

/*!
 * \brief plist_map
 * \param self
 * \param transformer
 * \return
 */
plist_list *plist_map(plist_list *self, void *(*transformer)(void *));

/*!
 * \brief plist_replace
 * \param self
 * \param index
 * \param element
 * \return
 */
void *plist_replace(plist_list *self, size_t index, void *element);

/*!
 * \brief plist_replace_and_destroy
 * \param self
 * \param index
 * \param element
 * \param element_detroyer
 */
void plist_replace_and_destroy(plist_list *self, int index, void *element,
							   void(*element_destroyer)(void *));

/*!
 * \brief plist_remove
 * \param self
 * \param index
 * \return
 */
void *plist_remove(plist_list *self, size_t index);

/*!
 * \brief plist_remove_and_destroy
 * \param self
 * \param index
 * \param element_destroyer
 */
void plist_remove_and_destroy(plist_list *self, size_t index,
							  void(*element_destroyer)(void *));

/*!
 * \brief plist_remove_selected
 * \param self
 * \param condition
 * \return
 */
void *plist_remove_selected(plist_list *self, _Bool(*condition)(void *));

/*!
 * \brief plist_remove_destroying_selected
 * \param self
 * \param condition
 * \param element_destroyer
 */
void plist_remove_destroying_selected(plist_list *self,
									  _Bool(*condition)(void *),
									  void(*element_destroyer)(void *));

/*!
 * \brief plist_clean
 * \param self
 */
void plist_clean(plist_list *self);

/*!
 * \brief plist_clean_destroying_data
 * \param self
 * \param data_destroyer
 */
void plist_clean_destroying_data(plist_list *self,
								 void(*data_destroyer)(void *));

/*!
 * \brief plist_iterate
 * \param self
 * \param closure
 */
void plist_iterate(plist_list *self, void(*closure)(void *));

/*!
 * \brief plist_find
 * \param self
 * \param closure
 * \return
 */
void *plist_find(plist_list *self, _Bool(*closure)(void *));

/*!
 * \brief plist_size
 * \param self
 * \return
 */
size_t plist_size(plist_list *self);

/*!
 * \brief plist_isEmpty
 * \param self
 * \return
 */
size_t plist_is_empty(plist_list *self);

/*!
 * \brief plist_sort
 * \param self
 * \param comparator
 */
void plist_sort(plist_list *self, _Bool (*comparator)(const void *,
				const void *));

/*!
 * \brief plist_count_matching
 * \param self
 * \param condition
 * \return
 */
int plist_count_matching(plist_list *self, _Bool(*condition)(void *));

/*!
 * \brief plist_any_match
 * \param self
 * \param condition
 * \return
 */
_Bool plist_any_match(plist_list *self, _Bool(*condition)(void *));

/*!
 * \brief plist_all_match
 * \param self
 * \param condition
 * \return
 */
_Bool plist_all_match(plist_list *self, _Bool(*condition)(void *));

#endif // PLISTS_H
