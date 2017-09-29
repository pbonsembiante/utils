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

#include "putils/pstack.h"
#include "unity.h"
#include <string.h>

pstack* S = 0;

void setUp(void) {
    S = pstack_create();
}

void tearDown(void) {
    pstack_destroy(S);
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

void test_create_ShouldCreateAnEmptyStack(void) {
    TEST_ASSERT_NOT_NULL(S);
    TEST_ASSERT_EQUAL_UINT(true, pstack_is_empty(S));
}

void test_push_ShouldPushAnElementToTheStack(void) {
    size_t prev_size = pstack_size(S);
    char* test_string = calloc(11, sizeof(char));

    strncpy(test_string, "Test data!", 11);
    pstack_push(S, test_string);

    TEST_ASSERT_EQUAL_UINT(prev_size + 1, pstack_size(S));

    free(test_string);
}

void test_pop_ShouldRemoveAnElementFromTheStack(void) {
    char* test_string = calloc(11, sizeof(char));
    char* popped = 0;
    size_t prev_size = 0;

    strncpy(test_string, "Test data!", 11);
    pstack_push(S, test_string);
    prev_size = pstack_size(S);

    popped = pstack_pop(S);
    TEST_ASSERT_EQUAL_STRING((const char*) popped, test_string);
    TEST_ASSERT_EQUAL_UINT(prev_size - 1, pstack_size(S));

    free(test_string);
}

void test_push_ShouldPushNullElements(void) {
    char* test_string = 0;
    size_t prev_size = pstack_size(S);

    pstack_push(S, test_string);

    TEST_ASSERT_NULL(test_string);
    TEST_ASSERT_EQUAL_UINT(prev_size + 1, pstack_size(S));
}

void test_pop_ShouldNotPopFromAnEmptyStack(void) {
    TEST_ASSERT_NULL(pstack_pop(S));
}

void test_destroyAll_ShouldDestroyTheStackAndEveryElementInIt(void) {
    pstack* stack = pstack_create();
    size_t* x = calloc(1, sizeof(size_t));
    *x = 99;
    size_t* y = calloc(1, sizeof(size_t));
    *y = 98;
    size_t* z = calloc(1, sizeof(size_t));
    *z = 97;

    pstack_push(stack, x);
    pstack_push(stack, y);
    pstack_push(stack, z);

    pstack_destroy_all(stack, free);
    TEST_ASSERT_TRUE(isFreed(stack));
}

void test_peek_ShouldRetrieveTheTopElementFromTheStackButNotPopIt(void) {
    size_t x = 99, y = 98, z = 97;

    pstack_push(S, &x);
    pstack_push(S, &y);
    pstack_push(S, &z);

    TEST_ASSERT_EQUAL_UINT(z, PSTACK_PEEK_UINT(S));
    TEST_ASSERT_EQUAL_UINT(3, pstack_size(S));
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_create_ShouldCreateAnEmptyStack);

    RUN_TEST(test_pop_ShouldNotPopFromAnEmptyStack);
    RUN_TEST(test_pop_ShouldRemoveAnElementFromTheStack);

    RUN_TEST(test_push_ShouldPushAnElementToTheStack);
    RUN_TEST(test_push_ShouldPushNullElements);

    RUN_TEST(test_peek_ShouldRetrieveTheTopElementFromTheStackButNotPopIt);

    RUN_TEST(test_destroyAll_ShouldDestroyTheStackAndEveryElementInIt);

    return UNITY_END();
}
