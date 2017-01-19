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

#include <stdlib.h>
#include "plist.h"

struct plist_list
{
	plist_linked_node *head;
	int elements_count;
};

static void plist_link_element(plist_linked_node *previous,
							   plist_linked_node *next);
static plist_linked_node *plist_create_element(void *data);
static plist_linked_node *plist_get_element(plist_list *self, int index);
static plist_linked_node *plist_find_element(plist_list *self,
		bool(*condition)(void *), int *index);

plist_list *plist_create()
{
	plist_list *list = malloc(sizeof(plist_list));
	list->head = NULL;
	list->elements_count = 0;
	return list;
}

int plist_append(plist_list *self, void *data)
{
	plist_linked_node *new_element = plist_create_element(data);

	if (self->elements_count == 0) {
		self->head = new_element;
	} else {
		plist_link_element(plist_get_element(self, self->elements_count - 1),
						   new_element);
	}

	self->elements_count++;
	return self->elements_count - 1;
}

void plist_merge(plist_list *self, plist_list *other)
{
	void _add_in_list(void *element) {
		plist_append(self, element);
	}
	plist_iterate(other, _add_in_list);
}

void *plist_get(plist_list *self, int index)
{
	plist_linked_node *element_in_index = plist_get_element(self, index);
	return element_in_index != NULL ? element_in_index->data : NULL;
}

void plist_addElementByIndex(plist_list *self, int index, void *data)
{
	plist_linked_node *next = NULL;
	plist_linked_node *new_element = NULL;
	plist_linked_node *previous = NULL;

	if ((self->elements_count >= index) && (index >= 0)) {
		new_element = plist_create_element(data);

		if (index == 0) {
			plist_link_element(new_element, self->head);
			self->head = new_element;
		} else {
			next = plist_get_element(self, index);
			previous = plist_get_element(self, index - 1);
			plist_link_element(previous, new_element);
			plist_link_element(new_element, next);
		}

		self->elements_count++;
	}
}

void *plist_replace(plist_list *self, int index, void *data)
{
	void *old_data = NULL;
	plist_linked_node *element = plist_get_element(self, index);

	if (element != NULL) {
		old_data = element->data;
		element->data = data;
	}

	return old_data;
}

void plist_replaceAndDestroyElement(plist_list *self, int num, void *data,
									void(*element_destroyer)(void *))
{
	void *old_data = plist_replace(self, num, data);
	element_destroyer(old_data);
}

void *plist_find(plist_list *self, bool(*condition)(void *))
{
	plist_linked_node *element = plist_find_element(self, condition, NULL);
	return element != NULL ? element->data : NULL;
}

void plist_iterate(plist_list *self, void(*closure)(void *))
{
	plist_linked_node *element = self->head;

	while (element != NULL) {
		closure(element->data);
		element = element->next;
	}
}

void *plist_remove(plist_list *self, int index)
{
	void *data = NULL;
	plist_linked_node *aux_element = NULL;

	if (self->head == NULL) {
		return NULL;
	}

	aux_element = plist_get_element(self, index);
	data = aux_element->data;

	if (index == 0) {
		self->head = aux_element->next;
	} else {
		plist_linked_node *previous = plist_get_element(self, index - 1);
		plist_link_element(previous, aux_element->next);
	}

	self->elements_count--;
	free(aux_element);
	return data;
}

void *plist_removeByCondition(plist_list *self, bool(*condition)(void *))
{
	int index = 0;
	plist_linked_node *element = plist_find_element(self, condition, &index);

	if (element != NULL) {
		return plist_remove(self, index);
	}

	return NULL;
}

void plist_removeAndDestroyElement(plist_list *self, int index,
								   void(*element_destroyer)(void *))
{
	void *data = plist_remove(self, index);
	element_destroyer(data);
}

void plist_removeAndDestroyByCondition(plist_list *self,
									   bool(*condition)(void *), void(*element_destroyer)(void *))
{
	void *data = plist_removeByCondition(self, condition);

	if (data) {
		element_destroyer(data);
	}
}

int plist_size(plist_list *list)
{
	return list->elements_count;
}

int plist_isEmpty(plist_list *list)
{
	return plist_size(list) == 0;
}

