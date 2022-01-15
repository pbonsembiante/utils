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

#ifndef _PLIST_H_
#define _PLIST_H_
/*!
 * \file plist.h
 * \brief Header file for List handling library
 *
 * Detail:
 *
 * This file contains all the required definitions to implement and use the
 * library. No internal or implementation details are provided in this headers.
 *
 */
#include "pnode.h"
#include <stdbool.h>
#include <stdlib.h>

/*!
 * \typedef plist_comparator
 * \brief User-defined function type to compare two objects.
 *
 * __Detail:__
 *
 * Mostly created to avoid using such an ugly syntax in function prototypes.
 * We strongly prefer this:
 * ~~~~~~~~~~~~~~~{.c}
 * void plist_sort(plist_list *self, plist_comparator comparator);
 * ~~~~~~~~~~~~~~~
 *
 * over this:
 * ~~~~~~~~~~~~~~~{.c}
 * void plist_sort(plist_list *self, bool (*comparator)(const void *, const void
 * *));
 * ~~~~~~~~~~~~~~~
 */
typedef bool (*plist_comparator)(const plist_member_t *,
                                 const plist_member_t *);

/*!
 * \typedef plist_evaluator
 * \brief User-defined function type to use custom boolean evaluations on data
 * elements.
 *
 * __Detail:__
 *
 * Mostly created to avoid using such an ugly syntax in function prototypes.
 * We strongly prefer this:
 * ~~~~~~~~~~~~~~~{.c}
 * plist_list *plist_filter(plist_list *self, plist_evaluator condition);
 * ~~~~~~~~~~~~~~~
 *
 * over this:
 * ~~~~~~~~~~~~~~~{.c}
 * plist_list *plist_filter(plist_list *self, bool (*condition)(void *));
 * ~~~~~~~~~~~~~~~
 */
typedef bool (*plist_evaluator)(const plist_member_t *);

/*!
 * \typedef plist_transformer
 * \brief User-defined function type to apply a modifier to all elements in the
 * list.
 *
 * __Detail:__
 *
 * Mostly created to avoid using such an ugly syntax in function prototypes.
 * We strongly prefer this:
 * ~~~~~~~~~~~~~~~{.c}
 * plist_list *plist_map(plist_list *self, plist_transformer transformer);
 * ~~~~~~~~~~~~~~~
 *
 * over this:
 * ~~~~~~~~~~~~~~~{.c}
 * plist_list *plist_map(plist_list *self, void* (*plist_transformer)(void *));
 * ~~~~~~~~~~~~~~~
 *
 */
typedef plist_member_t *(*plist_transformer)(const plist_member_t *);

/*!
 * \typedef plist_destroyer
 * \brief User-defined function type to free data elements allocated in the
 * list.
 *
 * __Detail:__
 *
 * Mostly created to avoid using such an ugly syntax in function prototypes.
 * We strongly prefer this:
 * ~~~~~~~~~~~~~~~{.c}
 * void plist_destroy_all(plist_list *self, plist_destroyer destroyer);
 * ~~~~~~~~~~~~~~~
 *
 * over this:
 * ~~~~~~~~~~~~~~~{.c}
 * void plist_destroy_all(plist_list *self, void (*destroyer)(void *));
 * ~~~~~~~~~~~~~~~
 *
 */
typedef void (*plist_destroyer)(void *);

/*!
 * \typedef plist_closure
 * \brief User-defined function type.
 *
 * __Detail:__
 *
 * I'll probably remove this... or adapt old code to use it... who knows.
 *
 */
typedef void (*plist_closure)(void *);

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
typedef struct plist plist;

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
 * ~~~~~~~~~~~~~~~{.c}
 * plist_list *L = plist_create();
 *
 * if(L) {
 *      //Now your list is ready to be used, destroy it when you're done with
 * it. } else {
 *      //Ups, something went wrong!
 * }
 * ~~~~~~~~~~~~~~~
 */
plist *plist_create(void);

/*!
 * \brief Frees and destroys the given list.
 * \param self: A pointer to the list to be freed.
 *
 * __Detail:__
 *
 * The given list will be destroyed, freeing any node.
 * If the internal data of the list needs to be freed too, [plist_destroy_all]
 * (@ref plist_destroy_all) should be used instead.
 *
 * ~~~~~~~~~~~~~~~{.c}
 * plist_list *L = plist_create();
 *
 * //Do some pretty things here!
 *
 * plist_destroy(L);
 * ~~~~~~~~~~~~~~~
 *
 */
