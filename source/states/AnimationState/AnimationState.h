/**
 * DaumenKinosaurier – VUEdition
 *
 * © Marten Reiß and Christian Radke
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef AnimationStATE_H_
#define AnimationStATE_H_


//---------------------------------------------------------------------------------------------------------
// 												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <GameState.h>
#include <AnimatedEntity.h>


//---------------------------------------------------------------------------------------------------------
// 												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

enum AnimationSequences
{
	kAnimationSequenceRexScream,
	kAnimationSequenceRexRun,
	kAnimationSequenceBanana,
	kAnimationSequenceVertigo,
	kAnimationSequenceVolcano,
	kAnimationSequenceCredits,
};


//---------------------------------------------------------------------------------------------------------
// 											CLASS'S DECLARATION
//---------------------------------------------------------------------------------------------------------

singleton class AnimationState : GameState
{
	AnimatedEntity resumeButtonEntity;
	AnimatedEntity backButtonEntity;
	bool isPaused;
	uint8 currentSequence;

	static AnimationState getInstance();
	override void enter(void* owner);
	override void processUserInput(UserInput userInput);
    override void execute(void* owner);
    void playBanana();
    void playRexRun();
    void playVertigo();
    void playVolcanoEnd();
    void playCreditsAnimation();
    void playCreditsText();
}


#endif
