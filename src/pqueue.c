/***************************************************************************
 * Copyright (C) 2016 - 2020 Patricio Bonsembiante. All rights reserved.
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

struct pqueue {
    plist *list;
};

pqueue *pqueue_create() {
    pqueue *q = calloc(1, sizeof(pqueue));
    q->list = plist_create();
    return q;
}

size_t pqueue_enqueue(pqueue *self, void *data) {
    return plist_append(self->list, data);
}

void *pqueue_dequeue(pqueue *self) {
    return plist_remove(self->list, 0);
}

void *pqueue_peek(pqueue *self) {
    return plist_get(self->list, 0);
}

size_t pqueue_size(pqueue *self) {
    return plist_size(self->list);
}

bool pqueue_is_empty(pqueue *self) {
    return plist_is_empty(self->list);
}

void pqueue_clean(pqueue *self) {
    plist_clean(self->list);
}

void pqueue_clean_destroying_data(pqueue *self, plist_destroyer destroyer) {
    plist_clean_destroying_data(self->list, destroyer);
}

void pqueue_destroy(pqueue *self) {
    plist_destroy(self->list);
    free(self);
}

void pqueue_destroy_all(pqueue *self, pqueue_destroyer destroyer) {
    plist_destroy_all(self->list, destroyer);
    free(self);
}
