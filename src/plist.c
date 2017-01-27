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
	size_t elements_count;
};

static void plist_link_nodes(plist_linked_node *previous,
							   plist_linked_node *next);
static plist_linked_node *plist_create_node(void *data);
static plist_linked_node *plist_get_node(plist_list *self, size_t index);
static plist_linked_node *plist_find_node(plist_list *self,
		_Bool (*condition)(void *), size_t *index);
static void plist_merge_sort(plist_linked_node** headref,
							 _Bool (*comparator)(const void*, const void*));
static plist_linked_node* plist_sorted_merge(plist_linked_node* a,
											 plist_linked_node* b,
								_Bool (*comparator)(const void*, const void*));
static void plist_front_back_split(plist_linked_node* source,
								   plist_linked_node** frontRef,
								plist_linked_node** backRef);

plist_list *plist_create()
{
	plist_list *list = calloc(1,sizeof(plist_list));
	list->head = 0;
	list->elements_count = 0;
	return list;
}

int plist_append(plist_list *self, void *data)
{
	plist_linked_node *new_element = plist_create_node(data);
	plist_linked_node *last_node = plist_get_node(self, self->elements_count-1);

	if (self->elements_count == 0) {
		self->head = new_element;
	} else {
		plist_link_nodes(last_node, new_element);
	}

	return self->elements_count++;
}

void plist_merge(plist_list *self, plist_list *other)
{
	plist_linked_node *node = other->head;

	while (node){
		plist_append(self, node->data);
		node = node->next;
	}

}

void *plist_get(plist_list *self, size_t index)
{
	plist_linked_node *element_in_index = plist_get_node(self, index);
	return element_in_index ? element_in_index->data : 0;
}

void plist_add(plist_list *self, size_t index, void *data)
{
	plist_linked_node *next = 0;
	plist_linked_node *new_element = 0;
	plist_linked_node *previous = 0;

	if ((self->elements_count >= index)) {
		new_element = plist_create_node(data);

		if (index == 0) {
			plist_link_nodes(new_element, self->head);
			self->head = new_element;
		} else {
			next = plist_get_node(self, index);
			previous = plist_get_node(self, index - 1);
			plist_link_nodes(previous, new_element);
			plist_link_nodes(new_element, next);
		}

		self->elements_count++;
	}
}

void *plist_replace(plist_list *self, size_t index, void *data)
{
	void *old_data = 0;
	plist_linked_node *element = plist_get_node(self, index);

	if(element){
		old_data = element->data;
		element->data = data;
	}

	return old_data;
}

void plist_replace_and_destroy(plist_list *self, int index, void *data,
									void(*element_destroyer)(void *))
{
	void *old_data = plist_replace(self, index, data);
	element_destroyer(old_data);
}

void *plist_find(plist_list *self, bool(*condition)(void *))
{
	plist_linked_node *element = plist_find_node(self, condition, 0);
	return element ? element->data : 0;
}

void plist_iterate(plist_list *self, void(*closure)(void *))
{
	plist_linked_node *element = self->head;

	while (element){
		closure(element->data);
		element = element->next;
	}
}

void *plist_remove(plist_list *self, size_t index)
{
	void *data = 0;
	plist_linked_node *aux_element = 0;
	plist_linked_node *previous = 0;

	if(!self->head){
		return 0;
	}

	aux_element = plist_get_node(self, index);
	data = aux_element->data;

	if (index == 0) {
		self->head = aux_element->next;
	} else {
		previous = plist_get_node(self, index - 1);
		plist_link_nodes(previous, aux_element->next);
	}

	self->elements_count--;
	free(aux_element);
	return data;
}

void *plist_remove_selected(plist_list *self, bool(*condition)(void *))
{
	size_t index = 0;
	plist_linked_node *element = plist_find_node(self, condition, &index);

	return element ? plist_remove(self, index) : 0;
}

void plist_remove_and_destroy(plist_list *self, size_t index,
								   void(*element_destroyer)(void *))
{
	void *data = plist_remove(self, index);
	element_destroyer(data);
}

void plist_remove_destroying_selected(plist_list *self,
									   bool(*condition)(void *),
									   void(*element_destroyer)(void *))
{
	void *data = plist_remove_selected(self, condition);

	if(data){
		element_destroyer(data);
	}
}

size_t plist_size(plist_list *list)
{
	return list->elements_count;
}

size_t plist_isEmpty(plist_list *list)
{
	return plist_size(list) == 0;
}

