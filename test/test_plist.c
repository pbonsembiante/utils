#include "putils/plist.h"
#include "unity.h"

#define DATA_ARRAY_LEN 10

plist *L = 0;
size_t *data = 0;

/*
void f(void)
{
    plist_clean_destroying_data();
    plist_destroy();
    plist_destroy_all();
    plist_find();
    plist_get_elements();
    plist_get_removing_elements();
    plist_is_empty();
    plist_iterate();
    plist_prepend();
    plist_remove();
    plist_remove_and_destroy();
    plist_remove_destroying_selected();
    plist_remove_selected();
    plist_replace();
    plist_replace_and_destroy();
    plist_size();
*/
void *mapper(void *_orig);

bool comp(const void *a, const void *b);

bool isEven(void *_val);

void setUp(void);

void tearDown(void);

void test_create_NewListShouldBeEmpty(void);

void test_create_NewListShouldNotBeNull(void);

void test_size_SizeOfNewListWithOneItemShouldBeOne(void);

void test_sort_ShouldOrderShuffledList(void);

void test_add_ShouldAddANewElement(void);

void test_sort_ShouldOrderAOneItemList(void);

void test_sort_ShouldHandleAnEmptyList(void);

void test_add_ShouldAddANewElementAndCheckIt(void);

void test_add_ShouldAddANewElementInTheMiddleOfAList(void);

void test_add_ShouldAddANewElementAtTheEndOfAList(void);

void test_add_ShouldNotAddANewElementOutOfIndexBoundFromAnEmptyList(void);

void test_map_ShouldReturnATransformedList(void);

void test_append_ShouldAddAnElementToTheEndOfList(void);

void test_append_ShouldAddAnElementToAnEmptyList(void);

void test_clean_ShouldEmptyALoadedList(void);

void test_map_ShouldMapAnEmptyListAndReturnANewValidEmptyList(void);

void test_filter_ShouldFilterOutAllEvenNumbersFromList(void);

void test_filter_ShouldFilterAnEmptyListAndReturnAValidEmptyList(void);

void test_filter_ShouldFilterAListAndMatchAllItems(void);

void test_filter_ShoildFilterAListAndMatchNoneAndReturnAnEmptyList(void);

void test_allMatch_ShouldFilterAListAndAllElementsShouldMatch(void);

void test_allMatch_ShouldFilterAlistAndNotAllTheElementsWillMatch(void);

void test_anyMatch_ShouldFilterAListAndAtLeastOneShouldMatch(void);

void test_anyMatch_ShouldFilterAListAndNoElementShouldMatch(void);

void test_countMatch_ShouldCountAllEvenNumbersAndShouldBeGreaterThanZero(void);

void test_countMatch_ShouldCountEvenNumbersAndResultShouldBeZero(void);

void test_countMatch_ShouldCountEvenNumbersAndResultShouldBeHalfListSize(void);

void test_size_ShouldBeZeroForANewList(void);

void test_size_ShouldBeEqualToTheNumberOfAddedElements(void);

void test_size_ShouldBeZeroForACleanList(void);

void test_size_ShouldNotBeZeroAfterAddingElements(void);

void test_get_ShouldGetAValidElementFromTheList(void);

void test_get_ShouldGetAllElementsFromTheList(void);

void test_get_ShouldGetTheTailElementFromTheList(void);

void test_merge_ShouldMergeTwoLists(void);

void test_merge_ShouldMergeShouldHandleAnEmptyListAsSecondParameter(void);

