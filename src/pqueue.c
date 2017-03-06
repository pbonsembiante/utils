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
#include "putils/pqueue.h"

struct pqueue_queue
{
    plinked_node *head;
    plinked_node *tail;
    size_t elements_count;
};

static plinked_node *pqueue_create_node(void *data);

pqueue_queue *pqueue_create()
{
    pqueue_queue *q = calloc(1, sizeof(pqueue_queue));
    q->head = 0;
    q->tail = 0;
    q->elements_count = 0;
    return q;
}

size_t pqueue_enqueue(pqueue_queue *self, void *data)
{
    plinked_node *new_element = pqueue_create_node(data);

    if (new_element) {
        if (self->elements_count == 0) {
            self->head = new_element;
        } else {
            self->tail->next = new_element;
        }

        self->tail = new_element;
        self->elements_count++;
    }

    return self->elements_count;
}

void *pqueue_dequeue(pqueue_queue *self)
{
    void *data = 0;
    plinked_node *tmp = self->head;

    if (self->head) {
        if (self->elements_count == 1) {
            self->tail = tmp->next;
        }

        self->head = tmp->next;
        data = tmp->data;
        free(tmp);
        self->elements_count--;
    }

    return data;
}

size_t pqueue_size(pqueue_queue *self)
{
    return self->elements_count;
}

bool pqueue_is_empty(pqueue_queue *self)
{
    return self->elements_count == 0;
}

bool pqueue_is_not_empty(pqueue_queue *self)
{
    return self->elements_count > 0;
}

bool pqueue_destroy(pqueue_queue *self)
{
    bool can_be_freed = self->elements_count == 0;

    if (can_be_freed) {
        free(self);
    }

    return can_be_freed;
}

void pqueue_destroy_all(pqueue_queue *self, pqueue_destroyer destroyer)
{
    for ( size_t i = 0; i < pqueue_size(self); ++i) {
        if(destroyer) {
            destroyer(pqueue_dequeue(self));
        }
    }

    free(self);
}

/**
 * Private Functions
 */

static plinked_node *pqueue_create_node(void *data)
{
    plinked_node *element = 0;

    if (data) {
        element = calloc(1, sizeof(plinked_node));

        if (element) {
            element->data = data;
            element->next = 0;
        }
    }

    return element;
}
