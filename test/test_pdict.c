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

#include "putils/pdict.h"
#include "unity.h"
#include <stdlib.h>
#include <string.h>

#define DICT_DATA_LEN 10
#define KEYS_LEN 10

pdict *D = 0;
size_t *data = 0;
char (*keys)[KEYS_LEN];

void setUp(void) { D = pdict_create(); }

void tearDown(void) { pdict_destroy(D); }

void putIntoDict(size_t items_count) {
  keys = calloc(items_count, sizeof(char[KEYS_LEN]));
  data = calloc(items_count, sizeof(size_t));

  for (size_t i = 0; i < items_count; ++i) {
    snprintf(keys[i], KEYS_LEN, "%c", (int) i + 65);
    data[i] = i + 1;
    pdict_put(D, keys[i], &data[i]);
  }
}

void fillDict(void) {
  putIntoDict(DICT_DATA_LEN);
}

void free_keys_data() {
  free(keys);
  free(data);
}

void test_create_NewDictShouldBeEmpty(void) {
  TEST_ASSERT_TRUE(pdict_is_empty(D));
}

void test_create_NewDictShouldNotBeNull(void) { TEST_ASSERT_NOT_NULL(D); }

void test_size_ShouldBeZeroForANewDict(void) {
  TEST_ASSERT_EQUAL_UINT(pdict_size(D), 0);
}

void test_isEmpty_ShouldReturnTrueOnAnEmptyDict(void) {
  TEST_ASSERT_TRUE(pdict_is_empty(D));
}

void test_size_SizeOfNewDictWithOneItemShouldBeOne(void) {
  char *_key = malloc(KEYS_LEN);
  size_t _data = 0;
  snprintf(_key, KEYS_LEN, "%s", "0_key");
  pdict_put(D, _key, &_data);
  TEST_ASSERT_EQUAL_UINT(pdict_size(D), 1);
  free(_key);
}

void test_size_ShouldBeEqualToTheNumberOfAddedElements(void) {
  fillDict();
  TEST_ASSERT_EQUAL_UINT(pdict_size(D), DICT_DATA_LEN);
  free_keys_data();
}

void test_size_ShouldBeZeroForACleanDict(void) {
  fillDict();
  pdict_clean(D);
  TEST_ASSERT_EQUAL_UINT(pdict_size(D), 0);
  free_keys_data();
}

void test_size_ShouldNotBeZeroAfterAddingElements(void) {
  fillDict();
  TEST_ASSERT_TRUE(pdict_size(D) != 0);
  free_keys_data();
}

void test_size_FullDictShouldBeResized(void) {
  putIntoDict(PDICT_INITIAL_SIZE);
  TEST_ASSERT_EQUAL_UINT(pdict_size(D), PDICT_INITIAL_SIZE);
  free_keys_data();
}

void test_add_ShouldAddANewElement(void) {
  char *_key = malloc(KEYS_LEN);
  size_t _data = 99;
  snprintf(_key, KEYS_LEN, "%s", "99_key");
  TEST_ASSERT_TRUE(pdict_is_empty(D));
  pdict_put(D, _key, &_data);
  TEST_ASSERT_EQUAL_UINT(1, pdict_size(D));
  free(_key);
}

void test_append_ShouldAddAnElementToAnEmptyDict(void) {
  char *_key = malloc(KEYS_LEN);
  snprintf(_key, KEYS_LEN, "%s", "1_key");
  size_t _data = 1;
  TEST_ASSERT_TRUE(pdict_is_empty(D));
  pdict_put(D, _key, &_data);
  TEST_ASSERT_FALSE(pdict_is_empty(D));
  free(_key);
}

void test_isEmpty_ShouldReturnFalseOnALoadedDict(void) {
  fillDict();
  TEST_ASSERT_FALSE(pdict_is_empty(D));
  free_keys_data();
}

void test_get_ShouldGetAllValuesFromTheDict(void) {
  fillDict();
  for (size_t i = 0; i < DICT_DATA_LEN - 1; ++i) {
    void *element = pdict_get_value(D, keys[i]);
    TEST_ASSERT_NOT_NULL(element);
    TEST_ASSERT_EQUAL_UINT(element, &data[i]);
  }
  free_keys_data();
}

void test_get_ShouldGetAllEntriesFromTheDictOneByOne(void) {
  fillDict();
  for (size_t i = 0; i < DICT_DATA_LEN; ++i) {
    char *key_to_test = keys[i];
    void *element = pdict_get_value(D, keys[i]);
    pdict_entry pair = pdict_get(D, key_to_test);
    TEST_ASSERT_TRUE(strcmp(key_to_test, pair.key) == 0);
    TEST_ASSERT_EQUAL_UINT(element, pair.value);
  }
  free_keys_data();
}

void test_getAll_ShouldGetAllEntriesFromTheDict(void) {
  fillDict();

  pdict_entries entries = pdict_get_all(D);

  TEST_ASSERT_EQUAL_UINT(pdict_size(D), entries.count);

  for (size_t i = 0; i < entries.count; ++i) {
    size_t *value = pdict_get_value(D, entries.entries[i].key);
    TEST_ASSERT_EQUAL(value, entries.entries[i].value);
  }

  free(entries.entries);
  free_keys_data();
}

void test_getAll_ShouldNotErrorWithANullDict(void) {
  pdict_entries entries = pdict_get_all(0);

  TEST_ASSERT_NULL(entries.entries);
  TEST_ASSERT_EQUAL_UINT(0, entries.count);
}

void test_remove_ShouldRemoveAllElementsFromDict(void) {
  fillDict();
  for (int i = 0; i < DICT_DATA_LEN; i++) {
    TEST_ASSERT_EQUAL_UINT(DICT_DATA_LEN - i, pdict_size(D));
    pdict_remove(D, keys[i]);
  }
  free_keys_data();
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_create_NewDictShouldBeEmpty);
  RUN_TEST(test_create_NewDictShouldNotBeNull);

  RUN_TEST(test_size_ShouldBeZeroForANewDict);
  RUN_TEST(test_size_ShouldBeEqualToTheNumberOfAddedElements);
  RUN_TEST(test_size_SizeOfNewDictWithOneItemShouldBeOne);
  RUN_TEST(test_size_ShouldBeZeroForACleanDict);
  RUN_TEST(test_size_ShouldNotBeZeroAfterAddingElements);
  RUN_TEST(test_size_FullDictShouldBeResized);

  RUN_TEST(test_isEmpty_ShouldReturnTrueOnAnEmptyDict);
  RUN_TEST(test_isEmpty_ShouldReturnFalseOnALoadedDict);

  RUN_TEST(test_add_ShouldAddANewElement);

  RUN_TEST(test_append_ShouldAddAnElementToAnEmptyDict);

  RUN_TEST(test_get_ShouldGetAllValuesFromTheDict);
  RUN_TEST(test_get_ShouldGetAllEntriesFromTheDictOneByOne);
  RUN_TEST(test_getAll_ShouldGetAllEntriesFromTheDict);
  RUN_TEST(test_getAll_ShouldNotErrorWithANullDict);

  RUN_TEST(test_remove_ShouldRemoveAllElementsFromDict);
  return UNITY_END();
}

/* Pending

test_plist based:

void test_removeDestroy_ShouldRemoveAndDestroyAnElement(void) {}
*/
