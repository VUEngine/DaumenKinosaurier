/* VUEngine - Virtual Utopia Engine <http://vuengine.planetvb.com/>
 * A universal game engine for the Nintendo Virtual Boy
 *
 * Copyright (C) 2007, 2017 by Jorge Eremiev <jorgech3@gmail.com> and Christian Radke <chris@vr32.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
 * NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef ADJUSTMENT_SCREEN_STATE_H_
#define ADJUSTMENT_SCREEN_STATE_H_


//---------------------------------------------------------------------------------------------------------
//												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <SplashScreenState.h>


//---------------------------------------------------------------------------------------------------------
//												MACROS
//---------------------------------------------------------------------------------------------------------

#define ADJUSTMENT_SCREEN_PALETTES_NUMBER		3 * 3
#define ADJUSTMENT_SCREEN_RHOMBUS_INITIAL_VALUE	-64


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DECLARATION
//---------------------------------------------------------------------------------------------------------

// declare the virtual methods
#define AdjustmentScreenState_METHODS(ClassName)														\
		SplashScreenState_METHODS(ClassName)															\

// declare the virtual methods which are redefined
#define AdjustmentScreenState_SET_VTABLE(ClassName)														\
		SplashScreenState_SET_VTABLE(ClassName)															\
		__VIRTUAL_SET(ClassName, AdjustmentScreenState, enter);											\
		__VIRTUAL_SET(ClassName, AdjustmentScreenState, execute);										\
		__VIRTUAL_SET(ClassName, AdjustmentScreenState, processInput);									\


__CLASS(AdjustmentScreenState);

#define AdjustmentScreenState_ATTRIBUTES																\
		SplashScreenState_ATTRIBUTES																	\
		u8 currentPaletteIndex;																			\


//---------------------------------------------------------------------------------------------------------
//										PUBLIC INTERFACE
//---------------------------------------------------------------------------------------------------------

AdjustmentScreenState AdjustmentScreenState_getInstance(void);

void AdjustmentScreenState_enter(AdjustmentScreenState this, void* owner);
void AdjustmentScreenState_execute(AdjustmentScreenState this, void* owner);


#endif