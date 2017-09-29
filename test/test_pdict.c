#include <string.h>
#include "putils/pdict.h"
#include "unity.h"

#define DATA_ARRAY_LEN 10

pdict* D = 0;
size_t* data = 0;

void setUp(void) {
    D = pdict_create();
}

void tearDown(void) {
    pdict_destroy(D);
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

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create_NewDictShouldBeEmpty);
    RUN_TEST(test_create_NewDictShouldNotBeNull);

    RUN_TEST(test_size_ShouldBeZeroForANewDict);

    RUN_TEST(test_isEmpty_ShouldReturnTrueOnAnEmptyDict);

    return UNITY_END();
}

/* Pending

test_plist based:

void test_size_SizeOfNewDictWithOneItemShouldBeOne(void) {}
void test_size_ShouldBeEqualToTheNumberOfAddedElements(void) {}
void test_size_ShouldBeZeroForACleanDict(void) {}
void test_size_ShouldNotBeZeroAfterAddingElements(void) {}
void test_add_ShouldAddANewElement(void) {}
void test_append_ShouldAddAnElementToAnEmptyDict(void) {}
void test_get_ShouldGetAValidElementFromTheDict(void) {}
void test_get_ShouldGetAllElementsFromTheDict(void) {}
void test_remove_ShouldRemoveAnElementFromDict(void) {}
void test_remove_ShouldRemoveFirstElementFromDict(void) {}
void test_remove_ShouldRemoveLastElementFromList(void) {}
void test_removeDestroy_ShouldRemoveAndDestroyAnElement(void) {}
void test_isEmpty_ShouldReturnFalseOnAnLoadedDict(void) {}

*/
