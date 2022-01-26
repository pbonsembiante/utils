#ifndef PUTILS_PEXCEPT_CONFIG_H
#define PUTILS_PEXCEPT_CONFIG_H
#endif

#ifndef PEXCEPT_T
#define PEXCEPT_T unsigned int
#endif

extern PEXCEPT_T uncaught_exception;
extern short status;
enum statuses {BEFORE_TRY, SUCCESS_TRY, AFTER_TRY, BEFORE_CATCH};

#ifndef PEXCEPT_NO_CATCH_HANDLER
#include <stdio.h>
#define PEXCEPT_NO_CATCH_HANDLER(e) uncaught_exception = e;
#endif

#ifndef PEXCEPT_HOOK_BEFORE_TRY
#define PEXCEPT_HOOK_BEFORE_TRY status = BEFORE_TRY;
#endif

#ifndef PEXCEPT_HOOK_SUCCESS_TRY
#define PEXCEPT_HOOK_SUCCESS_TRY status = SUCCESS_TRY;
#endif

#ifndef PEXCEPT_HOOK_AFTER_TRY
#define PEXCEPT_HOOK_AFTER_TRY status = AFTER_TRY;
#endif

#ifndef PEXCEPT_HOOK_BEFORE_CATCH
#define PEXCEPT_HOOK_BEFORE_CATCH status = BEFORE_CATCH;
#endif
