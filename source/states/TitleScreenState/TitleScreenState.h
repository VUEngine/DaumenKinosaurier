/* VUEngine - Virtual Utopia Engine <http://vuengine.planetvb.com/>
 * A universal game engine for the Nintendo Virtual Boy
 *
 * Copyright (C) 2007, 2018 by Jorge Eremiev <jorgech3@gmail.com> and Christian Radke <chris@vr32.de>
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
#include <AnimatedEntity.h>


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

singleton class TitleScreenState : GameState
{
	AnimatedEntity cursorEntity;
	AnimatedEntity pauseButtonEntity;
	AnimatedEntity resumeButtonEntity;
	AnimatedEntity backButtonEntity;
	AnimatedEntity nextButtonEntity;
	bool fadeInComplete;
	bool isPaused;
	u8 currentSelection;
	u8 currentCreditsFrame;
	u8 mode;

	static TitleScreenState getInstance();
	override void enter(void* owner);
	override void processUserInput(UserInput userInput);
	override bool handleMessage(Telegram telegram);
	void onAFlipbookByComplete();
	void playCreditsAnimation();
}


#endif
