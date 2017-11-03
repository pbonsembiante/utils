#include <string.h>

#define plist_member_t size_t

#include "putils/plist.h"
#include "unity.h"

#define DATA_ARRAY_LEN 10

plist* L = 0;
size_t* data = 0;

/* See: https://stackoverflow.com/a/22637665/6194674*/
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

void setUp(void) {
    data = calloc(DATA_ARRAY_LEN, sizeof(plist_member_t));
    L = plist_create();
}

void tearDown(void) {
    free(data);
    plist_destroy(L);
}

bool comp(const plist_member_t* a, const plist_member_t* b) {
    const plist_member_t* _a = a;
    const plist_member_t* _b = b;
    return *_a < *_b;
}

plist_member_t* mapper(const plist_member_t* _orig) {
    const plist_member_t* orig = _orig;
    plist_member_t* mapped = calloc(1, sizeof(plist_member_t));
    *mapped = *orig * 2;
    return mapped;
}

bool isEven(const plist_member_t* _val) {
    const plist_member_t* number = _val;
    return (*number % 2) == 0;
}

bool is99(const plist_member_t* _val) {
    const plist_member_t* number = _val;
    return (*number == 99);
}

void loadList(void) {
    for (plist_member_t i = 0; i < DATA_ARRAY_LEN; ++i) {
        data[i] = i + 1;
        plist_append(L, &data[i]);
    }
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
    loadList();
    TEST_ASSERT_EQUAL_UINT(plist_size(L), DATA_ARRAY_LEN);
}

void test_size_ShouldBeZeroForACleanList(void) {
    loadList();
    plist_clean(L);
    TEST_ASSERT_EQUAL_UINT(plist_size(L), 0);
}

void test_size_ShouldNotBeZeroAfterAddingElements(void) {
    loadList();
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
    size_t x = 99;
    TEST_ASSERT_TRUE(plist_is_empty(L));
    plist_add(L, 0, &x);
    TEST_ASSERT_EQUAL_UINT(1, plist_size(L));
}

void test_add_ShouldAddANewElementAndCheckIt(void) {
    for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
        data[i] = i + 1;
    }

    plist_append(L, data);
    int* out = plist_get(L, 0);
    TEST_ASSERT_EQUAL_PTR(data, out);
    TEST_ASSERT_EQUAL_UINT_ARRAY(data, out, DATA_ARRAY_LEN);
}

void test_add_ShouldAddANewElementInTheMiddleOfAList(void) {
    loadList();

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

    loadList();
    plist_add(L, DATA_ARRAY_LEN, &x);

    TEST_ASSERT_EQUAL_UINT(x, PLIST_GET_UINT(L, DATA_ARRAY_LEN));
}

void test_map_ShouldReturnATransformedList(void) {
    plist* mapped = 0;

    loadList();

    mapped = plist_map(L, mapper);

    for (size_t i = 0; i < plist_size(L); ++i) {
        TEST_ASSERT_EQUAL_UINT(PLIST_GET_UINT(L, i) * 2,
                               PLIST_GET_UINT(mapped, i));
    }

    plist_destroy_all(mapped, free);
}

void test_map_ShouldMapAnEmptyListAndReturnANewValidEmptyList() {
    plist* mapped = 0;
    mapped = plist_map(L, mapper);
    TEST_ASSERT_NOT_NULL(mapped);
    TEST_ASSERT_TRUE(plist_is_empty(mapped));
    plist_destroy(mapped);
}

void test_append_ShouldAddAnElementToTheEndOfList(void) {
    loadList();

    plist_append(L, &data[DATA_ARRAY_LEN - 1]);
    TEST_ASSERT_EQUAL_UINT(data[DATA_ARRAY_LEN - 1],
                           PLIST_GET_UINT(L, plist_size(L) - 1));
}

void test_append_ShouldAddAnElementToAnEmptyList(void) {
    TEST_ASSERT_TRUE(plist_is_empty(L));
    plist_append(L, data);
    TEST_ASSERT_FALSE(plist_is_empty(L))
}


