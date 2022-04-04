/**
 * DaumenKinosaurier – VUEdition
 *
 * © Marten Reiß and Christian Radke
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */


//---------------------------------------------------------------------------------------------------------
// 												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <Game.h>
#include <AdjustmentScreenState.h>
#include <TitleScreenState.h>


//---------------------------------------------------------------------------------------------------------
// 										  GAME'S MAIN LOOP
//---------------------------------------------------------------------------------------------------------

int main(void)
{
	// initialize plugins
	SplashScreenState::setNextState(
		SplashScreenState::safeCast(AdjustmentScreenState::getInstance()),
		GameState::safeCast(TitleScreenState::getInstance())
	);

    // start the game
	Game::start(Game::getInstance(), GameState::safeCast(AdjustmentScreenState::getInstance()));

	// end program
	return true;
}
