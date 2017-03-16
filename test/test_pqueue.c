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
#include "unity.h"
#include <string.h>
#include <assert.h>

pqueue_queue *Q = 0;

void setUp(void)
{
    Q = pqueue_create();
}

void tearDown(void)
{
    pqueue_destroy(Q);
}

void test_create_ShouldCreateAnEmptyQueue(void)
{
    TEST_ASSERT_NOT_NULL(Q);
    TEST_ASSERT_EQUAL_UINT(1, pqueue_is_empty(Q));
}

void test_create_ShouldCreateAZeroSizeQueue(void)
{
    TEST_ASSERT_NOT_NULL(Q);
    TEST_ASSERT_EQUAL_UINT(0, pqueue_size(Q));
}

void test_enqueue_ShouldAppendAnElementToTheQueue(void)
{
    size_t prev_size = pqueue_size(Q);
    char *test_string = calloc(11, sizeof(char));

    strncpy(test_string, "Test data!", 11);

    pqueue_enqueue(Q, test_string);

    TEST_ASSERT_EQUAL_UINT(prev_size + 1, pqueue_size(Q));

    free(test_string);
}

void test_dequeue_ShouldRemoveAnElementFromTheQueue(void)
{
    char *test_string = calloc(11, sizeof(char));
    strncpy(test_string, "Test data!", 11);

    pqueue_enqueue(Q, test_string);

    size_t prev_size = pqueue_size(Q);

    pqueue_dequeue(Q);
    TEST_ASSERT_EQUAL_UINT(prev_size - 1, pqueue_size(Q) );

    free(test_string);
}

void test_enqueue_ShouldEnqueueNullElements(void)
{
    char *test_string = 0;
    size_t prev_size = pqueue_size(Q);

    pqueue_enqueue(Q, test_string);
    TEST_ASSERT_EQUAL_UINT(prev_size + 1, pqueue_size(Q));
}

void test_dequeue_ShouldNotDequeueFromAnEmptyQueue(void)
{
    TEST_ASSERT_NULL(pqueue_dequeue(Q));
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_create_ShouldCreateAnEmptyQueue);
    RUN_TEST(test_create_ShouldCreateAnEmptyQueue);
    RUN_TEST(test_dequeue_ShouldNotDequeueFromAnEmptyQueue);
    RUN_TEST(test_dequeue_ShouldRemoveAnElementFromTheQueue);
    RUN_TEST(test_enqueue_ShouldAppendAnElementToTheQueue);
    RUN_TEST(test_enqueue_ShouldEnqueueNullElements);

    return UNITY_END();
}