void test_clean_ShouldEmptyALoadedList() {
    loadList();

    TEST_ASSERT_FALSE(plist_is_empty(L));
    plist_clean(L);
    TEST_ASSERT_TRUE(plist_is_empty(L));

}

void test_filter_ShouldFilterOutAllEvenNumbersFromList(void) {
    plist* filtered = 0;

    loadList();
    filtered = plist_filter(L, isEven);

    for (size_t i = 0; i < plist_size(filtered); ++i) {
        TEST_ASSERT_TRUE((PLIST_GET_UINT(filtered, i) % 2) == 0);
    }

    plist_destroy(filtered);
}

void test_filter_ShouldFilterAnEmptyListAndReturnAValidEmptyList(void) {
    plist* filtered = plist_filter(L, isEven);
    TEST_ASSERT_NOT_NULL(filtered);
    TEST_ASSERT_TRUE(plist_size(filtered) == 0);
    plist_destroy(filtered);
}

void test_filter_ShouldFilterAListAndMatchAllItems(void) {
    plist* filtered = 0;

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

void test_filter_ShouldFilterAListAndMatchNoneAndReturnAnEmptyList(void) {
    plist* filtered = 0;

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
    loadList();

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
    loadList();

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
    loadList();
    TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN / 2, plist_count_matching(L, isEven));
}


void test_get_ShouldGetAllElementsFromTheList(void) {
    loadList();

    for (size_t i = 0; i < DATA_ARRAY_LEN - 1; ++i) {
        TEST_ASSERT_NOT_NULL(plist_get(L, i));
        TEST_ASSERT_EQUAL_UINT(i + 1, PLIST_GET_UINT(L, i));
    }

}

void test_get_ShouldGetTheTailElementFromTheList(void) {
    loadList();

    TEST_ASSERT_NOT_NULL(plist_get(L, DATA_ARRAY_LEN - 1));
    TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN,
                           PLIST_GET_UINT(L, DATA_ARRAY_LEN - 1));
}

void test_merge_ShouldMergeTwoLists(void) {
    plist* temp = plist_create();
    size_t* val = calloc(DATA_ARRAY_LEN, sizeof(size_t));

    loadList();

    for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
        val[i] = i + DATA_ARRAY_LEN + 1;
        plist_append(temp, &val[i]);
    }

    plist_merge(L, temp);

    for (size_t i = 0; i < DATA_ARRAY_LEN * 2; ++i) {
        TEST_ASSERT_EQUAL_UINT(i + 1, PLIST_GET_UINT(L, i));
    }

    free(val);
    plist_destroy(temp);
}

void test_merge_ShouldMergeShouldHandleAnEmptyListAsSecondParameter(void) {
    plist* temp = plist_create();

    loadList();

    plist_merge(L, temp);

    for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
        TEST_ASSERT_EQUAL_UINT(i + 1, PLIST_GET_UINT(L, i));
    }

    plist_destroy(temp);
}

void test_merge_ShouldMergeShouldHandleAnEmptyListAsFirstParameter(void) {
    plist* temp = plist_create();

    loadList();

    plist_merge(temp, L);

    for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
        TEST_ASSERT_EQUAL_UINT(i + 1, PLIST_GET_UINT(L, i));
    }

    plist_destroy(temp);
}

void test_getElements_ShouldReturnANewEmptyList(void) {
    plist* sublist = 0;

    sublist = plist_get_elements(L, DATA_ARRAY_LEN / 2);

    TEST_ASSERT_NOT_NULL(sublist);
    TEST_ASSERT_TRUE(plist_is_empty(sublist));
}

void test_getElements_ShouldReturnANewLoadedList(void) {
    plist* sublist = 0;

    loadList();
    sublist = plist_get_elements(L, DATA_ARRAY_LEN / 2);

    TEST_ASSERT_NOT_NULL(sublist);
    TEST_ASSERT_FALSE(plist_is_empty(sublist));
    TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN / 2, plist_size(sublist));
}

