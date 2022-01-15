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

#ifndef _PSTACK_H_
#define _PSTACK_H_
/*!
 * \file pstack.h
 * \brief Header for Stack handling lib.
 */

#include <stdbool.h>
#include <stdlib.h>

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

typedef struct pstack pstack;

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
pstack *pstack_create(void);

/*!
 * \brief pstack_push
 * \param self
 * \param data
 * \return
 */
size_t pstack_push(pstack *self, void *data);

/*!
 * \brief pstack_pop
 * \param self
 * \return
 */
void *pstack_pop(pstack *self);

/*!
 * \brief pstack_peek
 * \param self
 * \return
 */
void *pstack_peek(pstack *self);

/*!
 * \brief pstack_size
 * \param self
 * \return
 */
size_t pstack_size(pstack *self);

/*!
 * \brief pstack_is_empty
 * \param self
 * \return
 */
bool pstack_is_empty(pstack *self);

/*!
 * \brief pstack_destroy
 * \param self
 * \return
 */
void pstack_destroy(pstack *self);

/*!
 * \brief pqueue_destroy_all
 * \param self
 * \param destroyer
 */
void pstack_destroy_all(pstack *self, pstack_destroyer destroyer);

/*
 * Handy macros
 */
#define PSTACK_PEEK_INT(Q) *((int *)pstack_peek(Q))
#define PSTACK_PEEK_PINT(Q) ((int *)pstack_peek(Q))

#define PSTACK_PEEK_UINT(Q) *((unsigned int *)pstack_peek(Q))
#define PSTACK_PEEK_PUINT(Q) ((unsigned int *)pstack_peek(Q))

#define PSTACK_PEEK_CHAR(Q) *((char *)pstack_peek(Q))
#define PSTACK_PEEK_UCHAR(Q) *((unsigned char *)pstack_peek(Q))
#define PSTACK_PEEK_PCHAR(Q) ((char *)pstack_peek(Q))

#define PSTACK_PEEK_FLOAT(Q) *((float *)pstack_peek(Q))
#define PSTACK_PEEK_PFLOAT(Q) ((float *)pstack_peek(Q))

#define PSTACK_PEEK_DOUBLE(Q) *((double *)pstack_peek(Q))
#define PSTACK_PEEK_PDOUBLE(Q) ((double *)pstack_peek(Q))

#endif /* _PSTACK_H_ */
