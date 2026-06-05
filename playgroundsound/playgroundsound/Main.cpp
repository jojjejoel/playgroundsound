// Must define these BEFORE any includes that bring in Windows.h
#define NOMINMAX
#define NOGDI
#define NOUSER
#define MMNOSOUND  // Prevent multimedia sound function conflicts with raylib

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

	if (const AKRESULT result = wwiseAPI.LoadBank(AK::BANKS::INIT); result != AK_Success) {
		std::cerr << "Failed to load INIT bank: " << result << '\n';
	}
	
	if (const AKRESULT result = wwiseAPI.LoadBank(AK::BANKS::MAIN); result != AK_Success) {
		std::cerr << "Failed to load MAIN bank: " << result << '\n';
	}

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