void test_merge_ShouldMergeShouldHandleAnEmptyListAsFirstParameter(void);

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_create_NewListShouldBeEmpty);
    RUN_TEST(test_create_NewListShouldNotBeNull);

    RUN_TEST(test_size_SizeOfNewListWithOneItemShouldBeOne);
    RUN_TEST(test_size_ShouldBeZeroForANewList);
    RUN_TEST(test_size_ShouldBeEqualToTheNumberOfAddedElements);
    RUN_TEST(test_size_ShouldBeZeroForACleanList);
    RUN_TEST(test_size_ShouldNotBeZeroAfterAddingElements);

    RUN_TEST(test_sort_ShouldOrderShuffledList);
    RUN_TEST(test_sort_ShouldOrderAOneItemList);
    RUN_TEST(test_sort_ShouldHandleAnEmptyList);

    RUN_TEST(test_add_ShouldAddANewElement);
    RUN_TEST(test_add_ShouldAddANewElementAndCheckIt);
    RUN_TEST(test_add_ShouldAddANewElementInTheMiddleOfAList);
    RUN_TEST(test_add_ShouldNotAddANewElementOutOfIndexBoundFromAnEmptyList);
    RUN_TEST(test_add_ShouldAddANewElementAtTheEndOfAList);

    RUN_TEST(test_map_ShouldReturnATransformedList);
    RUN_TEST(test_map_ShouldMapAnEmptyListAndReturnANewValidEmptyList);

    RUN_TEST(test_append_ShouldAddAnElementToTheEndOfList);
    RUN_TEST(test_append_ShouldAddAnElementToAnEmptyList);

    RUN_TEST(test_clean_ShouldEmptyALoadedList);

    RUN_TEST(test_filter_ShoildFilterAListAndMatchNoneAndReturnAnEmptyList);
    RUN_TEST(test_filter_ShouldFilterAListAndMatchAllItems);
    RUN_TEST(test_filter_ShouldFilterAnEmptyListAndReturnAValidEmptyList);
    RUN_TEST(test_filter_ShouldFilterOutAllEvenNumbersFromList);

    RUN_TEST(test_allMatch_ShouldFilterAListAndAllElementsShouldMatch);
    RUN_TEST(test_allMatch_ShouldFilterAlistAndNotAllTheElementsWillMatch);

    RUN_TEST(test_anyMatch_ShouldFilterAListAndAtLeastOneShouldMatch);
    RUN_TEST(test_anyMatch_ShouldFilterAListAndNoElementShouldMatch);

    RUN_TEST(test_countMatch_ShouldCountAllEvenNumbersAndShouldBeGreaterThanZero);
    RUN_TEST(test_countMatch_ShouldCountEvenNumbersAndResultShouldBeHalfListSize);
    RUN_TEST(test_countMatch_ShouldCountEvenNumbersAndResultShouldBeZero);

    RUN_TEST(test_get_ShouldGetAValidElementFromTheList);
    RUN_TEST(test_get_ShouldGetAllElementsFromTheList);
    RUN_TEST(test_get_ShouldGetTheTailElementFromTheList);

    RUN_TEST(test_merge_ShouldMergeTwoLists);
    RUN_TEST(test_merge_ShouldMergeShouldHandleAnEmptyListAsSecondParameter);
    RUN_TEST(test_merge_ShouldMergeShouldHandleAnEmptyListAsFirstParameter);

    return UNITY_END();
}

void setUp(void) {
    data = calloc(DATA_ARRAY_LEN, sizeof(size_t));
    L = plist_create();
}

void tearDown(void) {
    free(data);
    plist_destroy(L);
}

bool comp(const void *a, const void *b) {
    const int *_a = a;
    const int *_b = b;
    return _a < _b;
}

void *mapper(void *_orig) {
    const int *orig = _orig;
    int *mapped = calloc(1, sizeof(int));
    *mapped = *orig * 2;
    return mapped;
}

bool isEven(void *_val) {
    const int *number = _val;
    return (*number % 2) == 0;
}

void test_create_NewListShouldBeEmpty(void) {
    TEST_ASSERT_TRUE(plist_is_empty(L));
}

void test_create_NewListShouldNotBeNull(void) {
    TEST_ASSERT_NOT_NULL(L);
}

void test_size_SizeOfNewListWithOneItemShouldBeOne(void) {
    data[0] = 13;
    plist_append(L, &data[0]);
    TEST_ASSERT_EQUAL_UINT(plist_size(L), 1);
}

