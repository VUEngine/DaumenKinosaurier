/**
 * DaumenKinosaurier – VUEdition
 *
 * © Marten Reiß and Christian Radke
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef CreditsStATE_H_
#define CreditsStATE_H_


//---------------------------------------------------------------------------------------------------------
// 												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <GameState.h>
#include <AnimatedEntity.h>


//---------------------------------------------------------------------------------------------------------
// 												DEFINITIONS
//---------------------------------------------------------------------------------------------------------

#define	__CREDITS_LENGTH	250


//---------------------------------------------------------------------------------------------------------
// 											CLASS'S DECLARATION
//---------------------------------------------------------------------------------------------------------

singleton class CreditsState : GameState
{
	AnimatedEntity pauseButtonEntity;
	AnimatedEntity resumeButtonEntity;
	AnimatedEntity backButtonEntity;
	uint16 remaining;
	bool isPaused;
	bool fadeInComplete;

	static CreditsState getInstance();
	override void enter(void* owner);
	override void execute(void* owner);
	override void processUserInput(UserInput userInput);
}


#endif
