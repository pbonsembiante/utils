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
#ifndef _PQUEUE_H_
#define _PQUEUE_H_
/*!
 * \file pqueue.h
 * \brief Header for Queue handling lib.
 */

#include <stdbool.h>
#include <stdlib.h>

/*!
 * \typedef pqueue_queue
 * \brief Type definition for abstract Queue handler.
 *
 * __Detail:__
 *
 * Forward declaration for Queue handling. No need to expose structure internals
 * since they are implementation dependent and can change at any time.
 *
 * All functions provided know how to handle the structure properly.
 *
 */
typedef struct pqueue pqueue;

typedef void (* pqueue_destroyer)(void *);

/*!
 * \brief pqueue_create
 * \return
 */
pqueue *pqueue_create(void);

/*!
 * \brief pqueue_enqueue
 * \param self
 * \param data
 * \return
 */
size_t pqueue_enqueue(pqueue *self, void *data);

/*!
 * \brief pqueue_dequeue
 * \param self
 * \return
 */
void *pqueue_dequeue(pqueue *self);

/*!
 * \brief pqueue_peek
 * \param self
 * \return
 */
void *pqueue_peek(pqueue *self);

/*!
 * \brief pqueue_size
 * \param self
 * \return
 */
size_t pqueue_size(pqueue *self);

/*!
 * \brief pqueue_is_empty
 * \param self
 * \return
 */
bool pqueue_is_empty(pqueue *self);

/*!
 * \brief pqueue_clean
 * \param self
 */
void pqueue_clean(pqueue *self);

/*!
 * \brief pqueue_clean_destroying_data
 * \param self
 * \param destroyer
 */
void pqueue_clean_destroying_data(pqueue *self, pqueue_destroyer destroyer);

/*!
 * \brief pqueue_destroy
 * \param self
 */
void pqueue_destroy(pqueue *self);

/*!
 * \brief pqueue_destroy_all
 * \param self
 * \param destroyer
 */
void pqueue_destroy_all(pqueue *self, pqueue_destroyer destroyer);

/*
 * Handy macros
*/

#define PQUEUE_PEEK_INT(Q) *((int*) pqueue_peek(Q))
#define PQUEUE_PEEK_PINT(Q) ((int*) pqueue_peek(Q))

#define PQUEUE_PEEK_UINT(Q) *((unsigned int*) pqueue_peek(Q))
#define PQUEUE_PEEK_PUINT(Q) ((unsigned int*) pqueue_peek(Q))

#define PQUEUE_PEEK_CHAR(Q) *((char*) pqueue_peek(Q))
#define PQUEUE_PEEK_UCHAR(Q) *((unsigned char*) pqueue_peek(Q))
#define PQUEUE_PEEK_PCHAR(Q) ((char*) pqueue_peek(Q))

#define PQUEUE_PEEK_FLOAT(Q) *((float*) pqueue_peek(Q))
#define PQUEUE_PEEK_PFLOAT(Q) ((float*) pqueue_peek(Q))

#define PQUEUE_PEEK_DOUBLE(Q) *((double*) pqueue_peek(Q))
#define PQUEUE_PEEK_PDOUBLE(Q) ((double*) pqueue_peek(Q))

#endif /* _PQUEUE_H_ */