void plist_destroy(plist **self);

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
 * ~~~~~~~~~~~~~~~{.c}
 * plist_list *L = plist_create();
 *
 * //Do some pretty things here!
 *
 * plist_destroy_all(L, free);
 * ~~~~~~~~~~~~~~~
 *
 */
void plist_destroy_all(plist **self, plist_destroyer destroyer);

/*!
 * \brief Add the given data as a new node to the end of the list.
 * \param self: A pointer to the list.
 * \param data: A pointer to the element to be appended.
 * \return the new current quantity of elements in the list.
 *
 * Creates a new node containing the data received and adds it to the end of the
 * list. If the list is empty, the new node becomes the head of the list.
 *
 * ~~~~~~~~~~~~~~~{.c}
 * plist_list *L = plist_create(); //L = []
 *
 * char *s1 = get_my_new_pretty_string();
 * char *s2 = get_other_pretty_string();
 *
 * plist_append(L, s1); //L = ["Pretty String"]
 * plist_append(L, s2); //L = ["Pretty String", "Another Pretty String"]
 *
 * ~~~~~~~~~~~~~~~
 *
 */
size_t plist_append(plist *self, plist_member_t *data);

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
 * ~~~~~~~~~~~~~~~{.c}
 * plist_list *L = plist_create(); //L = []
 *
 * char *s1 = get_my_new_pretty_string();
 * char *s2 = get_other_pretty_string();
 *
 * plist_add(L, 0, s1); //L = ["Pretty String"]
 * plist_add(L, 0, s2); //L = ["Another Pretty String", "Pretty String"]
 *
 * ~~~~~~~~~~~~~~~
 *
 */
void plist_add(plist *self, size_t index, plist_member_t *data);

/*!
 * \brief Merges two lists.
 * \param self: A pointer to the list that will receive the contents of the
 * other. \param other: A pointer to the list to be merged.
 *
 * __Detail:__
 *
 * This function will merge the \other other list into the one pointed by \self
 * self. Here _merge_ means append \other other to \self self, without
 * performing any sorting.
 *
 * If the resulting list needs to be sorted, [@ref plist_sort] should be used
 * after the merging occurs.
 *
 * ~~~~~~~~~~~~~~~{.c}
 * plist_list *L1 = plist_create(); //L1 = []
 * plist_list *L2 = plist_create(); //L2 = []
 *
 * char *s1 = get_my_new_pretty_string();
 * char *s2 = get_other_pretty_string();
 *
 * plist_append(L1, s1); //L1 = ["Pretty String"]
 * plist_append(L2, s2); //L2 = ["Another Pretty String"]
 *
 * plist_merge(L1, L2); //L1 = ["Pretty String", "Another Pretty String"]
 *
 * ~~~~~~~~~~~~~~~
 *
 */
void plist_merge(plist *self, plist *other);

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
void *plist_get(plist *self, size_t index);

/*!
 * \brief Returns a list of the first \count n elements in the list.
 * \param self: A pointer to a list object.
 * \param count: The amount of elements to retrieve.
 * \return: A pointer to a newly allocated list, containing \count count
 * elements from \self self.
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
plist *plist_get_elements(plist *self, size_t count);

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
plist *plist_get_removing_elements(plist *self, size_t count);

/*!
 * \brief Creates a new list with the elements that match a given condition.
 * \param self: A pointer to the list to filter.
 * \param condition: The condition that should be matched against to filter
 * elements. \return: A newly allocated list, containing the elements of \self
 * self that matched the \condition condition.
 *
 * __Detail:__
 *
 * This function returns a new list containing all the elements in \self self
 * that match the given \condition condition.
 *
 * If any element in \self self matches the condition, an empty list is
 * returned. To avoid this, [@ref plist_any_match] or [@ref
 * plist_count_matching] can be used, either to get how many items will the new
 * list hold, or to know if it is not going to be empty.
 *
 */
plist *plist_filter(plist *self, plist_evaluator condition);

/*!
 * \brief Creates a new list with the elements of \self self mapped with
 * \transformer transformer function. \param self: A pointer to the list to
 * transform. \param transformer: A pointer to a function that will be applied
 * to every element of the list. \return A pointer to a newly allocated list
 * containing all the elements in \self self ran through \transformer
 * transformer.
 *
 * __Detail:__
 *
 * This function returns a new list containing all the elements in \self self
 * ran through \transformer transformer.
 *
 */
plist *plist_map(plist *self, plist_transformer transformer);