void plist_clean(plist_list *self)
{
	plist_linked_node *element;

	while (self->head != NULL) {
		element = self->head;
		self->head = self->head->next;
		free(element);
	}

	self->elements_count = 0;
}

void plist_cleanAndDestroyElements(plist_list *self,
								   void(*element_destroyer)(void *))
{
	plist_iterate(self, element_destroyer);
	plist_clean(self);
}

void plist_detroy(plist_list *self)
{
	plist_clean(self);
	free(self);
}

void plist_destroyListAndElements(plist_list *self,
								  void(*element_destroyer)(void *))
{
	plist_cleanAndDestroyElements(self, element_destroyer);
	free(self);
}

plist_list *plist_getElements(plist_list *self, int count)
{
	plist_list *sublist = plist_create();
	int i = 0;

	for (i = 0; i < count; ++i) {
		void *element = plist_get(self, i);
		plist_append(sublist, element);
	}

	return sublist;
}

plist_list *plist_getElementsAndRemove(plist_list *self, int count)
{
	plist_list *sublist = plist_create();
	int i = 0;

	for (i = 0; i < count; ++i) {
		void *element = plist_remove(self, 0);
		plist_append(sublist, element);
	}

	return sublist;
}

plist_list *plist_filter(plist_list *self, bool(*condition)(void *))
{
	plist_list *filtered = plist_create();
	void _add_if_apply(void *element) {
		if (condition(element)) {
			plist_append(filtered, element);
		}
	}
	plist_iterate(self, _add_if_apply);
	return filtered;
}

plist_list *plist_map(plist_list *self, void *(*transformer)(void *))
{
	plist_list *mapped = plist_create();
	void _add_after_transform(void *element) {
		void *new_element = transformer(element);
		plist_append(mapped, new_element);
	}
	plist_iterate(self, _add_after_transform);
	return mapped;
}

void plist_sort(plist_list *self, bool (*comparator)(void *, void *))
{
	// TODO: optimizar (usar un algoritmo mas copado)
	int unsorted_elements = self->elements_count;

	if(unsorted_elements < 2) {
		return;
	}

	plist_linked_node *auxiliar = NULL;
	bool sorted = true;

	do {
		plist_linked_node *previous_element = self->head,
						   *cursor = previous_element->next;
		sorted = true;
		int index = 0, last_changed = unsorted_elements;

		while(index < unsorted_elements && cursor != NULL) {
			if(!comparator(previous_element->data, cursor->data)) {
				auxiliar = cursor->data;
				cursor->data = previous_element->data;
				previous_element->data = auxiliar;
				last_changed = index;
				sorted = false;
			}

			previous_element = cursor;
			cursor = cursor->next;
			index++;
		}

		unsorted_elements = last_changed;
	} while(!sorted);
}

int plist_count(plist_list *self, bool(*condition)(void *))
{
	plist_list *satisfying = plist_filter(self, condition);
	int result = satisfying->elements_count;
	plist_detroy(satisfying);
	return result;
}

bool plist_anyMatching(plist_list *self, bool(*condition)(void *))
{
	return plist_count(self, condition) > 0;
}

bool list_allMatching(plist_list *self, bool(*condition)(void *))
{
	return plist_count(self, condition) == self->elements_count;
}

/********* PRIVATE FUNCTIONS **************/

static void plist_link_element(plist_linked_node *previous,
							   plist_linked_node *next)
{
	if (previous != NULL) {
		previous->next = next;
	}
}

static plist_linked_node *plist_create_element(void *data)
{
	plist_linked_node *element = malloc(sizeof(plist_linked_node));
	element->data = data;
	element->next = NULL;
	return element;
}

static plist_linked_node *plist_get_element(plist_list *self, int index)
{
	int cont = 0;

	if ((self->elements_count > index) && (index >= 0)) {
		plist_linked_node *element = self->head;

		while (cont < index) {
			element = element->next;
			cont++;
		}

		return element;
	}

	return NULL;
}

static plist_linked_node *plist_find_element(plist_list *self,
		bool(*condition)(void *), int *index)
{
	plist_linked_node *element = self->head;
	int position = 0;

	while (element != NULL && !condition(element->data)) {
		element = element->next;
		position++;
	}

	if (index != NULL) {
		*index = position;
	}

	return element;
}