void test_size_ShouldBeZeroForANewList(void) {
    TEST_ASSERT_EQUAL_UINT(plist_size(L), 0);
}

void test_size_ShouldBeEqualToTheNumberOfAddedElements(void) {
    for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
        data[i] = i + 1;
        plist_append(L, &data[i]);
    }

    TEST_ASSERT_EQUAL_UINT(plist_size(L), DATA_ARRAY_LEN);
}

void test_size_ShouldBeZeroForACleanList(void) {
    for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
        data[i] = i + 1;
        plist_append(L, &data[i]);
    }

    plist_clean(L);
    TEST_ASSERT_EQUAL_UINT(plist_size(L), 0);
}

void test_size_ShouldNotBeZeroAfterAddingElements(void) {
    for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
        data[i] = i + 1;
        plist_append(L, &data[i]);
    }

    TEST_ASSERT_TRUE(plist_size(L) != 0);
}

void test_sort_ShouldOrderAOneItemList(void) {
    size_t x = 99;

    plist_append(L, &x);
    plist_sort(L, comp);

    TEST_ASSERT_EQUAL_UINT(99, PLIST_GET_UINT(L, 0));
}

void test_sort_ShouldHandleAnEmptyList(void) {
    plist_sort(L, comp);
    TEST_ASSERT_TRUE(L);
    TEST_ASSERT_EQUAL_UINT(0, plist_size(L));
}

void test_sort_ShouldOrderShuffledList(void) {
    for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
        data[i] = i + 1;
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
    size_t len = plist_size(L);

    for (size_t i = 0; i < len; ++i) {
        TEST_ASSERT_EQUAL_UINT(i + 1, PLIST_GET_UINT(L, i));
    }
}

void test_add_ShouldAddANewElement(void) {
    data[0] = 13;

    if (plist_is_empty(L)) {
        plist_add(L, 0, &data[0]);
        TEST_ASSERT_EQUAL_UINT(1, plist_size(L));
    } else {
        TEST_FAIL();
    }
}

void test_add_ShouldAddANewElementAndCheckIt(void) {
    for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
        data[i] = i + 1;
    }

    plist_append(L, data);
    int *out = plist_get(L, 0);
    TEST_ASSERT_EQUAL_PTR(data, out);
    TEST_ASSERT_EQUAL_UINT_ARRAY(data, out, DATA_ARRAY_LEN);
}

void test_add_ShouldAddANewElementInTheMiddleOfAList(void) {
    for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
        data[i] = i + 1;
        plist_append(L, &data[i]);
    }

    data[DATA_ARRAY_LEN / 2] = 99;
    plist_add(L, DATA_ARRAY_LEN / 2, &data[DATA_ARRAY_LEN / 2]);
    int out = PLIST_GET_UINT(L, DATA_ARRAY_LEN / 2);
    TEST_ASSERT_EQUAL_UINT(data[DATA_ARRAY_LEN / 2], out);
}

void test_add_ShouldNotAddANewElementOutOfIndexBoundFromAnEmptyList(void) {
    plist_add(L, 1, data);
    TEST_ASSERT_EQUAL_UINT(0, plist_size(L));
}

void test_add_ShouldAddANewElementAtTheEndOfAList(void) {
    size_t x = 99;

    for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
        data[i] = i + 1;
        plist_append(L, &data[i]);
    }

    plist_add(L, DATA_ARRAY_LEN, &x);

    TEST_ASSERT_EQUAL_UINT(x, PLIST_GET_UINT(L, DATA_ARRAY_LEN));
}

void test_map_ShouldReturnATransformedList(void) {
    plist *mapped = 0;

    for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
        data[i] = i + 1;
        plist_append(L, &data[i]);
    }

    mapped = plist_map(L, mapper);

    for (size_t i = 0; i < plist_size(L); ++i) {
        TEST_ASSERT_EQUAL_UINT(PLIST_GET_UINT(L, i) * 2,
                              PLIST_GET_UINT(mapped, i));
    }

    plist_destroy_all(mapped, free);
}

