/***************************************************************************
 * Copyright (C) 2016 - 2022 Patricio Bonsembiante. All rights reserved.
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

#include <string.h>

#define plist_member_t size_t

#include "putils/plist.h"
#include "unity.h"

#define DATA_ARRAY_LEN 10

plist *L = 0;
size_t *data = 0;

void setUp(void) {
  data = calloc(DATA_ARRAY_LEN, sizeof(plist_member_t));
  L = plist_create();
}

void tearDown(void) {
  free(data);
  plist_destroy(&L);
}

bool helper_comparator(const plist_member_t *a, const plist_member_t *b) {
  return *a < *b;
}

plist_member_t *helper_mapper(const plist_member_t *_orig) {
  plist_member_t *mapped = calloc(1, sizeof(plist_member_t));
  *mapped = *_orig * 2;
  return mapped;
}

bool helper_is_even(const plist_member_t *_val) {
  return (*_val % 2) == 0;
}

bool helper_is_99(const plist_member_t *_val) {
  return *_val == 99;
}

void helper_load_list(plist *list) {
  for (plist_member_t i = 0; i < DATA_ARRAY_LEN; ++i) {
    data[i] = i + 1;
    plist_append(list, &data[i]);
  }
}

void helper_load_default_list(void) {
  helper_load_list(L);
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
  helper_load_default_list();
  TEST_ASSERT_EQUAL_UINT(plist_size(L), DATA_ARRAY_LEN);
}

void test_size_ShouldBeZeroForACleanList(void) {
  helper_load_default_list();
  plist_clean(L);
  TEST_ASSERT_EQUAL_UINT(plist_size(L), 0);
}

void test_size_ShouldNotBeZeroAfterAddingElements(void) {
  helper_load_default_list();
  TEST_ASSERT_TRUE(plist_size(L) != 0);
}

void test_sort_ShouldOrderAOneItemList(void) {
  size_t x = 99;

  plist_append(L, &x);
  plist_sort(L, helper_comparator);

  TEST_ASSERT_EQUAL_UINT(99, PLIST_GET_UINT(L, 0));
}

void test_sort_ShouldNotErrorWithANullList(void) {
  plist_sort( 0, helper_comparator);
  TEST_PASS();
}

void test_sort_ShouldHandleAnEmptyList(void) {
  plist_sort(L, helper_comparator);
  TEST_ASSERT_TRUE(L);
  TEST_ASSERT_EQUAL_UINT(0, plist_size(L));
}

void test_sort_ShouldHandleAListWithTwoElements(void) {
  size_t x = 99;
  plist_append(L, &x);
  plist_append(L, &x);
  plist_sort(L, helper_comparator);
  TEST_ASSERT_TRUE(L);
  TEST_ASSERT_EQUAL_UINT(2, plist_size(L));
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

  plist_sort(L, helper_comparator);
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
  int *out = plist_get(L, 0);
  TEST_ASSERT_EQUAL_PTR(data, out);
  TEST_ASSERT_EQUAL_UINT_ARRAY(data, out, DATA_ARRAY_LEN);
}

void test_add_ShouldAddANewElementInTheMiddleOfAList(void) {
  helper_load_default_list();

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

  helper_load_default_list();
  plist_add(L, DATA_ARRAY_LEN, &x);

  TEST_ASSERT_EQUAL_UINT(x, PLIST_GET_UINT(L, DATA_ARRAY_LEN));
}

void test_map_ShouldReturnATransformedList(void) {
  helper_load_default_list();

  plist *mapped = plist_map(L, helper_mapper);
  for (size_t i = 0; i < plist_size(L); ++i) {
    TEST_ASSERT_EQUAL_UINT(PLIST_GET_UINT(L, i) * 2, PLIST_GET_UINT(mapped, i));
  }

  plist_destroy_all(&mapped, free);
}

void test_map_ShouldMapAnEmptyListAndReturnANewValidEmptyList() {
  plist *mapped = plist_map(L, helper_mapper);
  TEST_ASSERT_NOT_NULL(mapped);
  TEST_ASSERT_TRUE(plist_is_empty(mapped));
  plist_destroy(&mapped);
}

void test_append_ShouldAddAnElementToTheEndOfList(void) {
  helper_load_default_list();

  plist_append(L, &data[DATA_ARRAY_LEN - 1]);
  TEST_ASSERT_EQUAL_UINT(data[DATA_ARRAY_LEN - 1],
                         PLIST_GET_UINT(L, plist_size(L) - 1));
}

void test_append_ShouldAddAnElementToAnEmptyList(void) {
  TEST_ASSERT_TRUE(plist_is_empty(L));
  plist_append(L, data);
  TEST_ASSERT_FALSE(plist_is_empty(L));
}

void test_clean_ShouldNotErrorOnANullList() {
  plist_clean(0);
  TEST_PASS();
}

void test_clean_ShouldEmptyALoadedList() {
  helper_load_default_list();

  TEST_ASSERT_FALSE(plist_is_empty(L));
  plist_clean(L);
  TEST_ASSERT_TRUE(plist_is_empty(L));
}

void test_filter_ShouldFilterOutAllEvenNumbersFromList(void) {
  helper_load_default_list();

  plist *filtered = plist_filter(L, helper_is_even);
  for (size_t i = 0; i < plist_size(filtered); ++i) {
    TEST_ASSERT_TRUE((PLIST_GET_UINT(filtered, i) % 2) == 0);
  }

  plist_destroy(&filtered);
}

void test_filter_ShouldFilterAnEmptyListAndReturnAValidEmptyList(void) {
  plist *filtered = plist_filter(L, helper_is_even);
  TEST_ASSERT_NOT_NULL(filtered);
  TEST_ASSERT_TRUE(plist_size(filtered) == 0);
  plist_destroy(&filtered);
}

void test_filter_ShouldFilterAListAndMatchAllItems(void) {

  for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
    data[i] = (i + 1) * 2;
    plist_append(L, &data[i]);
  }

  plist *filtered = plist_filter(L, helper_is_even);
  TEST_ASSERT_TRUE(plist_size(L) == plist_size(filtered));

  for (size_t i = 0; i < plist_size(filtered); ++i) {
    TEST_ASSERT_EQUAL_UINT(PLIST_GET_UINT(L, i), PLIST_GET_UINT(filtered, i));
  }

  plist_destroy(&filtered);
}

void test_filter_ShouldFilterAListAndMatchNoneAndReturnAnEmptyList(void) {

  for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
    data[i] = ((i + 1) * 2) - 1;
    plist_append(L, &data[i]);
  }

  plist *filtered = plist_filter(L, helper_is_even);
  TEST_ASSERT_NOT_NULL(filtered);
  TEST_ASSERT_TRUE(plist_size(filtered) == 0);
  plist_destroy(&filtered);
}

void test_allMatch_ShouldFilterAListAndAllElementsShouldMatch(void) {
  for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
    data[i] = ((i + 1) * 2);
    plist_append(L, &data[i]);
  }

  TEST_ASSERT_TRUE(plist_all_match(L, helper_is_even));
}

void test_allMatch_ShouldFilterAlistAndNotAllTheElementsWillMatch(void) {
  for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
    data[i] = ((i + 1) * 2) - 1;
    plist_append(L, &data[i]);
  }

  TEST_ASSERT_FALSE(plist_all_match(L, helper_is_even));
}

void test_anyMatch_ShouldFilterAListAndAtLeastOneShouldMatch(void) {
  helper_load_default_list();

  TEST_ASSERT_TRUE(plist_any_match(L, helper_is_even));
}

void test_anyMatch_ShouldFilterAListAndNoElementShouldMatch(void) {
  for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
    data[i] = ((i + 1) * 2) - 1;
    plist_append(L, &data[i]);
  }

  TEST_ASSERT_FALSE(plist_any_match(L, helper_is_even));
}

void test_countMatch_ShouldCountAllEvenNumbersAndShouldBeGreaterThanZero(void) {
  helper_load_default_list();

  TEST_ASSERT_TRUE(plist_count_matching(L, helper_is_even) > 0);
}

void test_countMatch_ShouldCountEvenNumbersAndResultShouldBeZero(void) {
  for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
    data[i] = ((i + 1) * 2) - 1;
    plist_append(L, &data[i]);
  }

  TEST_ASSERT_EQUAL_UINT(0, plist_count_matching(L, helper_is_even));
}

void test_countMatch_ShouldCountEvenNumbersAndResultShouldBeHalfListSize(void) {
  helper_load_default_list();
  TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN / 2,
                         plist_count_matching(L, helper_is_even));
}

void test_get_ShouldGetAllElementsFromTheList(void) {
  helper_load_default_list();

  for (size_t i = 0; i < DATA_ARRAY_LEN - 1; ++i) {
    TEST_ASSERT_NOT_NULL(plist_get(L, i));
    TEST_ASSERT_EQUAL_UINT(i + 1, PLIST_GET_UINT(L, i));
  }
}

void test_get_ShouldGetTheTailElementFromTheList(void) {
  helper_load_default_list();

  TEST_ASSERT_NOT_NULL(plist_get(L, DATA_ARRAY_LEN - 1));
  TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN, PLIST_GET_UINT(L, DATA_ARRAY_LEN - 1));
}

void test_merge_ShouldMergeTwoLists(void) {
  plist *temp = plist_create();
  size_t *val = calloc(DATA_ARRAY_LEN, sizeof(size_t));

  helper_load_default_list();

  for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
    val[i] = i + DATA_ARRAY_LEN + 1;
    plist_append(temp, &val[i]);
  }

  plist_merge(L, temp);

  for (size_t i = 0; i < DATA_ARRAY_LEN * 2; ++i) {
    TEST_ASSERT_EQUAL_UINT(i + 1, PLIST_GET_UINT(L, i));
  }

  free(val);
  plist_destroy(&temp);
}

void test_merge_ShouldMergeShouldHandleAnEmptyListAsSecondParameter(void) {
  plist *temp = plist_create();

  helper_load_default_list();

  plist_merge(L, temp);

  for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
    TEST_ASSERT_EQUAL_UINT(i + 1, PLIST_GET_UINT(L, i));
  }

  plist_destroy(&temp);
}

void test_merge_ShouldHandleAnEmptyListAsFirstParameter(void) {
  plist *temp = plist_create();

  helper_load_default_list();

  plist_merge(temp, L);

  for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
    TEST_ASSERT_EQUAL_UINT(i + 1, PLIST_GET_UINT(L, i));
  }

  plist_destroy(&temp);
}

void test_getElements_ShouldReturnANewEmptyList(void) {
  plist *sublist = 0;

  sublist = plist_get_elements(L, DATA_ARRAY_LEN / 2);

  TEST_ASSERT_NOT_NULL(sublist);
  TEST_ASSERT_TRUE(plist_is_empty(sublist));
  plist_destroy(&sublist);
}

void test_getElements_ShouldReturnANewLoadedList(void) {
  plist *sublist = 0;

  helper_load_default_list();
  sublist = plist_get_elements(L, DATA_ARRAY_LEN / 2);

  TEST_ASSERT_NOT_NULL(sublist);
  TEST_ASSERT_FALSE(plist_is_empty(sublist));
  TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN / 2, plist_size(sublist));
  plist_destroy(&sublist);
}

void test_getRemoving_ShouldNotErrorOnANullList(void) {
  plist *sublist = plist_get_removing_elements(0, DATA_ARRAY_LEN / 2);
  TEST_ASSERT_NOT_NULL(sublist);
  TEST_ASSERT_TRUE(plist_is_empty(sublist));
  plist_destroy(&sublist);
}

void test_getRemoving_ShouldReturnAnEmptyListWhenRemovingFromAnEmptyList(void) {
  plist *sublist = plist_get_removing_elements(L, DATA_ARRAY_LEN / 2);
  TEST_ASSERT_NOT_NULL(sublist);
  TEST_ASSERT_TRUE(plist_is_empty(sublist));
  plist_destroy(&sublist);
}

void test_getRemoving_ShouldReturnAnNewLoadedListAndRemoveFromOriginal(void) {
  helper_load_default_list();

  plist *sublist = plist_get_removing_elements(L, DATA_ARRAY_LEN / 2);

  TEST_ASSERT_NOT_NULL(sublist);
  TEST_ASSERT_FALSE(plist_is_empty(sublist));
  TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN / 2, plist_size(sublist));
  TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN / 2, plist_size(L));
  plist_destroy(&sublist);
}

void test_remove_ShouldNotRemoveFromAnEmptyList(void) {
  TEST_ASSERT_TRUE(plist_is_empty(L));
  TEST_ASSERT_NULL(plist_remove(L, 0));
}

void test_remove_ShouldRemoveAnElementFromList(void) {
  helper_load_default_list();
  TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN, plist_size(L));
  plist_remove(L, DATA_ARRAY_LEN / 2);
  TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN - 1, plist_size(L));
}

void test_remove_ShouldRemoveFirstElementFromList(void) {
  helper_load_default_list();
  TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN, plist_size(L));
  plist_remove(L, 0);
  TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN - 1, plist_size(L));
  TEST_ASSERT_EQUAL_UINT(2, PLIST_GET_UINT(L, 0));
}

void test_remove_ShouldRemoveLastElementFromList(void) {
  helper_load_default_list();

  TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN, plist_size(L));
  plist_remove(L, DATA_ARRAY_LEN - 1);

  TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN - 1, plist_size(L));

  TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN - 1,
                         PLIST_GET_UINT(L, plist_size(L) - 1));
}

void test_remove_ShouldRemoveAllElementsFromList(void) {
  helper_load_default_list();

  TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN, plist_size(L));
  for (size_t i = 0; i < DATA_ARRAY_LEN; ++i) {
    plist_member_t *value = plist_remove(L, 0);
    TEST_ASSERT_EQUAL_UINT(i + 1, *value);
  }

  TEST_ASSERT_EQUAL_UINT(0, plist_size(L));
  TEST_ASSERT_TRUE(plist_is_empty(L));
}


void test_prepend_ShouldAddAnElementToTheHeadOfList(void) {
  size_t x = 99;

  helper_load_default_list();
  plist_prepend(L, &x);

  TEST_ASSERT_EQUAL_UINT(data[DATA_ARRAY_LEN - 1],
                         PLIST_GET_UINT(L, plist_size(L) - 1));
}

void test_prepend_ShouldAddAnElementToAnEmptyList(void) {
  size_t x = 99;

  TEST_ASSERT_TRUE(plist_is_empty(L));
  plist_prepend(L, &x);
  TEST_ASSERT_FALSE(plist_is_empty(L));
}

void test_replace_ShouldReplaceAnElementInTheList(void) {
  size_t x = 99;

  helper_load_default_list();
  plist_replace(L, DATA_ARRAY_LEN / 2, &x);

  TEST_ASSERT_EQUAL_UINT(x, PLIST_GET_UINT(L, DATA_ARRAY_LEN / 2));
}

void test_replace_ShouldReplaceFirstElementInTheList(void) {
  size_t x = 99;

  helper_load_default_list();
  plist_replace(L, 0, &x);

  TEST_ASSERT_EQUAL_UINT(x, PLIST_GET_UINT(L, 0));
}

void test_replace_ShouldReplaceLastElementInTheList(void) {
  size_t x = 99;

  helper_load_default_list();
  plist_replace(L, plist_size(L) - 1, &x);

  TEST_ASSERT_EQUAL_UINT(x, PLIST_GET_UINT(L, plist_size(L) - 1));
}

void test_replace_ShouldReplaceANullElementInTheList(void) {
  size_t x = 99;

  helper_load_default_list();
  plist_append(L, 0);
  void *element = plist_replace(L, plist_size(L) - 1, &x);

  TEST_ASSERT_EQUAL_UINT(x, PLIST_GET_UINT(L, plist_size(L) - 1));
  TEST_ASSERT_NULL(element);
}

void test_find_ShouldRetrieveTheDesiredItem(void) {
  helper_load_default_list();
  TEST_ASSERT_EQUAL_UINT(2, *((size_t *)plist_find(L, helper_is_even, 0)));
}

void test_find_ShouldRetrieveTheDesiredItemAndGetItsIndex(void) {
  size_t index = 0;
  helper_load_default_list();
  TEST_ASSERT_EQUAL_UINT(2, *((size_t *)plist_find(L, helper_is_even, &index)));
  TEST_ASSERT_EQUAL_UINT(1, index);
}

void test_find_ShouldNotFindElementsOnAnEmptyList(void) {
  TEST_ASSERT_NULL(plist_find(L, helper_is_even, 0));
}

void test_find_ShouldNotFindIfNotEvaluatorIsProvidedAndEmptyList(void) {
  TEST_ASSERT_NULL(plist_find(L, 0, 0));
}

void test_find_ShouldNotFindIfNotEvaluatorIsProvidedAndLoadedList(void) {
  helper_load_default_list();
  TEST_ASSERT_NULL(plist_find(L, 0, 0));
}

void test_removeDestroy_ShouldRemoveAndDestroyAnElement(void) {
  size_t *x = calloc(1, sizeof(size_t));
  *x = 99;
  helper_load_default_list();

  plist_append(L, x);
  plist_remove_and_destroy(L, plist_size(L) - 1, free);

  TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN, plist_size(L));
}

void test_isEmpty_ShouldReturnTrueOnAnEmptyList(void) {
  TEST_ASSERT_TRUE(plist_is_empty(L));
}

void test_isEmpty_ShouldReturnFalseOnAnLoadedList(void) {
  helper_load_default_list();
  TEST_ASSERT_FALSE(plist_is_empty(L));
}

void test_destroy_ShouldNotFailOnANullList(void) {
  plist *p = 0;
  plist_destroy(&p);
  /* Explicitly pass the test as if we got here, we are done*/
  TEST_PASS();
}

