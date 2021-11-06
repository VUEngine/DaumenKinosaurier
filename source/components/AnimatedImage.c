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


//---------------------------------------------------------------------------------------------------------
//												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <AnimatedImage.h>
#include <AnimationState.h>
#include <TitleScreenState.h>
#include <VIPManager.h>
#include <CharSetManager.h>


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DEFINITION
//---------------------------------------------------------------------------------------------------------

extern TextureROMSpec RexLTexture;
extern TextureROMSpec RexRTexture;
extern TextureROMSpec BananaLTexture;
extern TextureROMSpec BananaRTexture;
extern TextureROMSpec VertigoLTexture;
extern TextureROMSpec VertigoRTexture;
extern TextureROMSpec VolcanoLTexture;
extern TextureROMSpec VolcanoRTexture;
extern TextureROMSpec CreditsTextTexture;
extern AnimationDescriptionROMSpec VolcanoAnimation;
extern AnimationDescriptionROMSpec RexAnimation;
extern AnimationDescriptionROMSpec BananaAnimation;
extern AnimationDescriptionROMSpec VertigoAnimation;
extern AnimationDescriptionROMSpec CreditsTextAnimation;



//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// class's constructor
void AnimatedImage::constructor(const AnimatedImageSpec* actorSpec, s16 internalId, const char* const name)
{
	// construct base object
	Base::constructor((AnimatedEntitySpec*)&actorSpec->animatedEntitySpec, internalId, name);
}

// class's destructor
void AnimatedImage::destructor()
{
	// destroy the super object
	// must always be called at the end of the destructor
	Base::destructor();
}

void AnimatedImage::playRexRun(Object eventFirer __attribute__ ((unused)))
{
	AnimationState::playRexRun(AnimationState::getInstance());
}

void AnimatedImage::playCreditsAnimation(Object eventFirer __attribute__ ((unused)))
{
	AnimationState::playCreditsAnimation(AnimationState::getInstance());
}

void AnimatedImage::playCreditsText(Object eventFirer __attribute__ ((unused)))
{
	AnimationState::playCreditsText(AnimationState::getInstance());
}

void AnimatedImage::playBanana(Object eventFirer __attribute__ ((unused)))
{
	AnimationState::playBanana(AnimationState::getInstance());
}

void AnimatedImage::playVertigo(Object eventFirer __attribute__ ((unused)))
{
	AnimationState::playVertigo(AnimationState::getInstance());
}

void AnimatedImage::playVolcanoEnd(Object eventFirer __attribute__ ((unused)))
{
	AnimationState::playVolcanoEnd(AnimationState::getInstance());
}

void AnimatedImage::playCreditsAnimationForTitleScreen(Object eventFirer __attribute__ ((unused)))
{
	TitleScreenState::playCreditsAnimation(TitleScreenState::getInstance());
}

void AnimatedImage::onAFlipbookByComplete(Object eventFirer __attribute__ ((unused)))
{
	TitleScreenState::onAFlipbookByComplete(TitleScreenState::getInstance());
}

void AnimatedImage::changeSpec(AnimationDescription* animationDescription, char* animationName)
{
	// get image entity sprites
	VirtualList entitySprites = Entity::getSprites(this);

	// cycle left and right sprites
	VirtualNode node = VirtualList::begin(entitySprites);
	u8 i = 0;

	VIPManager::disableDrawing(VIPManager::getInstance());

	// rewrite animation description and play loop animation
	for(i = 0; node; node = VirtualNode::getNext(node), i++)
	{
		// get image entity texture
		Texture texture = Sprite::getTexture(VirtualNode::getData(node));

		Texture::setSpec(texture, AnimatedImage::getTextureSpec(this, animationDescription, i));
	}

	AnimatedEntity::setAnimationDescription(this, animationDescription);
	AnimatedEntity::playAnimation(this, animationName);

	// force CHAR memory defragmentation to prevent memory depletion
	CharSetManager::writeCharSets(CharSetManager::getInstance());

	node = VirtualList::begin(entitySprites);

	for(i = 0; node; node = VirtualNode::getNext(node), i++)
	{
		Sprite sprite = Sprite::safeCast(VirtualNode::getData(node));
		Texture texture = Sprite::getTexture(sprite);

		Sprite::render(sprite, 31, false);
		Texture::update(texture);
	}

	VIPManager::enableDrawing(VIPManager::getInstance());
}

TextureSpec* AnimatedImage::getTextureSpec(AnimationDescription* animationDescription, u8 textureNumber)
{
	if(&RexAnimation == animationDescription)
	{
		return (textureNumber == 0) ? (TextureSpec*)&RexLTexture : (TextureSpec*)&RexRTexture;
	}
	else if(&BananaAnimation == animationDescription)
	{
		return (textureNumber == 0) ? (TextureSpec*)&BananaLTexture : (TextureSpec*)&BananaRTexture;
	}
	else if(&VertigoAnimation == animationDescription)
	{
		return (textureNumber == 0) ? (TextureSpec*)&VertigoLTexture : (TextureSpec*)&VertigoRTexture;
	}
	else if(&VolcanoAnimation == animationDescription)
	{
		return (textureNumber == 0) ? (TextureSpec*)&VolcanoLTexture : (TextureSpec*)&VolcanoRTexture;
	}
	else if(&CreditsTextAnimation == animationDescription)
	{
		return (TextureSpec*)&CreditsTextTexture;
	}

	return (textureNumber == 0) ? (TextureSpec*)&VolcanoLTexture : (TextureSpec*)&VolcanoRTexture;
}