void test_map_ShouldMapAnEmptyListAndReturnANewValidEmptyList() {
    plist *mapped = 0;
    mapped = plist_map(L, mapper);
    TEST_ASSERT_NOT_NULL(mapped);
    TEST_ASSERT_TRUE(plist_is_empty(mapped));
    plist_destroy(mapped);
}

void test_append_ShouldAddAnElementToTheEndOfList(void) {
    for (size_t i = 0; i < DATA_ARRAY_LEN - 1; ++i) {
        data[i] = i + 1;
        plist_append(L, &data[i]);
    }

    plist_append(L, &data[DATA_ARRAY_LEN - 1]);
    TEST_ASSERT_EQUAL_UINT(data[DATA_ARRAY_LEN - 1],
                      PLIST_GET_UINT(L, plist_size(L) - 1));
}

void test_append_ShouldAddAnElementToAnEmptyList(void) {
    if (plist_is_empty(L)) {
        plist_append(L, data);
        TEST_ASSERT_FALSE(plist_is_empty(L))
    }
}


void test_clean_ShouldEmptyALoadedList() {
    for (size_t i = 0; i < DATA_ARRAY_LEN - 1; ++i) {
        data[i] = i + 1;
        plist_append(L, &data[i]);
    }

    if (!plist_is_empty(L)) {
        plist_clean(L);
        TEST_ASSERT_TRUE(plist_is_empty(L));
    }
}

void test_filter_ShouldFilterOutAllEvenNumbersFromList(void) {
    plist *filtered = 0;

    for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
        data[i] = i + 1;
        plist_append(L, &data[i]);
    }

    filtered = plist_filter(L, isEven);

    for (size_t i = 0; i < plist_size(filtered); ++i) {
        TEST_ASSERT_TRUE((PLIST_GET_UINT(filtered, i) % 2) == 0);
    }

    plist_destroy(filtered);
}

void test_filter_ShouldFilterAnEmptyListAndReturnAValidEmptyList(void) {
    plist *filtered = plist_filter(L, isEven);
    TEST_ASSERT_NOT_NULL(filtered);
    TEST_ASSERT_TRUE(plist_size(filtered) == 0);
    plist_destroy(filtered);
}

void test_filter_ShouldFilterAListAndMatchAllItems(void) {
    plist *filtered = 0;

    for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
        data[i] = (i + 1) * 2;
        plist_append(L, &data[i]);
    }

    filtered = plist_filter(L, isEven);
    TEST_ASSERT_TRUE(plist_size(L) == plist_size(filtered));

    for (size_t i = 0; i < plist_size(filtered); ++i) {
        TEST_ASSERT_EQUAL_UINT(PLIST_GET_UINT(L, i),
                                      PLIST_GET_UINT(filtered, i));
    }

    plist_destroy(filtered);
}

void test_filter_ShoildFilterAListAndMatchNoneAndReturnAnEmptyList(void) {
    plist *filtered = 0;

    for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
        data[i] = ((i + 1) * 2) - 1;
        plist_append(L, &data[i]);
    }

    filtered = plist_filter(L, isEven);
    TEST_ASSERT_NOT_NULL(filtered);
    TEST_ASSERT_TRUE(plist_size(filtered) == 0);
    plist_destroy(filtered);
}

void test_allMatch_ShouldFilterAListAndAllElementsShouldMatch(void) {
    for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
        data[i] = ((i + 1) * 2);
        plist_append(L, &data[i]);
    }

    TEST_ASSERT_TRUE(plist_all_match(L, isEven));
}

void test_allMatch_ShouldFilterAlistAndNotAllTheElementsWillMatch(void) {
    for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
        data[i] = ((i + 1) * 2) - 1;
        plist_append(L, &data[i]);
    }

    TEST_ASSERT_FALSE(plist_all_match(L, isEven));
}

void test_anyMatch_ShouldFilterAListAndAtLeastOneShouldMatch(void) {
    for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
        data[i] = (i + 1);
        plist_append(L, &data[i]);
    }

    TEST_ASSERT_TRUE(plist_any_match(L, isEven));
}

