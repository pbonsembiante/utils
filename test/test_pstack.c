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
    strncpy(test_string, "Test data!", 11);

    pstack_push(q, test_string);

    size_t prev_size = pstack_size(q);

    assert(strcmp((const char *)pstack_pop(q), test_string) == 0);
    assert(pstack_size(q) == prev_size - 1);

    free(test_string);
    pstack_destroy(q);
}

void test_push_ShouldNotPushNullElements(void)
{
    pstack_stack *q = pstack_create();
    char *test_string = 0;

    size_t prev_size = pstack_size(q);
    pstack_push(q, test_string);

    assert(pstack_size(q) == prev_size);

    pstack_destroy(q);
}

void test_pop_ShouldNotPopFromAnEmptyStack(void)
{
    pstack_stack *q = pstack_create();

    assert(pstack_pop(q) == 0);

    pstack_destroy(q);
}

int main(void)
{
    test_create_ShouldCreateAnEmptyStack();
    test_pop_ShouldNotPopFromAnEmptyStack();
    test_pop_ShouldRemoveAnElementFromThestack();
    test_push_ShouldPushAnElementToTheStack();
    test_push_ShouldNotPushNullElements();

    return 0;
}
