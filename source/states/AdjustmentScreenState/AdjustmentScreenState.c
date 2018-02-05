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

#include <Game.h>
#include <Screen.h>
#include <SoundManager.h>
#include <MessageDispatcher.h>
#include <AdjustmentScreenState.h>
#include <TitleScreenState.h>
#include <DirectDraw.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern StageROMDef ADJUSTMENT_SCREEN_STAGE_ST;
extern const u16 BLIP_SND[];

const u8 ADJUSTMENT_SCREEN_PALETTES[ADJUSTMENT_SCREEN_PALETTES_NUMBER] =
{
	0b01011001,	0b01011001,	0b01011001,
	0b01100101,	0b01100101,	0b01100101,
	0b10010101,	0b10010101,	0b10010101,
};


//---------------------------------------------------------------------------------------------------------
//												PROTOTYPES
//---------------------------------------------------------------------------------------------------------

static void AdjustmentScreenState_destructor(AdjustmentScreenState this);
static void AdjustmentScreenState_constructor(AdjustmentScreenState this);
static void AdjustmentScreenState_processInput(AdjustmentScreenState this, u32 pressedKey);
void AdjustmentScreenState_rhombusEmitterPostProcessingEffect(u32 currentDrawingFrameBufferSet, SpatialObject spatialObject);


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DEFINITION
//---------------------------------------------------------------------------------------------------------

__CLASS_DEFINITION(AdjustmentScreenState, SplashScreenState);
__SINGLETON_DYNAMIC(AdjustmentScreenState);


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// class's constructor
static void __attribute__ ((noinline)) AdjustmentScreenState_constructor(AdjustmentScreenState this)
{
	__CONSTRUCT_BASE(SplashScreenState);

	SplashScreenState_setNextState(__SAFE_CAST(SplashScreenState, this), __SAFE_CAST(GameState, TitleScreenState_getInstance()));
	this->stageDefinition = (StageDefinition*)&ADJUSTMENT_SCREEN_STAGE_ST;

	this->currentPaletteIndex = 0;
	_vipRegisters[__GPLT2] = ADJUSTMENT_SCREEN_PALETTES[this->currentPaletteIndex];
}

// class's destructor
static void AdjustmentScreenState_destructor(AdjustmentScreenState this)
{
	// destroy base
	__SINGLETON_DESTROY;
}

// state's enter
void AdjustmentScreenState_enter(AdjustmentScreenState this, void* owner)
{
	// call base
	__CALL_BASE_METHOD(SplashScreenState, enter, this, owner);

	// move the printing area out of the visible screen to save CPU resources
	Printing_setWorldCoordinates(Printing_getInstance(), __SCREEN_WIDTH, __SCREEN_HEIGHT);

	// add rhombus effect
	VIPManager_pushBackPostProcessingEffect(VIPManager_getInstance(), AdjustmentScreenState_rhombusEmitterPostProcessingEffect, NULL);

	// play start-up sound
	VBVec3D position = {192, 112, 0};
	SoundManager_playFxSound(SoundManager_getInstance(), BLIP_SND, position);
}

static void AdjustmentScreenState_processInput(AdjustmentScreenState this, u32 pressedKey __attribute__ ((unused)))
{
	SplashScreenState_loadNextState(__SAFE_CAST(SplashScreenState, this));
}

void AdjustmentScreenState_execute(AdjustmentScreenState this, void* owner __attribute__ ((unused)))
{
	ASSERT(this, "AdjustmentScreenState::execute: null this");

	this->currentPaletteIndex++;
	if(this->currentPaletteIndex == ADJUSTMENT_SCREEN_PALETTES_NUMBER)
	{
		this->currentPaletteIndex = 0;
	}

	_vipRegisters[__GPLT2] = ADJUSTMENT_SCREEN_PALETTES[this->currentPaletteIndex];
}

void AdjustmentScreenState_rhombusEmitterPostProcessingEffect(u32 currentDrawingFrameBufferSet __attribute__ ((unused)), SpatialObject spatialObject __attribute__ ((unused)))
{
	// runtime working variables
	// negative value to achieve an initial delay
	static int radius = ADJUSTMENT_SCREEN_RHOMBUS_INITIAL_VALUE;

	// increase radius in each cycle
	radius += 2;

	if(radius < 68)
	{
		return;
	}
	else if(radius > 300)
	{
		// reset radius when reaching a certain length
		radius = ADJUSTMENT_SCREEN_RHOMBUS_INITIAL_VALUE;
		return;
	}

	// draw rhombus around object with given radius
	DirectDraw directDraw = DirectDraw_getInstance();

	DirectDraw_drawLine(
		directDraw,
		(VBVec2D) {__I_TO_FIX19_13(192 - radius),	__I_TO_FIX19_13(112),			0, -((radius + ADJUSTMENT_SCREEN_RHOMBUS_INITIAL_VALUE)>>5)},
		(VBVec2D) {__I_TO_FIX19_13(192),			__I_TO_FIX19_13(112 - radius),	0, -((radius + ADJUSTMENT_SCREEN_RHOMBUS_INITIAL_VALUE)>>5)},
		__COLOR_BRIGHT_RED
	);

	DirectDraw_drawLine(
		directDraw,
		(VBVec2D) {__I_TO_FIX19_13(192 + radius),	__I_TO_FIX19_13(112),			0, -((radius + ADJUSTMENT_SCREEN_RHOMBUS_INITIAL_VALUE)>>5)},
		(VBVec2D) {__I_TO_FIX19_13(192),			__I_TO_FIX19_13(112 - radius),	0, -((radius + ADJUSTMENT_SCREEN_RHOMBUS_INITIAL_VALUE)>>5)},
		__COLOR_BRIGHT_RED
	);

	DirectDraw_drawLine(
		directDraw,
		(VBVec2D) {__I_TO_FIX19_13(192 + radius),	__I_TO_FIX19_13(112),			0, -((radius + ADJUSTMENT_SCREEN_RHOMBUS_INITIAL_VALUE)>>5)},
		(VBVec2D) {__I_TO_FIX19_13(192),			__I_TO_FIX19_13(112 + radius),	0, -((radius + ADJUSTMENT_SCREEN_RHOMBUS_INITIAL_VALUE)>>5)},
		__COLOR_BRIGHT_RED
	);

	DirectDraw_drawLine(
		directDraw,
		(VBVec2D) {__I_TO_FIX19_13(192 - radius),	__I_TO_FIX19_13(112),			0, -((radius + ADJUSTMENT_SCREEN_RHOMBUS_INITIAL_VALUE)>>5)},
		(VBVec2D) {__I_TO_FIX19_13(192),			__I_TO_FIX19_13(112 + radius),	0, -((radius + ADJUSTMENT_SCREEN_RHOMBUS_INITIAL_VALUE)>>5)},
		__COLOR_BRIGHT_RED
	);
}
