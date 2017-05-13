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

#include "putils/pexcept.h"

volatile PEXCEPT_FRAME_T pexceptFrames[PEXCEPT_STACK_ID] = { { .frame = 0 }, };

void Throw(PEXCEPT_T ExceptionID)
{
    unsigned int current = PEXCEPT_GET_ID;
    pexceptFrames[current].exception = ExceptionID;
    if (pexceptFrames[current].frame) {
        longjmp(*pexceptFrames[current].frame, 1);
	}
    PEXCEPT_NO_CATCH_HANDLER(ExceptionID);
}
