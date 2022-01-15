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
#include "putils/pstack.h"
#include "putils/plist.h"

struct pstack {
  plist *list;
};

pstack *pstack_create() {
  pstack *stack = calloc(1, sizeof(pstack));
  stack->list = plist_create();

  return stack;
}

size_t pstack_push(pstack *self, void *data) {
  return plist_prepend(self->list, data);
}

void *pstack_pop(pstack *self) { return plist_remove(self->list, 0); }

void *pstack_peek(pstack *self) { return plist_get(self->list, 0); }

size_t pstack_size(pstack *self) { return plist_size(self->list); }

bool pstack_is_empty(pstack *self) { return plist_size(self->list) == 0; }

void pstack_destroy(pstack **self) {
  plist_destroy(&(*self)->list);
  if (*self) free(*self);
  *self = 0;
}

void pstack_destroy_all(pstack **self, pstack_destroyer destroyer) {
  plist_destroy_all(&(*self)->list, destroyer);
  if (*self) free(*self);
  *self = 0;
}
