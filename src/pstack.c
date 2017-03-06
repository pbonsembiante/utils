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
#include "putils/pstack.h"

struct pstack_stack
{
    plinked_node *top;
    size_t elements_count;
};

static plinked_node *pstack_create_node(void *data);

static void pstack_link_nodes(plinked_node *previous, plinked_node *next);

pstack_stack *pstack_create()
{
    pstack_stack *stack = calloc(1, sizeof(pstack_stack));
    stack->top = 0;
    stack->elements_count = 0;
    return stack;
}

size_t pstack_push(pstack_stack *self, void *data)
{
    plinked_node *new_element = pstack_create_node(data);

    if (new_element) {
        pstack_link_nodes(new_element, self->top);
        self->top = new_element;
        self->elements_count++;
    }

    return self->elements_count;
}

void *pstack_pop(pstack_stack *self)
{
    plinked_node *tmp = 0;
    void *data = 0;

    if (self->top) {
        tmp = self->top;
        data = tmp->data;
        pstack_link_nodes(self->top, self->top->next);
        self->elements_count--;
        free(tmp);
    }

    return data;
}

void *pstack_peek(pstack_stack *self)
{
    void *data = 0;

    if (self->top) {
        data = self->top->data;
    }

    return data;
}

size_t pstack_size(pstack_stack *self)
{
    return self->elements_count;
}

bool pstack_is_empty(pstack_stack *self)
{
    return self->elements_count == 0;
}

bool pstack_destroy(pstack_stack *self)
{
    bool can_be_freed = self->elements_count == 0;

    if (can_be_freed) {
        free(self);
    }

    return can_be_freed;
}

void pstack_destroy_all(pstack_stack *self, pstack_destroyer destroyer)
{
    for ( size_t i = 0; i < pstack_size(self); ++i) {
        void *data = pstack_pop(self);

        if(destroyer) {
            destroyer(data);
        }
    }

    free(self);
}

/**
 * Private Functions
 */

static void pstack_link_nodes(plinked_node *previous, plinked_node *next)
{
    if (previous) {
        previous->next = next;
    }
}

static plinked_node *pstack_create_node(void *data)
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
