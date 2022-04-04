/**
 * DaumenKinosaurier – VUEdition
 *
 * © Marten Reiß and Christian Radke
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef ANIMATED_IMAGE_H_
#define ANIMATED_IMAGE_H_


//---------------------------------------------------------------------------------------------------------
//												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <AnimatedEntity.h>
#include <Body.h>
#include <StateMachine.h>
#include <Clock.h>


//---------------------------------------------------------------------------------------------------------
//											TYPE DEFINITIONS
//---------------------------------------------------------------------------------------------------------

typedef struct AnimatedImageSpec
{
	/// it has an Entity at the beginning
	AnimatedEntitySpec animatedEntitySpec;

} AnimatedImageSpec;

typedef const AnimatedImageSpec AnimatedImageROMSpec;

#define VOLCANO_TEXTURE			0
#define REX_TEXTURE_1			1
#define REX_TEXTURE_2			2
#define BANANA_TEXTURE			3
#define VERTIGO_TEXTURE			4


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DECLARATION
//---------------------------------------------------------------------------------------------------------

/// @ingroup stage-entities
class AnimatedImage : AnimatedEntity
{
	/// @publicsection
	void constructor(const AnimatedImageSpec* actorSpec, int16 internalId, const char* const name);

	void changeSpec(AnimationDescription* animationDescription, char* animationName);

	void playRexRun(Object eventFirer);
	void playCreditsAnimation(Object eventFirer);
	void playCreditsText(Object eventFirer);
	void playBanana(Object eventFirer);
	void playVertigo(Object eventFirer);
	void playVolcanoEnd(Object eventFirer);
	void playCreditsAnimation(Object eventFirer);
	void playCreditsAnimationForTitleScreen(Object eventFirer);
	void onAFlipbookByComplete(Object eventFirer);
}


#endif
