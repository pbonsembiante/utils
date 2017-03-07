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
#include "putils/pqueue.h"
#include "putils/plist.h"

struct pqueue_queue
{
    plist_list *list;
};

pqueue_queue *pqueue_create()
{
    pqueue_queue *q = calloc(1, sizeof(pqueue_queue));
    q->list = plist_create();
    return q;
}

size_t pqueue_enqueue(pqueue_queue *self, void *data)
{
    return plist_append(self->list, data);
}

void *pqueue_dequeue(pqueue_queue *self)
{
    return plist_remove(self->list, 0);
}

void *pqueue_peek(pqueue_queue *self)
{
    return plist_get(self->list, 0);
}

size_t pqueue_size(pqueue_queue *self)
{
    return plist_size(self->list);
}

bool pqueue_is_empty(pqueue_queue *self)
{
    return plist_is_empty(self->list);
}

void pqueue_clean(pqueue_queue *self)
{
    plist_clean(self->list);
}

void pqueue_clean_destroying_data(pqueue_queue *self, plist_destroyer destroyer)
{
    plist_clean_destroying_data(self->list, destroyer);
}

void pqueue_destroy(pqueue_queue *self)
{
    plist_destroy(self->list);
    free(self);
}

void pqueue_destroy_all(pqueue_queue *self, pqueue_destroyer destroyer)
{
    plist_destroy_all(self->list, destroyer);
    free(self);
}
