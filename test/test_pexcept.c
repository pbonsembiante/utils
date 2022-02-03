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

#include "unity.h"
#include "putils/pexcept.h"

void setUp(void) {}

void tearDown(void) {}

static void _throw_an_exception(void) {
  throw (666);
}

void test_try_ShouldBeAbleToStopAtAnyPoint(void) {
  PEXCEPT_T e;
  try {
    exit_try();
    TEST_FAIL_MESSAGE("This point should be not reached due to use of exit_try()");
  } catch (e) {
    TEST_FAIL_MESSAGE("This point should be not reached due to use of exit_try()");
  }
}

void test_catch_ShouldCatchExceptionsInNestedFrames(void) {
  PEXCEPT_T e;
  try {
    _throw_an_exception();
  } catch (e) {
    TEST_ASSERT_EQUAL_UINT(e, 666);
  }
}

void test_catch_ShouldCatchThrewException(void) {
  PEXCEPT_T e;
  try {
    throw (666);
  } catch (e) {
    TEST_ASSERT_EQUAL_UINT(e, 666);
  }
}

void test_catch_ShouldNotRunWhenThereIsNoException(void) {
  PEXCEPT_T e;
  try {}
  catch (e) {
    TEST_FAIL_MESSAGE("Catch shouldn't be executed when exceptions weren't raised");
  }
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_catch_ShouldCatchThrewException);
  RUN_TEST(test_catch_ShouldNotRunWhenThereIsNoException);
  RUN_TEST(test_try_ShouldBeAbleToStopAtAnyPoint);
  RUN_TEST(test_catch_ShouldCatchExceptionsInNestedFrames);
  return UNITY_END();
}
