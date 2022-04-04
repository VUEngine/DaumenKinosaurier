/**
 * DaumenKinosaurier – VUEdition
 *
 * © Marten Reiß and Christian Radke
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef CUSTOM_CAMERA_EFFECT_MANAGER_H_
#define CUSTOM_CAMERA_EFFECT_MANAGER_H_


//---------------------------------------------------------------------------------------------------------
//												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <CameraEffectManager.h>
#include <Telegram.h>
#include <Entity.h>


//---------------------------------------------------------------------------------------------------------
//												MACROS
//---------------------------------------------------------------------------------------------------------

// delay between camera movements during shaking effect (in ms)
#define SHAKE_OFFSET					8
#define MINIMUM_SHAKE_DELAY				20
#define SHAKE_DELAY_DELTA				60

enum CustomCameraFX
{
	kShake = kCameraLastFX
};


//---------------------------------------------------------------------------------------------------------
// 											CLASS'S DECLARATION
//---------------------------------------------------------------------------------------------------------

singleton class CustomCameraEffectManager : CameraEffectManager
{
	Entity tempFocusEntity;
	Vector3D lastShakeOffset;
	int shakeTimeLeft;

	static CustomCameraEffectManager getInstance();
	override void startEffect(int effect, va_list args);
	override void stopEffect(int effect);
	override bool handleMessage(Telegram telegram);
}


#endif
