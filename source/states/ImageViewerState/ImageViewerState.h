/**
 * DaumenKinosaurier – VUEdition
 *
 * © Marten Reiß and Christian Radke
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef ImageViewerStATE_H_
#define ImageViewerStATE_H_


//---------------------------------------------------------------------------------------------------------
// 												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <GameState.h>
#include <AnimatedImage.h>


//---------------------------------------------------------------------------------------------------------
//												DEFINES
//---------------------------------------------------------------------------------------------------------

#define	IMAGE_VIEWER_NUMBER_OF_ANIMATIONS		5


//---------------------------------------------------------------------------------------------------------
// 											CLASS'S DECLARATION
//---------------------------------------------------------------------------------------------------------

singleton class ImageViewerState : GameState
{
	AnimatedImage imageEntity;
	AnimatedEntity titleEntity;
	AnimatedEntity pauseButtonEntity;
	AnimatedEntity resumeButtonEntity;
	AnimatedEntity backButtonEntity;
	AnimatedEntity framesButtonEntity;
	bool isPaused;
	bool fadeInComplete;
	uint8 currentAnimation;

	static ImageViewerState getInstance();
	override void enter(void* owner);
	override void processUserInput(UserInput userInput);
}


#endif
