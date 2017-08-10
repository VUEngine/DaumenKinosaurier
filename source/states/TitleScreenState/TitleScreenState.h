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

#ifndef TITLE_SCREEN_STATE_H_
#define TITLE_SCREEN_STATE_H_


//---------------------------------------------------------------------------------------------------------
// 												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <GameState.h>


//---------------------------------------------------------------------------------------------------------
// 												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

#define	TITLE_SCREEN_MENU_OPTIONS	3

enum TitleScreenMenuOptions
{
	kMenuOptionPlayMovie,
	kMenuOptionAnimationGallery,
	kMenuOptionCredits,
};

enum TitleScreenStateMessages
{
	kMenuSelection = kLastEngineMessage + 1,
	kShowCreditsText,
	kShowCreditsAnimation,
};

enum TitleScreenStateModes
{
	kModeMenu,
	kModePlaying,
};


//---------------------------------------------------------------------------------------------------------
// 											CLASS'S DECLARATION
//---------------------------------------------------------------------------------------------------------

// declare the virtual methods
#define TitleScreenState_METHODS(ClassName)																\
		GameState_METHODS(ClassName)																	\

// declare the virtual methods which are redefined
#define TitleScreenState_SET_VTABLE(ClassName)															\
		GameState_SET_VTABLE(ClassName)																	\
		__VIRTUAL_SET(ClassName, TitleScreenState, enter);												\
		__VIRTUAL_SET(ClassName, TitleScreenState, handleMessage);										\
		__VIRTUAL_SET(ClassName, TitleScreenState, processUserInput);									\

__CLASS(TitleScreenState);

#define TitleScreenState_ATTRIBUTES																		\
		GameState_ATTRIBUTES																			\
		AnimatedInGameEntity cursorEntity;																\
		AnimatedInGameEntity pauseButtonEntity;															\
		AnimatedInGameEntity resumeButtonEntity;														\
		AnimatedInGameEntity backButtonEntity;															\
		AnimatedInGameEntity nextButtonEntity;															\
		bool fadeInComplete;																			\
		bool isPaused;																					\
		u8 currentSelection;																			\
		u8 currentCreditsFrame;																			\
		u8 mode;																						\


//---------------------------------------------------------------------------------------------------------
// 										PUBLIC INTERFACE
//---------------------------------------------------------------------------------------------------------

TitleScreenState TitleScreenState_getInstance(void);
void TitleScreenState_processUserInput(TitleScreenState this, UserInput userInput);
bool TitleScreenState_handleMessage(TitleScreenState this, Telegram telegram);
void TitleScreenState_onAFlipbookByComplete(TitleScreenState this);
void TitleScreenState_playCreditsAnimation(TitleScreenState this);


#endif