void test_getRemoving_ShouldReturnAnEmptyList(void) {
    plist* sublist = 0;

    sublist = plist_get_removing_elements(L, DATA_ARRAY_LEN / 2);

    TEST_ASSERT_NOT_NULL(sublist);
    TEST_ASSERT_TRUE(plist_is_empty(sublist));
}

void test_getRemoving_ShouldReturnAnNewLoadedListAndRemoveFromOriginal(void) {
    plist* sublist = 0;

    loadList();
    sublist = plist_get_removing_elements(L, DATA_ARRAY_LEN / 2);

    TEST_ASSERT_NOT_NULL(sublist);
    TEST_ASSERT_FALSE(plist_is_empty(sublist));
    TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN / 2, plist_size(sublist));
    TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN / 2, plist_size(L));
}

void test_remove_ShouldRemoveAnElementFromList(void) {
    loadList();
    TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN, plist_size(L));
    plist_remove(L, DATA_ARRAY_LEN / 2);
    TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN - 1, plist_size(L));
}

void test_remove_ShouldRemoveFirstElementFromList(void) {
    loadList();
    TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN, plist_size(L));
    plist_remove(L, 0);
    TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN - 1, plist_size(L));
    TEST_ASSERT_EQUAL_UINT(2, PLIST_GET_UINT(L, 0));
}

void test_remove_ShouldRemoveLastElementFromList(void) {
    loadList();

    TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN, plist_size(L));
    plist_remove(L, DATA_ARRAY_LEN - 1);

    TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN - 1, plist_size(L));

    TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN - 1,
                           PLIST_GET_UINT(L, plist_size(L) - 1));
}

void test_prepend_ShouldAddAnElementToTheHeadOfList(void) {
    size_t x = 99;

    loadList();
    plist_prepend(L, &x);

    TEST_ASSERT_EQUAL_UINT(data[DATA_ARRAY_LEN - 1],
                           PLIST_GET_UINT(L, plist_size(L) - 1));
}

void test_prepend_ShouldAddAnElementToAnEmptyList(void) {
    size_t x = 99;

    TEST_ASSERT_TRUE(plist_is_empty(L));
    plist_prepend(L, &x);
    TEST_ASSERT_FALSE(plist_is_empty(L))

}

void test_replace_ShouldReplaceAnElementInTheList(void) {
    size_t x = 99;

    loadList();
    plist_replace(L, DATA_ARRAY_LEN / 2, &x);

    TEST_ASSERT_EQUAL_UINT(x, PLIST_GET_UINT(L, DATA_ARRAY_LEN / 2));
}

void test_replace_ShouldReplaceFirstElementInTheList(void) {
    size_t x = 99;

    loadList();
    plist_replace(L, 0, &x);

    TEST_ASSERT_EQUAL_UINT(x, PLIST_GET_UINT(L, 0));
}

void test_replace_ShouldReplaceLastElementInTheList(void) {
    size_t x = 99;

    loadList();
    plist_replace(L, plist_size(L) - 1, &x);

    TEST_ASSERT_EQUAL_UINT(x, PLIST_GET_UINT(L, plist_size(L) - 1));
}

void test_replace_ShouldReplaceANullElementInTheList(void) {
    size_t x = 99;

    loadList();
    plist_append(L, 0);
    void* element = plist_replace(L, plist_size(L) - 1, &x);

    TEST_ASSERT_EQUAL_UINT(x, PLIST_GET_UINT(L, plist_size(L) - 1));
    TEST_ASSERT_NULL(element);
}

void test_find_ShouldRetrieveTheDesiredItem(void) {
    loadList();
    TEST_ASSERT_EQUAL_UINT(2, *((size_t*) plist_find(L, isEven, 0)));
}

void test_find_ShouldRetrieveTheDesiredItemAndGetItsIndex(void) {
    size_t index = 0;
    loadList();
    TEST_ASSERT_EQUAL_UINT(2, *((size_t*) plist_find(L, isEven, &index)));
    TEST_ASSERT_EQUAL_UINT(1, index);
}

