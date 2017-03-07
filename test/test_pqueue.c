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
