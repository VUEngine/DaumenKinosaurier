/**
 * DaumenKinosaurier – VUEdition
 *
 * © Marten Reiß and Christian Radke
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef TitleScreenStATE_H_
#define TitleScreenStATE_H_


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
	kMenuOptionPlayMovie = 0,
	kMenuOptionAnimationGallery,
	kMenuOptionCredits,
};

enum TitleScreenStateMessages
{
	kMessageMenuSelection = kMessageLastEngine + 1,
	kMessageShowCreditsAnimation,
	kMessageShowCreditsText,
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
	AnimatedEntity resumeButtonEntity;
	AnimatedEntity backButtonEntity;
	AnimatedEntity nextButtonEntity;
	bool fadeInComplete;
	bool isPaused;
	uint8 currentSelection;
	uint8 currentCreditsFrame;
	uint8 mode;

	static TitleScreenState getInstance();
	override void enter(void* owner);
	override void processUserInput(UserInput userInput);
	override bool handleMessage(Telegram telegram);
	void onAFlipbookByComplete();
	void playCreditsAnimation();
}


#endif
