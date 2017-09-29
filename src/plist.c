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
#include <stdlib.h>
#include <stdbool.h>
#include "putils/plist.h"

struct plist {
    plinked_node* head;
    plinked_node* tail;
    size_t elements_count;
};

static void plist_link_nodes(plinked_node* previous, plinked_node* next);

static plinked_node* plist_create_node(plist_member_t*);

static plinked_node* plist_get_node(plist* self, size_t index);

static plinked_node*
plist_find_node(plist* self, plist_evaluator condition, size_t* index);

static void
plist_merge_sort(plinked_node** headReference, plist_comparator comparator);

static plinked_node* plist_sorted_merge(plinked_node* self, plinked_node* other,
                                        plist_comparator comparator);

static void
plist_front_back_split(plinked_node* source, plinked_node** frontRef,
                       plinked_node** backRef);

plist* plist_create(void) {
    plist* list = calloc(1, sizeof(plist));
    list->head = 0;
    list->tail = 0;
    list->elements_count = 0;
    return list;
}

size_t plist_append(plist* self, plist_member_t* data) {
    plinked_node* new_element = plist_create_node(data);
    plinked_node* last = self->tail;

    if (self->elements_count == 0) {
        self->head = new_element;
    } else {
        plist_link_nodes(last, new_element);
    }

    self->tail = new_element;
    self->elements_count++;
    return self->elements_count;
}

void plist_merge(plist* self, plist* other) {
    plinked_node* node = other->head;

    while (node) {
        plist_append(self, node->data);
        node = node->next;
    }
}

void* plist_get(plist* self, size_t index) {
    plinked_node* element = plist_get_node(self, index);
    return element ? element->data : 0;
}

