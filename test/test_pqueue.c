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
#include <string.h>
#include <assert.h>

void test_create_ShouldCreateAnEmptyQueue(void)
{
    pqueue_queue *q = 0;

    q = pqueue_create();
    assert(q != 0);
    assert(pqueue_is_empty(q) == 1 );
    pqueue_destroy(q);
}

void test_create_ShouldNotCreateAZeroSizeQueue(void)
{
    pqueue_queue *q = 0;
    size_t size = 0;
    q = pqueue_create();
    assert(q != 0);
    size = pqueue_size(q);
    assert(size == 0);

    pqueue_destroy(q);
}

void test_enqueue_ShouldAppendAnElementToTheQueue(void)
{
    pqueue_queue *q = pqueue_create();
    size_t prev_size = pqueue_size(q);

    char *test_string = calloc(11, sizeof(char));

    strncpy(test_string, "Test data!", 11);

    pqueue_enqueue(q, test_string);

    assert(pqueue_size(q) == prev_size + 1);

    pqueue_destroy_all(q, free);
}

void test_dequeue_ShouldRemoveAnElementFromTheQueue(void)
{
    pqueue_queue *q = pqueue_create();

    char *test_string = calloc(11, sizeof(char));
    strncpy(test_string, "Test data!", 11);

    pqueue_enqueue(q, test_string);

    size_t prev_size = pqueue_size(q);

    pqueue_dequeue(q);
    assert(pqueue_size(q) == prev_size - 1);

    free(test_string);
    pqueue_destroy(q);
}

void test_enqueue_ShouldEnqueueNullElements(void)
{
    pqueue_queue *q = pqueue_create();
    char *test_string = 0;

    size_t prev_size = pqueue_size(q);
    pqueue_enqueue(q, test_string);

    assert(pqueue_size(q) == prev_size + 1);

    pqueue_destroy(q);
}

void test_dequeue_ShouldNotDequeueFromAnEmptyQueue(void)
{
    pqueue_queue *q = pqueue_create();

    assert(pqueue_dequeue(q) == 0);

    pqueue_destroy(q);
}
