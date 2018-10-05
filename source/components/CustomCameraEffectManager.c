/* VUEngine - Virtual Utopia Engine <http://vuengine.planetvb.com/>
 * A universal game engine for the Nintendo Virtual Boy
 *
 * Copyright (C) 2007, 2018 by Jorge Eremiev<jorgech3@gmail.com> and Christian Radke <chris@vr32.de>
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

#include <CustomCameraEffectManager.h>
#include <Camera.h>
#include <MessageDispatcher.h>
#include <Actor.h>
#include <Game.h>
#include <PhysicalWorld.h>
#include <Utilities.h>
#include <VIPManager.h>

#include <debugConfig.h>


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DEFINITION
//---------------------------------------------------------------------------------------------------------

__CLASS_DEFINITION(CustomCameraEffectManager, CameraEffectManager);
__CLASS_FRIEND_DEFINITION(Camera);


//---------------------------------------------------------------------------------------------------------
//												PROTOTYPES
//---------------------------------------------------------------------------------------------------------

static void CustomCameraEffectManager_constructor(CustomCameraEffectManager this);
static void CustomCameraEffectManager_FXShakeStart(CustomCameraEffectManager this, u16 duration);
void CustomCameraEffectManager_FXShakeStop(CustomCameraEffectManager this);
static void CustomCameraEffectManager_onCameraShake(CustomCameraEffectManager this);


//---------------------------------------------------------------------------------------------------------
//												GLOBALS
//---------------------------------------------------------------------------------------------------------

static Camera _camera = NULL;


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// it's a singleton
__SINGLETON(CustomCameraEffectManager);

// class's constructor
static void __attribute__ ((noinline)) CustomCameraEffectManager_constructor(CustomCameraEffectManager this)
{
	ASSERT(this, "CustomCameraEffectManager::constructor: null this");

	// construct base object
	__CONSTRUCT_BASE(CameraEffectManager);

	this->lastShakeOffset.x = 0;
	this->lastShakeOffset.y = 0;
	this->lastShakeOffset.z = 0;

	this->shakeTimeLeft = 0;

	_camera = Camera_getInstance();

	NM_ASSERT(_camera, "CustomCameraEffectManager::constructor: null _camera");
}

// class's destructor
void CustomCameraEffectManager_destructor(CustomCameraEffectManager this)
{
	ASSERT(this, "CustomCameraEffectManager::destructor: null this");

	// destroy base
	__SINGLETON_DESTROY;
}

void CustomCameraEffectManager_startEffect(CustomCameraEffectManager this, int effect, va_list args)
{
	ASSERT(this, "CustomCameraEffectManager::startEffect: null this");

	switch(effect)
	{
		case kShake:

			CustomCameraEffectManager_FXShakeStart(this, va_arg(args, int));
			break;

		default:

			CameraEffectManager_startEffect(CameraEffectManager_getInstance(), effect, args);
			break;
	}
}

void CustomCameraEffectManager_stopEffect(CustomCameraEffectManager this, int effect)
{
	ASSERT(this, "CustomCameraEffectManager::stopEffect: null this");

	switch(effect)
	{
		case kShake:

			CustomCameraEffectManager_FXShakeStop(this);
			break;

		default:

			__CALL_BASE_METHOD(CameraEffectManager, stopEffect, this, effect);
			break;
	}
}

bool CustomCameraEffectManager_handleMessage(CustomCameraEffectManager this, Telegram telegram)
{
	ASSERT(this, "CustomCameraEffectManager::handleMessage: null this");

	switch(Telegram_getMessage(telegram))
	{
		case kShake:

			CustomCameraEffectManager_onCameraShake(this);
			break;
	}

	return false;
}

// start shaking the camera
static void CustomCameraEffectManager_FXShakeStart(CustomCameraEffectManager this, u16 duration)
{
	ASSERT(this, "CustomCameraEffectManager::FXShakeStart: null this");

	// set desired fx duration
	this->shakeTimeLeft = duration;

	this->lastShakeOffset.x = __PIXELS_TO_METERS(SHAKE_OFFSET);

	// discard pending messages from previously started fx
	MessageDispatcher_discardDelayedMessagesFromSender(MessageDispatcher_getInstance(), __SAFE_CAST(Object, this), kShake);

	// instantly send message to self to start fx
	MessageDispatcher_dispatchMessage(0, __SAFE_CAST(Object, this), __SAFE_CAST(Object, this), kShake, NULL);
}

// stop shaking the _camera
void CustomCameraEffectManager_FXShakeStop(CustomCameraEffectManager this)
{
	ASSERT(this, "CustomCameraEffectManager::FXShakeStop: null this");

	this->shakeTimeLeft = 0;
}

// shake the camera
static void CustomCameraEffectManager_onCameraShake(CustomCameraEffectManager this)
{
	ASSERT(this, "CustomCameraEffectManager::onCameraShake: null this");

	// stop if no shaking time left
	if(this->shakeTimeLeft == 0)
	{
		// if needed, undo last offset
		if(this->lastShakeOffset.x != 0 || this->lastShakeOffset.y != 0)
		{
			this->lastShakeOffset.x = 0;
		}

		// center camera
		Vector3D position = {0, 0, 0};
		Camera_setPosition(_camera, position);

		return;
	}

	long seed = Utilities_randomSeed();

	int nextShakeDelay = MINIMUM_SHAKE_DELAY + Utilities_random(seed, __ABS(SHAKE_DELAY_DELTA));

	// subtract time until next shake
	this->shakeTimeLeft = (this->shakeTimeLeft <= nextShakeDelay) ? 0 : this->shakeTimeLeft - nextShakeDelay;

	// new offset
	this->lastShakeOffset.x = -this->lastShakeOffset.x;

	// move camera a bit
	Camera_move(_camera, this->lastShakeOffset, false);

	// send message for next camera movement
	MessageDispatcher_dispatchMessage(nextShakeDelay, __SAFE_CAST(Object, this), __SAFE_CAST(Object, this), kShake, NULL);
}