void plist_add(plist* self, size_t index, plist_member_t* data) {
    plinked_node* new_element = 0;

    if (self->elements_count >= index) {
        new_element = plist_create_node(data);

        if (index == 0) {
            plist_link_nodes(new_element, self->head);
            self->head = new_element;

            if (self->elements_count == 0) {
                self->tail = new_element;
            }
        } else {
            plinked_node* previous = 0;
            plinked_node* next = 0;
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

void* plist_replace(plist* self, size_t index, plist_member_t* data) {
    void* old_data = 0;
    plinked_node* element = plist_get_node(self, index);

    if (element) {
        old_data = element->data;
        element->data = data;
    }

    return old_data;
}

void plist_replace_and_destroy(plist* self, size_t index, plist_member_t* data,
                               plist_destroyer destroyer) {
    void* old_data = plist_replace(self, index, data);
    if (old_data && destroyer) {
        destroyer(old_data);
    }
}

void* plist_find(plist* self, plist_evaluator condition, size_t* index) {
    plinked_node* element = plist_find_node(self, condition, index);
    return element ? element->data : 0;
}

void plist_iterate(plist* self, plist_closure closure) {
    plinked_node* element = self->head;

    while (element) {
        closure(element->data);
        element = element->next;
    }
}

void* plist_remove(plist* self, size_t index) {
    void* data = 0;
    plinked_node* aux = 0;

    if (plist_is_empty(self)) {
        return 0;
    }

    if (index == 0) {
        aux = self->head;
        data = aux->data;
        self->head = aux->next;

        if (!self->head) {
            self->tail = 0;
        }
    } else {
        plinked_node* previous = 0;
        previous = plist_get_node(self, index - 1);

        if (!previous || !previous->next) {
            return 0;
        }

        aux = previous->next;
        data = aux->data;
        plist_link_nodes(previous, aux->next);

        if (aux->next == 0) {
            self->tail = previous;
        }
    }

    self->elements_count--;
    free(aux);

    return data;
}

plist_member_t* plist_remove_selected(plist* self, plist_evaluator condition) {
    size_t index = 0;
    plinked_node* element = plist_find_node(self, condition, &index);
    return element ? plist_remove(self, index) : 0;
}

void
plist_remove_and_destroy(plist* self, size_t index, plist_destroyer destroyer) {
    void* data = plist_remove(self, index);
    if (data && destroyer) {
        destroyer(data);
    }
}

void plist_remove_destroying_selected(plist* self, plist_evaluator condition,
                                      plist_destroyer destroyer) {
    void* data = plist_remove_selected(self, condition);

    if (data && destroyer) {
        destroyer(data);
    }
}

size_t plist_size(plist* self) {
    return self->elements_count;
}

bool plist_is_empty(plist* self) {
    return plist_size(self) == 0;
}

void plist_clean(plist* self) {
    if (!self) {
        return;
    }

    while (self->head) {
        plinked_node* element;
        element = self->head;
        self->head = self->head->next;
        free(element);
    }

    self->tail = self->head;
    self->elements_count = 0;
}

void plist_clean_destroying_data(plist* self, plist_destroyer destroyer) {
    plist_iterate(self, destroyer);
    plist_clean(self);
}

void plist_destroy(plist* self) {
    plist_clean(self);
    free(self);
}

void plist_destroy_all(plist* self, plist_destroyer destroyer) {
    plist_clean_destroying_data(self, destroyer);
    free(self);
}

plist* plist_get_elements(plist* self, size_t count) {
    plist* sublist = plist_create();

    if (!plist_is_empty(self)) {
        count = count > plist_size(self) ? plist_size(self) : count;
        for (size_t i = 0; i < count; ++i) {
            void* data = plist_get(self, i);
            plist_append(sublist, data);
        }
    }

    return sublist;
}

plist* plist_get_removing_elements(plist* self, size_t count) {
    plist* sublist = plist_create();

    if (!plist_is_empty(self)) {
        count = count > plist_size(self) ? plist_size(self) : count;
        for (size_t i = 0; i < count; ++i) {
            void* element = plist_remove(self, 0);
            plist_append(sublist, element);
        }
    }
    return sublist;
}

plist* plist_filter(plist* self, plist_evaluator condition) {
    plist* filtered = plist_create();
    plinked_node* element = self->head;

    while (element) {
        if (condition(element->data)) {
            plist_append(filtered, element->data);
        }

        element = element->next;
    }

    return filtered;
}

plist* plist_map(plist* self, plist_transformer transformer) {
    plist* mapped = plist_create();
    plinked_node* element = self->head;

    while (element) {
        plist_append(mapped, transformer(element->data));
        element = element->next;
    }

    return mapped;
}

void plist_sort(plist* self, plist_comparator comparator) {
    plist_merge_sort(&self->head, comparator);
    if (self->head != 0 && self->head->next != 0) {
        self->tail = plist_get_node(self, self->elements_count - 2)->next;
    }
}

size_t plist_count_matching(plist* self, plist_evaluator condition) {
    plist* satisfying = plist_filter(self, condition);
    size_t result = satisfying->elements_count;
    plist_destroy(satisfying);
    return result;
}

bool plist_any_match(plist* self, plist_evaluator condition) {
    return plist_count_matching(self, condition) > 0;
}

bool plist_all_match(plist* self, plist_evaluator condition) {
    return plist_count_matching(self, condition) == self->elements_count;
}

size_t plist_prepend(plist* self, plist_member_t* data) {
    plinked_node* new_element = plist_create_node(data);
    new_element->next = self->head;
    self->head = new_element;

    if (self->elements_count == 0) {
        self->tail = new_element;
    }

    self->elements_count++;
    return self->elements_count;
}

/********* PRIVATE FUNCTIONS **************/

static void plist_link_nodes(plinked_node* previous, plinked_node* next) {
    if (previous) {
        previous->next = next;
    }
}

static plinked_node* plist_create_node(plist_member_t* data) {
    plinked_node* element = calloc(1, sizeof(plinked_node));

    if (element) {
        element->data = data;
        element->next = 0;
    }

    return element;
}

static plinked_node* plist_get_node(plist* self, size_t index) {
    plinked_node* element = 0;
    bool is_in_range = self->elements_count > index;

    if (is_in_range && (self->elements_count - 1) == index) {
        element = self->tail;
    }

    if (is_in_range && !element) {
        size_t count = 0;
        element = self->head;

        while (count++ < index) {
            element = element->next;
        }
    }

    return element;
}

static plinked_node*
plist_find_node(plist* self, plist_evaluator condition, size_t* index) {
    plinked_node* element = self->head;
    size_t position = 0;

    if (!element || self->elements_count == 0 || !condition) {
        return 0;
    }

    while (!condition(element->data)) {
        element = element->next;
        position++;
    }

    if (index) {
        *index = position;
    }

    return element;
}

static void
plist_merge_sort(plinked_node** headReference, plist_comparator comparator) {
    plinked_node* head = *headReference;
    plinked_node* a;
    plinked_node* b;

    if (head == 0 || head->next == 0) {
        return;
    }

    plist_front_back_split(head, &a, &b);
    plist_merge_sort(&a, comparator);
    plist_merge_sort(&b, comparator);
    *headReference = plist_sorted_merge(a, b, comparator);
}

static plinked_node* plist_sorted_merge(plinked_node* self, plinked_node* other,
                                        plist_comparator comparator) {
    plinked_node* result = 0;

    /* Base cases */
    if (!self || !other) {
        return !self ? other : self;
    }

    if (comparator(self->data, other->data)) {
        result = self;
        result->next = plist_sorted_merge(self->next, other, comparator);
    } else {
        result = other;
        result->next = plist_sorted_merge(self, other->next, comparator);
    }

    return (result);
}

static void
plist_front_back_split(plinked_node* source, plinked_node** frontRef,
                       plinked_node** backRef) {
    plinked_node* fast;
    plinked_node* slow;

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
