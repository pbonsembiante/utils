#include "putils/plist.h"
#include "unity.h"

plist_list *L = 0;

void setUp(void)
{
    L = plist_create();
    return;
}

void tearDown(void)
{
    plist_destroy(L);
    return;
}

bool comp(const void *a, const void *b)
{
    const int *_a = a;
    const int *_b = b;

    return _a < _b;
}

/*
void f(void)
{
    plist_all_match();
    plist_any_match();
    plist_append();
    plist_clean();
    plist_clean_destroying_data();
    plist_count_matching();
    plist_destroy();
    plist_destroy_all();
    plist_filter();
    plist_find();
    plist_get();
    plist_get_elements();
    plist_get_removing_elements();
    plist_is_empty();
    plist_iterate();
    plist_map();
    plist_merge();
    plist_prepend();
    plist_remove();
    plist_remove_and_destroy();
    plist_remove_destroying_selected();
    plist_remove_selected();
    plist_replace();
    plist_replace_and_destroy();
    plist_size();
    plist_sort();
}
*/

void test_createNewList_ShouldBeEmptyOnCreationTime(void)
{
    TEST_ASSERT_EQUAL_UINT(plist_size(L), 0);
    return;
}

void test_createNewList_ShouldNotBeNull(void)
{
    TEST_ASSERT_NOT_NULL(L);
    return;
}

void test_size_SizeOfNewListWithOneItemShouldBeOne(void)
{
    int *data = calloc(1, sizeof(int));
    *data = 13;

    TEST_ASSERT_NOT_NULL(L);

    plist_append(L, data);

    TEST_ASSERT_EQUAL_UINT(plist_size(L), 1);

    free(data);
}

void test_sort_ShouldOrderShuffledList(void)
{
    int *data = calloc(10, sizeof(int));

    for (size_t i = 0; i < 10; ++i) {
        data[i] = i+1;
    }

    plist_append(L, &data[2]);
    plist_append(L, &data[6]);
    plist_append(L, &data[4]);
    plist_append(L, &data[3]);
    plist_append(L, &data[1]);
    plist_append(L, &data[0]);
    plist_append(L, &data[5]);
    plist_append(L, &data[9]);
    plist_append(L, &data[8]);
    plist_append(L, &data[7]);


    plist_sort(L, comp);

    for (size_t i = 0; i < plist_size(L); ++i ) {
        TEST_ASSERT_EQUAL_INT( i+1, *((int*)plist_get(L, i)));
    }

    free(data);
}

void test_add_ShouldAddANewElement(void)
{
    int *data = calloc(1, sizeof(int)), prev = 0;
    *data = 13;

    prev = plist_size(L);
    plist_add(L, 0, data);

    TEST_ASSERT_EQUAL_INT(prev + 1, plist_size(L));

    free(data);
}


void test_add_ShouldAddANewElementAndCheckIt(void)
{
    int *in = calloc(1, sizeof(int)), *out;
    *in = 13;

    plist_add(L, 0, in);
    out = plist_get(L, 0);

    TEST_ASSERT_EQUAL_PTR(in, out);
    TEST_ASSERT_EQUAL_INT(*in, *out);

    free(in);
}

void test_add_ShouldAddANewElementInTheMiddleOfAList(void)
{
    int *in = calloc(3, sizeof(int)), *out;
    in[0] = 1;
    in[1] = 2;
    in[2] = 3;

    plist_append(L, &in[0]);
    plist_append(L, &in[2]);

    plist_add(L, 1, &in[1]);

    out = plist_get(L, 1);

    TEST_ASSERT_EQUAL_PTR(&in[1], out);
    TEST_ASSERT_EQUAL_INT(in[1], *out);

    free(in);
}

void test_add_ShouldNotAddANewElementOutOfIndexInAnEmptyList(void)
{
    int *in = calloc(1, sizeof(int));
    *in = 1;

    plist_add(L, 1, in);

    TEST_ASSERT_EQUAL_UINT(0, plist_size(L));

    free(in);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_createNewList_ShouldBeEmptyOnCreationTime);
    RUN_TEST(test_createNewList_ShouldNotBeNull);
    RUN_TEST(test_size_SizeOfNewListWithOneItemShouldBeOne);
    RUN_TEST(test_sort_ShouldOrderShuffledList);
    RUN_TEST(test_add_ShouldAddANewElement);
    RUN_TEST(test_add_ShouldAddANewElementAndCheckIt);
    RUN_TEST(test_add_ShouldAddANewElementInTheMiddleOfAList);
    RUN_TEST(test_add_ShouldNotAddANewElementOutOfIndexInAnEmptyList);

    return UNITY_END();
}