void test_destroy_ShouldFreeTheList(void) {
  /* Do not use L since we cannot avoid tearDown */
  plist *tmp = plist_create();
  TEST_ASSERT_NOT_NULL(tmp);
  plist_destroy(&tmp);
  TEST_ASSERT_NULL(tmp);
}

void test_destroy_ShouldFreeAndCleanTheList(void) {
  /* Do not use L since we cannot avoid tearDown */
  plist *tmp = plist_create();
  helper_load_list(tmp);
  TEST_ASSERT_NOT_NULL(tmp);
  plist_destroy(&tmp);
  TEST_ASSERT_NULL(tmp);
}

void test_destroyAll_ShouldNotFailOnANullList(void) {
  plist *p = 0;
  plist_destroy_all(&p, free);
}

void test_replaceDestroy_ShouldReplaceAnElementFromTheListAndDestroyTheOldOne(void) {
  size_t *x = calloc(1, sizeof(size_t));
  *x = 99;
  size_t y = 0;

  plist_append(L, x);
  plist_replace_and_destroy(L, 0, &y, free);

  TEST_ASSERT_EQUAL_UINT(1, plist_size(L));
  TEST_ASSERT_EQUAL_UINT(y, PLIST_GET_UINT(L, 0));
}

void test_replaceDestroy_ShouldReplaceAnElementFromTheListAndKeepTheOldOneIfNoDestructorFunction(void) {
  size_t *x = calloc(1, sizeof(size_t));
  *x = 99;
  size_t y = 0;

  plist_append(L, x);
  plist_replace_and_destroy(L, 0, &y, 0);

  TEST_ASSERT_EQUAL_UINT(1, plist_size(L));
  TEST_ASSERT_EQUAL_UINT(y, PLIST_GET_UINT(L, 0));
  free(x);
}

