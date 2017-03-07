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
#include <string.h>
#include <assert.h>

void test_create_ShouldCreateAnEmptyStack(void)
{
    pstack_stack *stack = 0;

    stack = pstack_create();

    assert(stack != 0);
    assert(pstack_is_empty(stack) == 1 );

    pstack_destroy(stack);
}

void test_push_ShouldPushAnElementToTheStack(void)
{
    pstack_stack *stack = pstack_create();
    size_t prev_size = pstack_size(stack);

    char *test_string = calloc(11, sizeof(char));
    strncpy(test_string, "Test data!", 11);
    pstack_push(stack, test_string);

    assert(pstack_size(stack) == prev_size + 1);

    pstack_destroy_all(stack, free);
}

void test_pop_ShouldRemoveAnElementFromThestack(void)
{
    pstack_stack *q = pstack_create();

    char *test_string = calloc(11, sizeof(char));
    char *popped = 0;
    strncpy(test_string, "Test data!", 11);

    pstack_push(q, test_string);

    size_t prev_size = pstack_size(q);
    popped = pstack_pop(q);
    assert(strcmp((const char *)popped, test_string) == 0);
    assert(pstack_size(q) == prev_size - 1);

    free(test_string);
    pstack_destroy(q);
}

void test_push_ShouldPushNullElements(void)
{
    pstack_stack *q = pstack_create();
    char *test_string = 0;

    size_t prev_size = pstack_size(q);
    pstack_push(q, test_string);

    assert(pstack_size(q) == prev_size + 1);

    pstack_destroy(q);
}

void test_pop_ShouldNotPopFromAnEmptyStack(void)
{
    pstack_stack *q = pstack_create();

    assert(pstack_pop(q) == 0);

    pstack_destroy(q);
}
