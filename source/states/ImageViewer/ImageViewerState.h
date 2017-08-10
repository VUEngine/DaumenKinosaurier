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

#ifndef IMAGE_VIEWER_STATE_H_
#define IMAGE_VIEWER_STATE_H_


//---------------------------------------------------------------------------------------------------------
// 												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <GameState.h>


//---------------------------------------------------------------------------------------------------------
//												DEFINES
//---------------------------------------------------------------------------------------------------------

#define	IMAGE_VIEWER_NUMBER_OF_ANIMATIONS		5


//---------------------------------------------------------------------------------------------------------
// 											CLASS'S DECLARATION
//---------------------------------------------------------------------------------------------------------

// declare the virtual methods
#define ImageViewerState_METHODS(ClassName)																\
    	GameState_METHODS(ClassName)											    				    \

// declare the virtual methods which are redefined
#define ImageViewerState_SET_VTABLE(ClassName)															\
        GameState_SET_VTABLE(ClassName)								    								\
        __VIRTUAL_SET(ClassName, ImageViewerState, enter);												\
		__VIRTUAL_SET(ClassName, ImageViewerState, processUserInput);									\

__CLASS(ImageViewerState);

#define ImageViewerState_ATTRIBUTES							        									\
        GameState_ATTRIBUTES																			\
		AnimatedInGameEntity imageEntity;																\
		AnimatedInGameEntity titleEntity;																\
		AnimatedInGameEntity pauseButtonEntity;															\
		AnimatedInGameEntity resumeButtonEntity;														\
		AnimatedInGameEntity backButtonEntity;															\
		AnimatedInGameEntity framesButtonEntity;														\
		bool isPaused;																					\
		bool fadeInComplete;																			\
		u8 currentAnimation;																			\


//---------------------------------------------------------------------------------------------------------
// 										PUBLIC INTERFACE
//---------------------------------------------------------------------------------------------------------

ImageViewerState ImageViewerState_getInstance(void);
void ImageViewerState_processUserInput(ImageViewerState this, UserInput userInput);


#endif