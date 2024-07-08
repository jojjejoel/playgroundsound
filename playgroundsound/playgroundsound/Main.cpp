#include <iostream>
#include "Game.h"
#include "WwiseAPI.h"
#include "Wwise_Project/GeneratedSoundBanks/Wwise_IDs.h"
#include <functional>
#include "Misc/ApplicationTerminator.h"

int main()
{
	WwiseAPI wwiseAPI;
	wwiseAPI.Init();

	wwiseAPI.LoadBank(AK::BANKS::INIT);

	wwiseAPI.LoadBank(AK::BANKS::MAIN);
	Game game;
	game.Init();

	bool shouldExit = false;

	while (!shouldExit)
	{
		wwiseAPI.RenderAudio();
		game.Run(shouldExit);
	}
	
	ApplicationTerminator terminator;
	terminator.Kill();

	wwiseAPI.DeInit();
	game.DeInit();
	return 0;

}