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
#include <stdbool.h>
#include "putils/plist.h"

struct plist_list
{
	plist_linked_node *head;
	plist_linked_node *tail;
	size_t elements_count;
};

static void plist_link_nodes(plist_linked_node *previous,
                             plist_linked_node *next);

static plist_linked_node *plist_create_node(void *data);

static plist_linked_node *plist_get_node(plist_list *self, size_t index);

static plist_linked_node *plist_find_node(plist_list *self,
        plist_evaluator condition, size_t *index);

static void plist_merge_sort(plist_linked_node **headref,
                             plist_comparator comparator);

static plist_linked_node *plist_sorted_merge(plist_linked_node *self,
        plist_linked_node *other,
        plist_comparator comparator);

static void plist_front_back_split(plist_linked_node *source,
                                   plist_linked_node **frontRef,
                                   plist_linked_node **backRef);

plist_list *plist_create(void)
{
	plist_list *list = calloc(1, sizeof(plist_list));
	list->head = 0;
    list->tail = 0;
	list->elements_count = 0;
	return list;
}

size_t plist_append(plist_list *self, void *data)
{
    plist_linked_node *new_element = plist_create_node(data);
    plist_linked_node *last = self->tail;

	if (self->elements_count == 0) {
        self->head = new_element;
    } else {
        plist_link_nodes(last, new_element);
	}

    self->tail = new_element;

	return self->elements_count++;
}

void plist_merge(plist_list *self, plist_list *other)
{
	plist_linked_node *node = other->head;

	while (node) {
		plist_append(self, node->data);
		node = node->next;
	}
}

void *plist_get(plist_list *self, size_t index)
{
    plist_linked_node *element = plist_get_node(self, index);
    return element ? element->data : 0;
}

void plist_add(plist_list *self, size_t index, void *data)
{
	plist_linked_node *new_element = 0;

    if (self->elements_count >= index) {
		new_element = plist_create_node(data);

		if (index == 0) {
			plist_link_nodes(new_element, self->head);
			self->head = new_element;

            if (self->elements_count == 0) {
                self->tail = new_element;
            }
		} else {
			plist_linked_node *previous = 0;
			plist_linked_node *next = 0;
			previous = plist_get_node(self, index - 1);
			next = previous->next;
			plist_link_nodes(previous, new_element);
			plist_link_nodes(new_element, next);

            if (previous == self->tail) {
               self->tail = new_element;
            }
		}

		self->elements_count++;
	}
}

void *plist_replace(plist_list *self, size_t index, void *data)
{
	void *old_data = 0;
	plist_linked_node *element = plist_get_node(self, index);

	if(element) {
		old_data = element->data;
		element->data = data;
	}

	return old_data;
}

void plist_replace_and_destroy(plist_list *self, int index, void *data,
                               plist_destroyer destroyer)
{
	void *old_data = plist_replace(self, index, data);
    destroyer(old_data);
}

void *plist_find(plist_list *self, plist_evaluator condition)
{
	plist_linked_node *element = plist_find_node(self, condition, 0);
	return element ? element->data : 0;
}

void plist_iterate(plist_list *self, plist_closure closure)
{
	plist_linked_node *element = self->head;

	while (element) {
		closure(element->data);
		element = element->next;
	}
}

void *plist_remove(plist_list *self, size_t index)
{
    void *data = 0;
    plist_linked_node *aux = 0;
    plist_linked_node *previous = 0;

    previous = plist_get_node(self, index - 1);

    if (!previous || !previous->next || !self->head) {
        return 0;
    }

    aux = previous->next;
    data = aux->data;

	if (index == 0) {
        self->head = aux->next;
        if (!self->head) {
            self->tail = 0;
        }
	} else {
        plist_link_nodes(previous, aux->next);
        if (aux->next == 0) {
           self->tail = previous;
        }
	}

	self->elements_count--;
    free(aux);
	return data;
}

void *plist_remove_selected(plist_list *self, plist_evaluator condition)
{
	size_t index = 0;
	plist_linked_node *element = plist_find_node(self, condition, &index);
	return element ? plist_remove(self, index) : 0;
}

void plist_remove_and_destroy(plist_list *self, size_t index,
                              plist_destroyer destroyer)
{
	void *data = plist_remove(self, index);
    destroyer(data);
}

void plist_remove_destroying_selected(plist_list *self,
                                      plist_evaluator condition,
                                      plist_destroyer destroyer)
{
	void *data = plist_remove_selected(self, condition);

	if(data) {
        destroyer(data);
	}
}

size_t plist_size(plist_list *self)
{
    return self->elements_count;
}

size_t plist_is_empty(plist_list *self)
{
    return plist_size(self) == 0;
}

void plist_clean(plist_list *self)
{
	while(self->head) {
		plist_linked_node *element;
		element = self->head;
		self->head = self->head->next;
		free(element);
	}

	self->elements_count = 0;
}