void test_find_ShouldNotFindElementsOnAnEmptyList(void) {
    TEST_ASSERT_NULL(plist_find(L, isEven, 0));
}

void test_find_ShouldNotFindIfNotEvaluatorIsProvidedAndEmptyList(void) {
    TEST_ASSERT_NULL(plist_find(L, 0, 0));
}

void test_find_ShouldNotFindIfNotEvaluatorIsProvidedAndLoadedList(void) {
    loadList();
    TEST_ASSERT_NULL(plist_find(L, 0, 0));
}

void test_removeDestroy_ShouldRemoveAndDestroyAnElement(void) {
    size_t* x = calloc(1, sizeof(size_t));
    *x = 99;
    loadList();

    plist_append(L, x);
    plist_remove_and_destroy(L, plist_size(L) - 1, free);

    TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN, plist_size(L));
    TEST_ASSERT_TRUE(isFreed(x));
}

void test_isEmpty_ShouldReturnTrueOnAnEmptyList(void) {
    TEST_ASSERT_TRUE(plist_is_empty(L));
}

void test_isEmpty_ShouldReturnFalseOnAnLoadedList(void) {
    loadList();
    TEST_ASSERT_FALSE(plist_is_empty(L));
}

void test_destroy_ShouldFreeTheList(void) {
    /* Do not use L since we cannot avoid tearDown */
    plist* tmp = plist_create();
    TEST_ASSERT_NOT_NULL(tmp);
    plist_destroy(tmp);
    TEST_ASSERT_TRUE(isFreed(tmp));
}

void
test_replaceDestroy_ShouldReplaceAnElementFromTheListAndDestroyTheOldOne(void) {
    size_t* x = calloc(1, sizeof(size_t));
    *x = 99;
    size_t y = 0;

    plist_append(L, x);
    plist_replace_and_destroy(L, 0, &y, free);

    TEST_ASSERT_EQUAL_UINT(1, plist_size(L));
    TEST_ASSERT_TRUE(isFreed(x));
    TEST_ASSERT_EQUAL_UINT(y, PLIST_GET_UINT(L, 0));
}

void
test_replaceDestroy_ShouldReplaceAnElementFromTheListAndKeepTheOldOneIfNoDestructorFunction(
    void) {
    size_t* x = calloc(1, sizeof(size_t));
    *x = 99;
    size_t y = 0;

    plist_append(L, x);
    plist_replace_and_destroy(L, 0, &y, 0);

    TEST_ASSERT_EQUAL_UINT(1, plist_size(L));
    TEST_ASSERT_FALSE(isFreed(x));
    TEST_ASSERT_EQUAL_UINT(y, PLIST_GET_UINT(L, 0));
    free(x);
}

void test_replaceDestroy_ShouldReplaceANullElementFromTheList(void) {
    size_t* x = 0;
    size_t y = 99;

    plist_append(L, x);
    plist_replace_and_destroy(L, 0, &y, free);

    TEST_ASSERT_EQUAL_UINT(1, plist_size(L));
    TEST_ASSERT_EQUAL_UINT(y, PLIST_GET_UINT(L, 0));
}

void test_removeSelected_ShouldRemoveAParticularElementFromTheList(void) {
    loadList();

    TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN, plist_size(L));

    size_t* x = plist_remove_selected(L, isEven);

    TEST_ASSERT_NOT_NULL(x);
    TEST_ASSERT_EQUAL_UINT(*x, data[1]);
    TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN - 1, plist_size(L));
}

