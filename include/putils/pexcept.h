#ifndef _PEXCEPT_H_
#define _PEXCEPT_H_
/***************************************************************************
 * Copyright (C) 2016 - 2017 Patricio Bonsembiante. All rights reserved.
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

#include <setjmp.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef PEXCEPT_USE_CONFIG_FILE
#include "pexceptConfig.h"
#endif

#ifndef PEXCEPT_NONE
#define PEXCEPT_NONE    (0x5A5A5A5A)
#endif

#ifndef PEXCEPT_STACK_ID
#define PEXCEPT_STACK_ID    (1)
#endif

#ifndef PEXCEPT_GET_ID
#define PEXCEPT_GET_ID  (0)
#endif

#ifndef PEXCEPT_T
#define PEXCEPT_T   unsigned int
#endif

#ifndef PEXCEPT_NO_CATCH_HANDLER
#define PEXCEPT_NO_CATCH_HANDLER(id)
#endif

#ifndef PEXCEPT_HOOK_START_TRY
#define PEXCEPT_HOOK_START_TRY
#endif
#ifndef PEXCEPT_HOOK_HAPPY_TRY
#define PEXCEPT_HOOK_HAPPY_TRY
#endif
#ifndef PEXCEPT_HOOK_AFTER_TRY
#define PEXCEPT_HOOK_AFTER_TRY
#endif
#ifndef PEXCEPT_HOOK_START_CATCH
#define PEXCEPT_HOOK_START_CATCH
#endif

typedef struct PEXCEPT_FRAME_T PEXCEPT_FRAME_T;
struct PEXCEPT_FRAME_T
{
    jmp_buf *frame;
    PEXCEPT_T volatile exception;
};

extern volatile PEXCEPT_FRAME_T pFrames[];

/* *INDENT-OFF* */
#define Try                                                         \
    {                                                               \
        jmp_buf *PrevFrame, NewFrame;                               \
        unsigned int current = PEXCEPT_GET_ID;                      \
        PrevFrame = pexceptFrames[current].pframe;                  \
        pexceptFrames[current].pframe = (jmp_buf*)(&NewFrame);		\
        pexceptFrames[current].exception = PEXCEPT_NONE;			\
        PEXCEPT_HOOK_START_TRY;                                     \
        if (setjmp(NewFrame) == 0) {                                \
			if (1)

#define Catch(e)                                                    \
            else { }                                                \
            pexceptFrames[current].exception = PEXCEPT_NONE;		\
            PEXCEPT_HOOK_HAPPY_TRY;                                 \
        } else {                                                    \
            (e) = pexceptFrames[current].exception;					\
            (void)(e);                                              \
            PEXCEPT_HOOK_START_CATCH;								\
        }                                                           \
        pexceptFrames[current].pframe = PrevFrame;					\
        PEXCEPT_HOOK_AFTER_TRY;                                     \
    }                                                               \
    if (pexceptFrames[PEXCEPT_GET_ID].exception != PEXCEPT_NONE)

/*!
 * \brief Throw
 * \param ExceptionID
 */
void Throw(PEXCEPT_T ExceptionID);

/*!
 * \brief ExitTry
 *
 */
#define ExitTry() Throw(PEXCEPT_NONE)

#ifdef __cplusplus
}   // extern "C"
#endif

/* *INDENT-ON* */
#endif /* _PEXCEPT_H_ */