void plist_clean_destroying_data(plist_list *self,
                                 void(*data_destroyer)(void *))
{
	plist_iterate(self, data_destroyer);
	plist_clean(self);
}

void plist_destroy(plist_list *self)
{
	plist_clean(self);
	free(self);
}

void plist_destroy_all(plist_list *self,
                       plist_destroyer destroyer)
{
    plist_clean_destroying_data(self, destroyer);
	free(self);
}

plist_list *plist_get_elements(plist_list *self, size_t count)
{
	plist_list *sublist = plist_create();

	for (size_t i = 0; i < count; ++i) {
		void *data = 0;
		data = plist_get(self, i);
		plist_append(sublist, data);
	}

	return sublist;
}

plist_list *plist_get_removing_elements(plist_list *self, size_t count)
{
	plist_list *sublist = plist_create();

	for (size_t i = 0; i < count; ++i) {
		void *element = plist_remove(self, 0);
		plist_append(sublist, element);
	}

	return sublist;
}

plist_list *plist_filter(plist_list *self, plist_evaluator condition)
{
	plist_list *filtered = plist_create();
	plist_linked_node *element = self->head;

	while (element) {
		if(condition(element)) {
			plist_append(filtered, element);
		}

		element = element->next;
	}

	return filtered;
}

plist_list *plist_map(plist_list *self, void *(*transformer)(void *))
{
	plist_list *mapped = plist_create();
	plist_linked_node *element = self->head;

	while(element) {
		plist_append(mapped, transformer(element));
		element = element->next;
	}

	return mapped;
}

void plist_sort(plist_list *self, plist_comparator comparator)
{
	plist_merge_sort(&self->head, comparator);
}

size_t plist_count(plist_list *self, plist_evaluator condition)
{
	plist_list *satisfying = plist_filter(self, condition);
	size_t result = satisfying->elements_count;
	plist_destroy(satisfying);
	return result;
}

bool plist_any_match(plist_list *self, plist_evaluator condition)
{
	return plist_count(self, condition) > 0;
}

bool plist_all_match(plist_list *self, plist_evaluator condition)
{
	return plist_count(self, condition) == self->elements_count;
}

/********* PRIVATE FUNCTIONS **************/

static void plist_link_nodes(plist_linked_node *previous,
                             plist_linked_node *next)
{
	if (previous) {
		previous->next = next;
	}
}

static plist_linked_node *plist_create_node(void *data)
{
	plist_linked_node *element = calloc(1, sizeof(plist_linked_node));
    if (element) {
        element->data = data;
        element->next = 0;
    }
	return element;
}

static plist_linked_node *plist_get_node(plist_list *self, size_t index)
{
	plist_linked_node *element = 0;
    bool ok = self->elements_count > index;

    if( ok && (self->elements_count - 1) == index) {
        element = self->tail;
    }

    if( ok && !element) {
        size_t count = 0;
		element = self->head;

        while (count++ < index) {
			element = element->next;
		}
	}

	return element ? element : 0;
}

static plist_linked_node *plist_find_node(plist_list *self,
        plist_evaluator condition, size_t *index)
{
	plist_linked_node *element = self->head;
	size_t position = 0;

	while(element && !condition(element->data)) {
		element = element->next;
		position++;
	}

	if(index) {
		*index = position;
	}

	return element;
}

static void plist_merge_sort(plist_linked_node **headref,
                             plist_comparator comparator)
{
	plist_linked_node *head = *headref;
	plist_linked_node *a;
	plist_linked_node *b;

	if ((head == 0) || (head->next == 0)) {
		return;
	}

	plist_front_back_split(head, &a, &b);
	plist_merge_sort(&a, comparator);
	plist_merge_sort(&b, comparator);
	*headref = plist_sorted_merge(a, b, comparator);
}

static plist_linked_node *plist_sorted_merge(plist_linked_node *self,
        plist_linked_node *other,
        plist_comparator comparator)
{
	plist_linked_node *result = 0;

	/* Base cases */
	if (!self) {
		return(other);
	} else if (!other) {
		return(self);
	}

	if (comparator(self->data, other->data)) {
		result = self;
		result->next = plist_sorted_merge(self->next, other, comparator);
	} else {
		result = other;
		result->next = plist_sorted_merge(self, other->next, comparator);
	}

	return(result);
}

static void plist_front_back_split(plist_linked_node *source,
                                   plist_linked_node **frontRef,
                                   plist_linked_node **backRef)
{
	plist_linked_node *fast;
	plist_linked_node *slow;

	if (source == 0 || source->next == 0) {
		/* length < 2 cases */
		*frontRef = source;
		*backRef = 0;
	} else {
		slow = source;
		fast = source->next;

		while (fast != 0) {
			fast = fast->next;

			if (fast != 0) {
				slow = slow->next;
				fast = fast->next;
			}
		}

		*frontRef = source;
		*backRef = slow->next;
		slow->next = 0;
	}
}