void plist_clean(plist_list *self)
{
	plist_linked_node *element;

	while(self->head){
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

void plist_detroy(plist_list *self)
{
	plist_clean(self);
	free(self);
}

void plist_destroy_all(plist_list *self,
								  void(*element_destroyer)(void *))
{
	plist_clean_destroying_data(self, element_destroyer);
	free(self);
}

plist_list *plist_get_elements(plist_list *self, size_t count)
{
	plist_list *sublist = plist_create();
	void *data = 0;

	for (size_t i = 0; i < count; ++i) {
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

plist_list *plist_filter(plist_list *self, bool(*condition)(void *))
{
	plist_list *filtered = plist_create();
	plist_linked_node *element = self->head;

	while (element){
		if(condition(element))
			plist_append(filtered, element);
		element = element->next;
	}

	return filtered;
}

plist_list *plist_map(plist_list *self, void *(*transformer)(void *))
{
	plist_list *mapped = plist_create();
	plist_linked_node *element = self->head;

	while(element){
		plist_append(mapped, transformer(element));
		element = element->next;
	}

	return mapped;
}

void plist_sort(plist_list *self, _Bool (*comparator)(const void *, const void *))
{
	plist_merge_sort(&self->head, comparator);
}

size_t plist_count(plist_list *self, bool(*condition)(void *))
{
	plist_list *satisfying = plist_filter(self, condition);
	size_t result = satisfying->elements_count;
	plist_detroy(satisfying);
	return result;
}

bool plist_any_match(plist_list *self, bool(*condition)(void *))
{
	return plist_count(self, condition) > 0;
}

bool list_all_match(plist_list *self, bool(*condition)(void *))
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
	plist_linked_node *element = calloc(1,sizeof(plist_linked_node));
	element->data = data;
	element->next = 0;
	return element;
}

static plist_linked_node *plist_get_node(plist_list *self, size_t index)
{
	size_t cont = 0;
	plist_linked_node *element = 0;

	if((self->elements_count > index)){
		element = self->head;

		while (cont < index) {
			element = element->next;
			cont++;
		}
	}

	return element ? element : 0;
}

static plist_linked_node *plist_find_node(plist_list *self,
		bool(*condition)(void *), size_t *index)
{
	plist_linked_node *element = self->head;
	size_t position = 0;

	while(element && !condition(element->data)){
		element = element->next;
		position++;
	}

	if(index){
		*index = position;
	}

	return element;
}

/* sorts the linked list by changing next pointers (not data) */
static void plist_merge_sort(plist_linked_node** headref,
							 _Bool (*comparator)(const void *, const void *))
{
  plist_linked_node* head = *headref;
  plist_linked_node* a;
  plist_linked_node* b;

  /* Base case -- length 0 or 1 */
  if ((head == 0) || (head->next == 0))
  {
	return;
  }

  /* Split head into 'a' and 'b' sublists */
  plist_front_back_split(head, &a, &b);

  /* Recursively sort the sublists */
  plist_merge_sort(&a, comparator);
  plist_merge_sort(&b, comparator);

  /* answer = merge the two sorted lists together */
  *headref = plist_sorted_merge(a, b, comparator);
}

static plist_linked_node* plist_sorted_merge(plist_linked_node* a,
											 plist_linked_node* b,
							_Bool (*comparator)(const void *, const void *))
{
  plist_linked_node* result = NULL;

  /* Base cases */
  if (a == NULL)
	 return(b);
  else if (b==NULL)
	 return(a);

  /* Pick either a or b, and recur */
  if (comparator(a->data, b->data)){
	 result = a;
	 result->next = plist_sorted_merge(a->next, b, comparator);
  } else {
	 result = b;
	 result->next = plist_sorted_merge(a, b->next, comparator);
  }

  return(result);
}

static void plist_front_back_split(plist_linked_node* source,
		  plist_linked_node** frontRef, plist_linked_node** backRef)
{
  plist_linked_node* fast;
  plist_linked_node* slow;
  if (source==0 || source->next==0)
  {
	/* length < 2 cases */
	*frontRef = source;
	*backRef = 0;
  }
  else
  {
	slow = source;
	fast = source->next;

	/* Advance 'fast' two nodes, and advance 'slow' one node */
	while (fast != 0)
	{
	  fast = fast->next;
	  if (fast != 0)
	  {
		slow = slow->next;
		fast = fast->next;
	  }
	}

	/* 'slow' is before the midpoint in the list, so split it in two
	  at that point. */
	*frontRef = source;
	*backRef = slow->next;
	slow->next = 0;
  }
}
