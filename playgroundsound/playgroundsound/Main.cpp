#include <iostream>
#include "Game.h"
#include "WwiseAPI.h"
#include "Wwise_Project/GeneratedSoundBanks/Wwise_IDs.h"
#include <functional>

int main()
{
	WwiseAPI wwiseAPI;
	wwiseAPI.Init();

	wwiseAPI.LoadBank(AK::BANKS::INIT);

	wwiseAPI.LoadBank(AK::BANKS::MAIN);
	Game game;
	game.Init();


	int a = 0;
	while (a >= 0)
	{
		game.Run();
		wwiseAPI.RenderAudio();
	}
	game.DeInit();


	std::cout << "Program terminated" << std::endl;
	wwiseAPI.DeInit();
	return 0;

}