/*!
 * \brief Replaces a data element of the list, with the new data provided.
 * \param self: A pointer to the list to transform.
 * \param index: Index of the element to be replaced.
 * \param data: New data element to be replaced at \index index position:
 * \return the old element at \index index in list.
 *
 * __Detail:__
 *
 * This function will replace the data in the \index index element of the list
 * with the new data element provided.
 *
 * Once the old element is replaced, the function will return a pointer to it.
 *
 */
void *plist_replace(plist *self, size_t index, plist_member_t *data);

/*!
 * \brief Replaces a data element of the list, and deletes the replaced element.
 * \param self: A pointer to the list to transform.
 * \param index:  Index of the element to be replaced.
 * \param data: New data element to be replaced at \index index position:
 * \param element_destroyer: A function to free the replaced element.
 *
 * __Detail:__
 *
 * This function will replace the data in the \index index element of the list
 * with the new data element provided.
 *
 * Once the old element is replaced, the function will not return the removed
 * element but apply the \destroyer destroyer function to it.
 */
void plist_replace_and_destroy(plist *self, size_t index, plist_member_t *data,
                               plist_destroyer destroyer);

/*!
 * \brief plist_remove
 * \param self
 * \param index
 * \return
 */
void *plist_remove(plist *self, size_t index);

/*!
 * \brief plist_remove_and_destroy
 * \param self
 * \param index
 * \param element_destroyer
 */
void plist_remove_and_destroy(plist *self, size_t index,
                              plist_destroyer destroyer);

/*!
 * \brief plist_remove_selected
 * \param self
 * \param condition
 * \return
 */
plist_member_t *plist_remove_selected(plist *self, plist_evaluator condition);

/*!
 * \brief plist_remove_destroying_selected
 * \param self
 * \param condition
 * \param element_destroyer
 */
void plist_remove_destroying_selected(plist *self, plist_evaluator condition,
                                      plist_destroyer destroyer);

/*!
 * \brief plist_clean
 * \param self
 */
void plist_clean(plist *self);

/*!
 * \brief plist_clean_destroying_data
 * \param self
 * \param data_destroyer
 */
void plist_clean_destroying_data(plist *self, plist_destroyer destroyer);

/*!
 * \brief plist_iterate
 * \param self
 * \param closure
 */
void plist_iterate(plist *self, plist_closure closure);

/*!
 * \brief plist_find
 * \param self
 * \param closure
 * \return
 */
void *plist_find(plist *self, plist_evaluator condition, size_t *index);

/*!
 * \brief plist_size
 * \param self
 * \return
 */
size_t plist_size(plist *self);

/*!
 * \brief plist_isEmpty
 * \param self
 * \return
 */
bool plist_is_empty(plist *self);

/*!
 * \brief plist_sort
 * \param self
 * \param comparator
 */
void plist_sort(plist *self, plist_comparator comparator);

/*!
 * \brief plist_count_matching
 * \param self
 * \param condition
 * \return
 */
size_t plist_count_matching(plist *self, plist_evaluator condition);

/*!
 * \brief plist_any_match
 * \param self
 * \param condition
 * \return
 */
bool plist_any_match(plist *self, plist_evaluator condition);

/*!
 * \brief plist_all_match
 * \param self
 * \param condition
 * \return
 */
bool plist_all_match(plist *self, plist_evaluator condition);

/*!
 * \brief plist_prepend
 * \param self
 * \param data
 * \return
 */
size_t plist_prepend(plist *self, plist_member_t *data);

/*
 * Handy macros
 */

#define PLIST_GET_INT(L, i) *((int *)plist_get(L, i))
#define PLIST_GET_PINT(L, i) ((int *)plist_get(L, i))

#define PLIST_GET_UINT(L, i) *((unsigned int *)plist_get(L, i))
#define PLIST_GET_PUINT(L, i) ((unsigned int *)plist_get(L, i))

#define PLIST_GET_CHAR(L, i) *((char *)plist_get(L, i))
#define PLIST_GET_UCHAR(L, i) *((unsigned char *)plist_get(L, i))
#define PLIST_GET_PCHAR(L, i) ((char *)plist_get(L, i))

#define PLIST_GET_FLOAT(L, i) *((float *)plist_get(L, i))
#define PLIST_GET_PFLOAT(L, i) ((float *)plist_get(L, i))

#define PLIST_GET_DOUBLE(L, i) *((double *)plist_get(L, i))
#define PLIST_GET_PDOUBLE(L, i) ((double *)plist_get(L, i))

#endif /* _PLIST_H_ */
