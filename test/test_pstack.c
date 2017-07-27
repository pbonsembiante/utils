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

pstack_stack *S = 0;

void setUp(void)
{
    S = pstack_create();
}

void tearDown(void)
{
    pstack_destroy(S);
}

void test_create_ShouldCreateAnEmptyStack(void)
{
    TEST_ASSERT_NOT_NULL(S);
    TEST_ASSERT_EQUAL_UINT(true, pstack_is_empty(S));
}

void test_push_ShouldPushAnElementToTheStack(void)
{
    size_t prev_size = pstack_size(S);
    char *test_string = calloc(11, sizeof(char));

    strncpy(test_string, "Test data!", 11);
    pstack_push(S, test_string);

    TEST_ASSERT_EQUAL_UINT(prev_size + 1, pstack_size(S));

    free(test_string);
}

void test_pop_ShouldRemoveAnElementFromTheStack(void)
{
    char *test_string = calloc(11, sizeof(char));
    char *popped = 0;
    size_t prev_size = 0;

    strncpy(test_string, "Test data!", 11);
    pstack_push(S, test_string);
    prev_size = pstack_size(S);

    popped = pstack_pop(S);
    TEST_ASSERT_EQUAL_STRING((const char *)popped, test_string);
    TEST_ASSERT_EQUAL_UINT(prev_size - 1, pstack_size(S));

    free(test_string);
}

void test_push_ShouldPushNullElements(void)
{
    char *test_string = 0;
    size_t prev_size = pstack_size(S);

    pstack_push(S, test_string);

    TEST_ASSERT_NULL(test_string);
    TEST_ASSERT_EQUAL_UINT(prev_size + 1, pstack_size(S));
}

void test_pop_ShouldNotPopFromAnEmptyStack(void)
{
    TEST_ASSERT_NULL(pstack_pop(S));
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_create_ShouldCreateAnEmptyStack);
    RUN_TEST(test_pop_ShouldNotPopFromAnEmptyStack);
    RUN_TEST(test_pop_ShouldRemoveAnElementFromTheStack);
    RUN_TEST(test_push_ShouldPushAnElementToTheStack);
    RUN_TEST(test_push_ShouldPushNullElements);

    return UNITY_END();
}
