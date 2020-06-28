/* VUEngine - Virtual Utopia Engine <https://www.vuengine.dev>
 * A universal game engine for the Nintendo Virtual Boy
 *
 * © Jorge Eremiev <jorgech3@gmail.com> and Christian Radke <c.radke@posteo.de>, 2007-2020
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
	void constructor(const AnimatedImageSpec* actorSpec, s16 internalId, const char* const name);

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
