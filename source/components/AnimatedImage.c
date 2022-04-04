/**
 * DaumenKinosaurier – VUEdition
 *
 * © Marten Reiß and Christian Radke
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
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
void AnimatedImage::constructor(const AnimatedImageSpec* actorSpec, int16 internalId, const char* const name)
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
	uint8 i = 0;

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

TextureSpec* AnimatedImage::getTextureSpec(AnimationDescription* animationDescription, uint8 textureNumber)
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