void test_anyMatch_ShouldFilterAListAndNoElementShouldMatch(void) {
    for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
        data[i] = ((i + 1) * 2) - 1;
        plist_append(L, &data[i]);
    }

    TEST_ASSERT_FALSE(plist_any_match(L, isEven));
}

void test_countMatch_ShouldCountAllEvenNumbersAndShouldBeGreaterThanZero(void) {
    for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
        data[i] = (i + 1);
        plist_append(L, &data[i]);
    }

    TEST_ASSERT_TRUE(plist_count_matching(L, isEven) > 0);
}

void test_countMatch_ShouldCountEvenNumbersAndResultShouldBeZero(void) {
    for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
        data[i] = ((i + 1) * 2) - 1;
        plist_append(L, &data[i]);
    }

    TEST_ASSERT_EQUAL_UINT(0, plist_count_matching(L, isEven));
}

void test_countMatch_ShouldCountEvenNumbersAndResultShouldBeHalfListSize(void) {
    for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
        data[i] = (i + 1);
        plist_append(L, &data[i]);
    }

    TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN / 2, plist_count_matching(L, isEven));
}

void test_get_ShouldGetAValidElementFromTheList(void) {

    for (size_t i = 0; i < DATA_ARRAY_LEN - 1; ++i) {
        data[i] = i + 1;
        plist_append(L, &data[i]);
    }

    TEST_ASSERT_NOT_NULL(plist_get(L, 0));
    TEST_ASSERT_EQUAL_UINT(1, PLIST_GET_UINT(L, 0));

}

void test_get_ShouldGetAllElementsFromTheList(void) {

    for (size_t i = 0; i < DATA_ARRAY_LEN - 1; ++i) {
        data[i] = i + 1;
        plist_append(L, &data[i]);
    }

    for (size_t i = 0; i < DATA_ARRAY_LEN - 1; ++i) {
        TEST_ASSERT_NOT_NULL(plist_get(L, i));
        TEST_ASSERT_EQUAL_UINT(i + 1, PLIST_GET_UINT(L, i));
    }

}

void test_get_ShouldGetTheTailElementFromTheList(void) {

    for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
        data[i] = i + 1;
        plist_append(L, &data[i]);
    }

    TEST_ASSERT_NOT_NULL(plist_get(L, DATA_ARRAY_LEN - 1));
    TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN, PLIST_GET_UINT(L, DATA_ARRAY_LEN - 1));
}

void test_merge_ShouldMergeTwoLists(void) {
    plist *temp = plist_create();
    size_t *val = calloc(DATA_ARRAY_LEN, sizeof(size_t));

    for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
        data[i] = i + 1;
        val[i] = i + DATA_ARRAY_LEN + 1;
        plist_append(temp, &val[i]);
        plist_append(L, &data[i]);
    }

    plist_merge(L, temp);

    for (size_t i = 0; i < DATA_ARRAY_LEN * 2; ++i) {
        TEST_ASSERT_EQUAL_UINT(i + 1, PLIST_GET_UINT(L, i));
    }

    free(val);
    plist_destroy(temp);
}

void test_merge_ShouldMergeShouldHandleAnEmptyListAsSecondParameter(void) {
    plist *temp = plist_create();

    for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
        data[i] = i + 1;
        plist_append(L, &data[i]);
    }

    plist_merge(L, temp);

    for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
        TEST_ASSERT_EQUAL_UINT(i + 1, PLIST_GET_UINT(L, i));
    }

    plist_destroy(temp);
}

void test_merge_ShouldMergeShouldHandleAnEmptyListAsFirstParameter(void) {
    plist *temp = plist_create();

    for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
        data[i] = i + 1;
        plist_append(L, &data[i]);
    }

    plist_merge(temp, L);

    for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
        TEST_ASSERT_EQUAL_UINT(i + 1, PLIST_GET_UINT(L, i));
    }

    plist_destroy(temp);
}