void
test_removeDestroyingSelected_ShouldRemoveAndDestroyAnElementSelectedFromTheList(
    void) {
    size_t* x = calloc(1, sizeof(size_t));
    *x = 99;

    loadList();
    plist_append(L, x);

    TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN + 1, plist_size(L));

    plist_remove_destroying_selected(L, is99, free);

    TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN, plist_size(L));
    TEST_ASSERT_TRUE(isFreed(x));
}

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

    RUN_TEST(test_filter_ShouldFilterAListAndMatchNoneAndReturnAnEmptyList);
    RUN_TEST(test_filter_ShouldFilterAListAndMatchAllItems);
    RUN_TEST(test_filter_ShouldFilterAnEmptyListAndReturnAValidEmptyList);
    RUN_TEST(test_filter_ShouldFilterOutAllEvenNumbersFromList);

    RUN_TEST(test_allMatch_ShouldFilterAListAndAllElementsShouldMatch);
    RUN_TEST(test_allMatch_ShouldFilterAlistAndNotAllTheElementsWillMatch);

    RUN_TEST(test_anyMatch_ShouldFilterAListAndAtLeastOneShouldMatch);
    RUN_TEST(test_anyMatch_ShouldFilterAListAndNoElementShouldMatch);

    RUN_TEST(
        test_countMatch_ShouldCountAllEvenNumbersAndShouldBeGreaterThanZero);
    RUN_TEST(
        test_countMatch_ShouldCountEvenNumbersAndResultShouldBeHalfListSize);
    RUN_TEST(test_countMatch_ShouldCountEvenNumbersAndResultShouldBeZero);

    RUN_TEST(test_get_ShouldGetAllElementsFromTheList);
    RUN_TEST(test_get_ShouldGetTheTailElementFromTheList);

    RUN_TEST(test_merge_ShouldMergeTwoLists);
    RUN_TEST(test_merge_ShouldMergeShouldHandleAnEmptyListAsSecondParameter);
    RUN_TEST(test_merge_ShouldMergeShouldHandleAnEmptyListAsFirstParameter);

    RUN_TEST(test_getElements_ShouldReturnANewEmptyList);
    RUN_TEST(test_getElements_ShouldReturnANewLoadedList);

    RUN_TEST(test_getRemoving_ShouldReturnAnEmptyList);
    RUN_TEST(test_getRemoving_ShouldReturnAnNewLoadedListAndRemoveFromOriginal);

    RUN_TEST(test_remove_ShouldRemoveAnElementFromList);
    RUN_TEST(test_remove_ShouldRemoveFirstElementFromList);
    RUN_TEST(test_remove_ShouldRemoveLastElementFromList);

    RUN_TEST(test_prepend_ShouldAddAnElementToTheHeadOfList);
    RUN_TEST(test_prepend_ShouldAddAnElementToAnEmptyList);

    RUN_TEST(test_replace_ShouldReplaceAnElementInTheList);
    RUN_TEST(test_replace_ShouldReplaceFirstElementInTheList);
    RUN_TEST(test_replace_ShouldReplaceLastElementInTheList);
    RUN_TEST(test_replace_ShouldReplaceANullElementInTheList);

    RUN_TEST(test_find_ShouldRetrieveTheDesiredItem);
    RUN_TEST(test_find_ShouldRetrieveTheDesiredItemAndGetItsIndex);
    RUN_TEST(test_find_ShouldNotFindIfNotEvaluatorIsProvidedAndLoadedList);
    RUN_TEST(test_find_ShouldNotFindIfNotEvaluatorIsProvidedAndEmptyList);
    RUN_TEST(test_find_ShouldNotFindElementsOnAnEmptyList);

    RUN_TEST(test_removeDestroy_ShouldRemoveAndDestroyAnElement);

    RUN_TEST(test_isEmpty_ShouldReturnTrueOnAnEmptyList);
    RUN_TEST(test_isEmpty_ShouldReturnFalseOnAnLoadedList);

    RUN_TEST(test_destroy_ShouldFreeTheList);

    RUN_TEST(
        test_replaceDestroy_ShouldReplaceAnElementFromTheListAndDestroyTheOldOne);
    RUN_TEST(
        test_replaceDestroy_ShouldReplaceAnElementFromTheListAndKeepTheOldOneIfNoDestructorFunction);
    RUN_TEST(test_replaceDestroy_ShouldReplaceANullElementFromTheList);

    RUN_TEST(test_removeSelected_ShouldRemoveAParticularElementFromTheList);

    RUN_TEST(
        test_removeDestroyingSelected_ShouldRemoveAndDestroyAnElementSelectedFromTheList);

    return UNITY_END();
}
