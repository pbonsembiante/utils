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

pqueue* Q = 0;

void setUp(void) {
    Q = pqueue_create();
}

void tearDown(void) {
    pqueue_destroy(Q);
}

/*
 * See: https://stackoverflow.com/a/22637665/6194674
 * */
int isFreed(void* p) {
    void* q;
    char p_addr[50];
    char q_addr[50];

    sprintf(p_addr, "%p", p);

    q = malloc(1);
    sprintf(q_addr, "%p", q);
    free(q);

    return !strncmp(q_addr, p_addr, 50);
}

void test_create_ShouldCreateAnEmptyQueue(void) {
    TEST_ASSERT_NOT_NULL(Q);
    TEST_ASSERT_TRUE(pqueue_is_empty(Q));
}

void test_create_ShouldCreateAZeroSizeQueue(void) {
    TEST_ASSERT_NOT_NULL(Q);
    TEST_ASSERT_EQUAL_UINT(0, pqueue_size(Q));
}

void test_enqueue_ShouldAppendAnElementToTheQueue(void) {
    size_t prev_size = pqueue_size(Q);
    char* test_string = calloc(11, sizeof(char));

    strncpy(test_string, "Test data!", 11);

    pqueue_enqueue(Q, test_string);

    TEST_ASSERT_EQUAL_UINT(prev_size + 1, pqueue_size(Q));

    free(test_string);
}

void test_dequeue_ShouldRemoveAnElementFromTheQueue(void) {
    char* test_string = calloc(11, sizeof(char));
    strncpy(test_string, "Test data!", 11);

    pqueue_enqueue(Q, test_string);

    size_t prev_size = pqueue_size(Q);

    pqueue_dequeue(Q);
    TEST_ASSERT_EQUAL_UINT(prev_size - 1, pqueue_size(Q));

    free(test_string);
}

void test_enqueue_ShouldEnqueueNullElements(void) {
    char* test_string = 0;
    size_t prev_size = pqueue_size(Q);

    pqueue_enqueue(Q, test_string);
    TEST_ASSERT_EQUAL_UINT(prev_size + 1, pqueue_size(Q));
}

void test_dequeue_ShouldNotDequeueFromAnEmptyQueue(void) {
    TEST_ASSERT_NULL(pqueue_dequeue(Q));
}

void test_peek_ShouldRetrieveTheNextElementFromTheQueueButNotDequeueIt(void) {
    size_t x = 99, y = 98, z = 97;

    pqueue_enqueue(Q, &x);
    pqueue_enqueue(Q, &y);
    pqueue_enqueue(Q, &z);

    TEST_ASSERT_EQUAL_UINT(x, PQUEUE_PEEK_UINT(Q));
    TEST_ASSERT_EQUAL_UINT(3, pqueue_size(Q));
}

void test_clean_ShouldEmptyTheQueue(void) {
    size_t x = 99, y = 98, z = 97;

    pqueue_enqueue(Q, &x);
    pqueue_enqueue(Q, &y);
    pqueue_enqueue(Q, &z);

    pqueue_clean(Q);

    TEST_ASSERT_TRUE(pqueue_is_empty(Q));
    TEST_ASSERT_EQUAL_UINT(0, pqueue_size(Q));
}

void test_destroyAll_ShouldDestroyTheQueueAndEveryElementInIt(void) {
    pqueue* queue = pqueue_create();
    size_t* x = calloc(1, sizeof(size_t));
    *x = 99;
    size_t* y = calloc(1, sizeof(size_t));
    *y = 99;
    size_t* z = calloc(1, sizeof(size_t));
    *z = 99;

    pqueue_enqueue(queue, x);
    pqueue_enqueue(queue, y);
    pqueue_enqueue(queue, z);

    pqueue_destroy_all(queue, free);
    TEST_ASSERT_TRUE(isFreed(queue));
}

void test_cleanDestroy_ShouldDestroyTheQueueAndEveryElementInIt(void) {
    size_t* x = calloc(1, sizeof(size_t));
    *x = 99;
    size_t* y = calloc(1, sizeof(size_t));
    *y = 99;
    size_t* z = calloc(1, sizeof(size_t));
    *z = 99;

    pqueue_enqueue(Q, x);
    pqueue_enqueue(Q, y);
    pqueue_enqueue(Q, z);

    pqueue_clean_destroying_data(Q, free);

    TEST_ASSERT_TRUE(pqueue_is_empty(Q));
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_create_ShouldCreateAnEmptyQueue);
    RUN_TEST(test_create_ShouldCreateAnEmptyQueue);
    RUN_TEST(test_create_ShouldCreateAZeroSizeQueue);

    RUN_TEST(test_dequeue_ShouldNotDequeueFromAnEmptyQueue);
    RUN_TEST(test_dequeue_ShouldRemoveAnElementFromTheQueue);

    RUN_TEST(test_enqueue_ShouldAppendAnElementToTheQueue);
    RUN_TEST(test_enqueue_ShouldEnqueueNullElements);

    RUN_TEST(test_peek_ShouldRetrieveTheNextElementFromTheQueueButNotDequeueIt);

    RUN_TEST(test_clean_ShouldEmptyTheQueue);

    RUN_TEST(test_destroyAll_ShouldDestroyTheQueueAndEveryElementInIt);

    RUN_TEST(test_cleanDestroy_ShouldDestroyTheQueueAndEveryElementInIt);

    return UNITY_END();
}
