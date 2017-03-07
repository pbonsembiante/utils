#ifndef _PQUEUE_H_
#define _PQUEUE_H_
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
typedef struct pqueue_queue pqueue_queue;

typedef void (*pqueue_destroyer)(void *);

/*!
 * \brief pqueue_create
 * \return
 */
pqueue_queue *pqueue_create(void);

/*!
 * \brief pqueue_enqueue
 * \param self
 * \param data
 * \return
 */
size_t pqueue_enqueue(pqueue_queue *self, void *data);

/*!
 * \brief pqueue_dequeue
 * \param self
 * \return
 */
void *pqueue_dequeue(pqueue_queue *self);

/*!
 * \brief pqueue_peek
 * \param self
 * \return
 */
void *pqueue_peek(pqueue_queue *self);

/*!
 * \brief pqueue_size
 * \param self
 * \return
 */
size_t pqueue_size(pqueue_queue *self);

/*!
 * \brief pqueue_is_empty
 * \param self
 * \return
 */
bool pqueue_is_empty(pqueue_queue *self);

/*!
 * \brief pqueue_clean
 * \param self
 */
void pqueue_clean(pqueue_queue *self);

/*!
 * \brief pqueue_clean_destroying_data
 * \param self
 * \param destroyer
 */
void pqueue_clean_destroying_data(pqueue_queue *self,
                                  plist_destroyer destroyer);

/*!
 * \brief pqueue_destroy
 * \param self
 */
void pqueue_destroy(pqueue_queue *self);

/*!
 * \brief pqueue_destroy_all
 * \param self
 * \param destroyer
 */
void pqueue_destroy_all(pqueue_queue *self, pqueue_destroyer destroyer);


#endif /* _PQUEUE_H_ */
