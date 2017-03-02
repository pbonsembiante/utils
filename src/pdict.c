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
#include <string.h>
#include "putils/pdict.h"

struct pdict_dictionary
{
	phashmap_node **elements;
	int table_max_size;
	int table_current_size;
	int elements_count;
};

static unsigned int pdict_hash(char *key, int key_len);

static void pdict_resize(pdict_dictionary *, int new_max_size);

static phashmap_node *pdict_create_element(char *key,
        unsigned int key_hash,
        void *data);

static phashmap_node *pdict_get_element(pdict_dictionary *self, char *key);

static void *pdict_remove_element(pdict_dictionary *self, char *key);

static void pdict_destroy_element(phashmap_node *element,
                                  void(*data_destroyer)(void *));

static void internal_dictionary_clean_and_destroy_elements(
    pdict_dictionary *self,
    void(*data_destroyer)(void *));

pdict_dictionary *pdict_create()
{
	pdict_dictionary *self = calloc(1, sizeof(pdict_dictionary));
	self->table_max_size = PDICT_INITIAL_SIZE;
	self->elements = calloc(self->table_max_size, sizeof(phashmap_node));
	self->table_current_size = 0;
	self->elements_count = 0;
	return self;
}

void pdict_put(pdict_dictionary *self, char *key, void *data)
{
	unsigned int key_hash = pdict_hash(key, strlen(key));
	int index = key_hash % self->table_max_size;
    /* TODO: change strdup since no standard C */
    phashmap_node *new_element = pdict_create_element(strdup(key), key_hash,
                                      data);
	phashmap_node *element = self->elements[index];

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

void *pdict_get(pdict_dictionary *self, char *key)
{
	phashmap_node *element = pdict_get_element(self, key);
	return element ? element->data : 0;
}

void *pdict_remove(pdict_dictionary *self, char *key)
{
	void *data = pdict_remove_element(self, key);

	if( data != 0) {
		self->elements_count--;
	}

	return data;
}

void pdict_remove_and_destroy(pdict_dictionary *self, char *key,
                              void(*data_destroyer)(void *))
{
	void *data = pdict_remove_element(self, key);

	if( data != 0) {
		self->elements_count--;
		data_destroyer(data);
	}
}

void pdict_iterator(pdict_dictionary *self, void(*closure)(char *, void *))
{
	int table_index;

	for (table_index = 0; table_index < self->table_max_size; table_index++) {
		phashmap_node *element = self->elements[table_index];

		while (element != 0) {
			closure(element->key, element->data);
			element = element->next;
		}
	}
}

void pdict_clean(pdict_dictionary *self)
{
	internal_dictionary_clean_and_destroy_elements(self, 0);
}

void pdict_clean_and_destroy_elements(pdict_dictionary *self,
                                      void(*destroyer)(void *))
{
	internal_dictionary_clean_and_destroy_elements(self, destroyer);
}

bool pdict_has_key(pdict_dictionary *self, char *key)
{
	return pdict_get_element(self, key) != 0;
}

bool pdict_is_empty(pdict_dictionary *self)
{
	return self->elements_count == 0;
}

int pdict_size(pdict_dictionary *self)
{
	return self->elements_count;
}

void pdict_destroy(pdict_dictionary *self)
{
	pdict_clean(self);
	free(self->elements);
	free(self);
}

void pdict_destroy_and_destroy_elements(pdict_dictionary *self,
                                        void(*destroyer)(void *))
{
	pdict_clean_and_destroy_elements(self, destroyer);
	free(self->elements);
	free(self);
}

static void pdict_resize(pdict_dictionary *self, int new_max_size)
{
    phashmap_node **new_table = calloc(new_max_size,
                                            sizeof(phashmap_node *));
	phashmap_node **old_table = self->elements;
	self->table_current_size = 0;
	int table_index;

	for (table_index = 0; table_index < self->table_max_size; table_index++) {
		phashmap_node *old_element = old_table[table_index];
		phashmap_node *next_element;

		while( old_element != 0) {
			// new position
			int new_index = old_element->hashcode % new_max_size;
			phashmap_node *new_element = new_table[new_index];

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

static void internal_dictionary_clean_and_destroy_elements(
    pdict_dictionary *self,
    void(*data_destroyer)(void *))
{
	int table_index;

	for (table_index = 0; table_index < self->table_max_size; table_index++) {
		phashmap_node *element = self->elements[table_index];
		phashmap_node *next_element = 0;

		while (element != 0) {
			next_element = element->next;
			pdict_destroy_element(element, data_destroyer);
			element = next_element;
		}

		self->elements[table_index] = 0;
	}

	self->table_current_size = 0;
	self->elements_count = 0;
}

static phashmap_node *pdict_create_element(char *key,
        unsigned int key_hash,
        void *data)
{
	phashmap_node *element = malloc(sizeof(phashmap_node));
	element->key = key;
	element->data = data;
	element->hashcode = key_hash;
	element->next = 0;
	return element;
}

static phashmap_node *pdict_get_element(pdict_dictionary *self, char *key)
{
	unsigned int key_hash = pdict_hash(key, strlen(key));
	int index = key_hash % self->table_max_size;
	phashmap_node *element = self->elements[index];

	if (element == 0) {
		return 0;
	}

	do {
		if (element->hashcode == key_hash) {
			return element;
		}
	} while ((element = element->next) != 0);

	return 0;
}

static void *pdict_remove_element(pdict_dictionary *self, char *key)
{
	unsigned int key_hash = pdict_hash(key, strlen(key));
	int index = key_hash % self->table_max_size;
	phashmap_node *element = self->elements[index];

	if (element == 0) {
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
			phashmap_node *aux = element->next;
			element->next = element->next->next;
			free(aux->key);
			free(aux);
			return data;
		}

		element = element->next;
	}

	return 0;
}

static void pdict_destroy_element(phashmap_node *element,
                                  void(*data_destroyer)(void *))
{
	if (data_destroyer != 0) {
		data_destroyer(element->data);
	}

	free(element->key);
	free(element);
}

static unsigned int pdict_hash(char *key, int key_len)
{
	unsigned int hash = 0;
	int index;

	for (index = 0; index < key_len; index++) {
		unsigned char c = key[index];
		hash += c;
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}

	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}