void test_replaceDestroy_ShouldReplaceANullElementFromTheList(void) {
  size_t *x = 0;
  size_t y = 99;

  plist_append(L, x);
  plist_replace_and_destroy(L, 0, &y, free);

  TEST_ASSERT_EQUAL_UINT(1, plist_size(L));
  TEST_ASSERT_EQUAL_UINT(y, PLIST_GET_UINT(L, 0));
}

void test_removeSelected_ShouldRemoveAParticularElementFromTheList(void) {
  helper_load_default_list();

  TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN, plist_size(L));

  size_t *x = plist_remove_selected(L, helper_is_even);

  TEST_ASSERT_NOT_NULL(x);
  TEST_ASSERT_EQUAL_UINT(*x, data[1]);
  TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN - 1, plist_size(L));
}

void test_removeDestroyingSelected_ShouldRemoveAndDestroyAnElementSelectedFromTheList(void) {
  size_t *x = calloc(1, sizeof(size_t));
  *x = 99;

  helper_load_default_list();
  plist_append(L, x);

  TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN + 1, plist_size(L));

  plist_remove_destroying_selected(L, helper_is_99, free);

  TEST_ASSERT_EQUAL_UINT(DATA_ARRAY_LEN, plist_size(L));
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
  RUN_TEST(test_sort_ShouldNotErrorWithANullList);
  RUN_TEST(test_sort_ShouldHandleAListWithTwoElements);

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
  RUN_TEST(test_clean_ShouldNotErrorOnANullList);

  RUN_TEST(test_filter_ShouldFilterAListAndMatchNoneAndReturnAnEmptyList);
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

  RUN_TEST(test_get_ShouldGetAllElementsFromTheList);
  RUN_TEST(test_get_ShouldGetTheTailElementFromTheList);

  RUN_TEST(test_merge_ShouldMergeTwoLists);
  RUN_TEST(test_merge_ShouldMergeShouldHandleAnEmptyListAsSecondParameter);
  RUN_TEST(test_merge_ShouldHandleAnEmptyListAsFirstParameter);

  RUN_TEST(test_getElements_ShouldReturnANewEmptyList);
  RUN_TEST(test_getElements_ShouldReturnANewLoadedList);

  RUN_TEST(test_getRemoving_ShouldReturnAnEmptyListWhenRemovingFromAnEmptyList);
  RUN_TEST(test_getRemoving_ShouldReturnAnNewLoadedListAndRemoveFromOriginal);
  RUN_TEST(test_getRemoving_ShouldNotErrorOnANullList);

  RUN_TEST(test_remove_ShouldNotRemoveFromAnEmptyList);
  RUN_TEST(test_remove_ShouldRemoveAnElementFromList);
  RUN_TEST(test_remove_ShouldRemoveFirstElementFromList);
  RUN_TEST(test_remove_ShouldRemoveLastElementFromList);
  RUN_TEST(test_remove_ShouldRemoveAllElementsFromList);

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
  RUN_TEST(test_destroy_ShouldFreeAndCleanTheList);
  RUN_TEST(test_destroy_ShouldNotFailOnANullList);

  RUN_TEST(test_destroyAll_ShouldNotFailOnANullList);

  RUN_TEST(test_replaceDestroy_ShouldReplaceAnElementFromTheListAndDestroyTheOldOne);
  RUN_TEST(test_replaceDestroy_ShouldReplaceAnElementFromTheListAndKeepTheOldOneIfNoDestructorFunction);
  RUN_TEST(test_replaceDestroy_ShouldReplaceANullElementFromTheList);

  RUN_TEST(test_removeSelected_ShouldRemoveAParticularElementFromTheList);

  RUN_TEST(test_removeDestroyingSelected_ShouldRemoveAndDestroyAnElementSelectedFromTheList);

  return UNITY_END();
}
