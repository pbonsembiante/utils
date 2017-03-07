#ifndef _PSTACK_H_
#define _PSTACK_H_
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
/*!
 * \file pstack.h
 * \brief Header for Stack handling lib.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "plist.h"

/*!
 * \typedef pstack_stack
 * \brief Type definition for abstract Stack handler.
 *
 * __Detail:__
 *
 * Forward declaration for Stack handling. No need to expose structure internals
 * since they are implementation dependent and can change at any time.
 *
 * All functions provided know how to handle the structure properly.
 *
 */

typedef struct pstack_stack pstack_stack;

/*!
 * \typedef pstack_destroyer
 * \brief
 *
 * __Detail:__
 */
typedef void (*pstack_destroyer)(void *);

/*!
 * \brief pstack_create
 * \return
 */
pstack_stack *pstack_create(void);

/*!
 * \brief pstack_push
 * \param self
 * \param data
 * \return
 */
size_t pstack_push(pstack_stack *self, void *data);

/*!
 * \brief pstack_pop
 * \param self
 * \return
 */
void *pstack_pop(pstack_stack *self);

/*!
 * \brief pstack_peek
 * \param self
 * \return
 */
void *pstack_peek(pstack_stack *self);

/*!
 * \brief pstack_size
 * \param self
 * \return
 */
size_t pstack_size(pstack_stack *self);

/*!
 * \brief pstack_is_empty
 * \param self
 * \return
 */
bool pstack_is_empty(pstack_stack *self);

/*!
 * \brief pstack_destroy
 * \param self
 * \return
 */
void pstack_destroy(pstack_stack *self);

/*!
 * \brief pqueue_destroy_all
 * \param self
 * \param destroyer
 */
void pstack_destroy_all(pstack_stack *self, pstack_destroyer destroyer);

#endif /* _PSTACK_H_ */
