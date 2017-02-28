#include "putils/pexcept.h"

volatile PEXCEPT_FRAME_T pexceptFrames[PEXCEPT_STACK_ID] = { { .pframe = 0 }, };

void Throw(PEXCEPT_T ExceptionID)
{
    unsigned int current = PEXCEPT_GET_ID;
    pexceptFrames[current].exception = ExceptionID;
    if (pexceptFrames[current].pframe) {
        longjmp(*pexceptFrames[current].pframe, 1);
	}
    PEXCEPT_NO_CATCH_HANDLER(ExceptionID);
}
