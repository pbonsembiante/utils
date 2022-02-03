#include "unity.h"
#include "putils/pexcept.h"

PEXCEPT_T uncaught_exception = PEXCEPT_NONE;
short status = NOTHING_HAPPENED;

void setUp(void) {}

void tearDown(void) {}

static void _throw_an_exception(void) {
  throw (666);
}

void test_catch_ADefaultCatchHandlerCanBeDefined(void) {
  _throw_an_exception();
  TEST_ASSERT_EQUAL_INT(666, uncaught_exception);
}

void test_hook_BeforeTryRunsAlwaysBeforeThrowingAnException(void) {
  PEXCEPT_T e;
  try {
    TEST_ASSERT_EQUAL_UINT(BEFORE_TRY, status);
    _throw_an_exception();
  } catch (e) {
    TEST_ASSERT_NOT_EQUAL_UINT(BEFORE_TRY, status);
  }
}

void test_hook_SuccessTryRunsWhenTryBlockIsDoneWithoutExceptions(void) {
  PEXCEPT_T e;
  try {
    TEST_ASSERT_NOT_EQUAL_UINT(SUCCESS_TRY, status);
  } catch (e) {
    TEST_ASSERT_EQUAL_UINT(SUCCESS_TRY, status);
  }
}

void test_hook_AfterTryRunsAfterAnException(void) {
  PEXCEPT_T e;
  try {
    TEST_ASSERT_NOT_EQUAL_UINT(AFTER_TRY, status);
    _throw_an_exception();
  } catch (e) {
    TEST_ASSERT_EQUAL_UINT(AFTER_TRY, status);
  }
}

void test_hook_BeforeCatchOnlyRunsBeforeExitingTryBlock(void) {
  PEXCEPT_T e;
  try {
    _throw_an_exception();
  } catch (e) {
      TEST_ASSERT_NOT_EQUAL_UINT(BEFORE_CATCH, status);
  }
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_catch_ADefaultCatchHandlerCanBeDefined);
  RUN_TEST(test_hook_BeforeCatchOnlyRunsBeforeExitingTryBlock);
  RUN_TEST(test_hook_BeforeTryRunsAlwaysBeforeThrowingAnException);
  RUN_TEST(test_hook_SuccessTryRunsWhenTryBlockIsDoneWithoutExceptions);
  RUN_TEST(test_hook_AfterTryRunsAfterAnException);
  return UNITY_END();
}
