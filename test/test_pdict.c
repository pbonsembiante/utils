#include <string.h>
#include <stdlib.h>
#include "putils/pdict.h"
#include "unity.h"

#define DICT_DATA_LEN 10
#define KEYS_LEN 10

pdict *D = 0;
size_t *data = 0;
char (*keys)[KEYS_LEN];

void setUp(void) {
    D = pdict_create();
}

void tearDown(void) {
    pdict_destroy(D);
}

void loadDict(void) {
    keys = malloc(sizeof(char[DICT_DATA_LEN][KEYS_LEN]));
    data = calloc(1, DICT_DATA_LEN * sizeof(size_t));
    for ( size_t i = 0; i < DICT_DATA_LEN; ++i) {
        snprintf(keys[i], KEYS_LEN, "%c", (int) i + 65);
        data[i] = i + 1;
        pdict_put(D, keys[i], &data[i]);
    }
}

void free_keys_data() {
    free(keys);
    free(data);
}

void test_create_NewDictShouldBeEmpty(void) {
    TEST_ASSERT_TRUE(pdict_is_empty(D));
}

void test_create_NewDictShouldNotBeNull(void) {
    TEST_ASSERT_NOT_NULL(D);
}

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
    loadDict();
    TEST_ASSERT_EQUAL_UINT(pdict_size(D), DICT_DATA_LEN);
    free_keys_data();
}

void test_size_ShouldBeZeroForACleanDict(void) {
    loadDict();
    pdict_clean(D);
    TEST_ASSERT_EQUAL_UINT(pdict_size(D), 0);
    free_keys_data();
}

void test_size_ShouldNotBeZeroAfterAddingElements(void) {
    loadDict();
    TEST_ASSERT_TRUE(pdict_size(D) != 0);
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
    loadDict();
    TEST_ASSERT_FALSE(pdict_is_empty(D));
    free_keys_data();
}

void test_get_ShouldGetAllValuesFromTheDict(void) {
    loadDict();
    for (size_t i = 0; i < DICT_DATA_LEN - 1; ++i) {
        void *element = pdict_get_value(D, keys[i]);
        TEST_ASSERT_NOT_NULL(element);
        TEST_ASSERT_EQUAL_UINT(element, &data[i]);
    }
    free_keys_data();
}

void test_get_ShouldGetAllPmapsFromTheDict(void) {
    loadDict();
    for (size_t i = 0; i < DICT_DATA_LEN; ++i) {
        char *key_to_test = keys[i];
        void *element = pdict_get_value(D, keys[i]);
        pmap pair = pdict_get(D, key_to_test);
        TEST_ASSERT_TRUE(strcmp(key_to_test, pair.key) == 0);
        TEST_ASSERT_EQUAL_UINT(element, pair.value);

    }
    free_keys_data();
}

void test_remove_ShouldRemoveAllElementsFromDict(void) {
    loadDict();
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

    RUN_TEST(test_isEmpty_ShouldReturnTrueOnAnEmptyDict);
    RUN_TEST(test_isEmpty_ShouldReturnFalseOnALoadedDict);

    RUN_TEST(test_add_ShouldAddANewElement);

    RUN_TEST(test_append_ShouldAddAnElementToAnEmptyDict);

    RUN_TEST(test_get_ShouldGetAllValuesFromTheDict);
    RUN_TEST(test_get_ShouldGetAllPmapsFromTheDict);

    RUN_TEST(test_remove_ShouldRemoveAllElementsFromDict);
    return UNITY_END();
}

/* Pending

test_plist based:

void test_removeDestroy_ShouldRemoveAndDestroyAnElement(void) {}
*/
