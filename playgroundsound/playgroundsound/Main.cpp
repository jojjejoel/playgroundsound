#include <iostream>
#include "Game.h"
#include "WwiseAPI.h"
#include "Wwise_IDs.h"
#include <functional>
#include "AkInstanceIDs.h"

int main()
{
	Game game;
	game.Init();

	WwiseAPI wwiseAPI;
	wwiseAPI.Init();

	wwiseAPI.LoadBank(AK::BANKS::INIT);

	wwiseAPI.LoadBank(AK::BANKS::MAIN);

	wwiseAPI.AddListener();

	wwiseAPI.AddGeometry(game.GetWalls()[0]);

	wwiseAPI.AddRoomGeometry(game.GetSoundBlockingObjects()[0]);

	wwiseAPI.AddRoom();
	wwiseAPI.AddPortals(*game.GetSoundBlockingObjects()[1], *game.GetSoundBlockingObjects()[1]);
	wwiseAPI.RenderAudio();
	wwiseAPI.PostEvent(AK::EVENTS::ENERGY, 1);
	GameObject emitterGO;
	emitterGO.SetScale({ 1,1,1 });
	emitterGO.SetPosition({ 0,0,0 });
	wwiseAPI.SetCallbackFunctionBeat(std::bind(&Game::MusicBeat, &game));
	wwiseAPI.SetCallbackFunctionBar(std::bind(&Game::MusicBar, &game));

	game.SetRtpcFunction(std::bind(&WwiseAPI::SetPlaybackSpeed, &wwiseAPI, std::placeholders::_1));

	wwiseAPI.RegisterGameObject(4, "Light_Bulb");
	wwiseAPI.PostEvent(AK::EVENTS::LIGHT_FLICKER, IDs::lightBulbID);
	GameObject lightBulbGO;
	lightBulbGO.SetPosition({ 10,1,10 });
	int a = 0;
	while (a >= 0)
	{
		game.Run();
		wwiseAPI.UpdateGameObject(IDs::listenerObjectID, *game.GetCameraGameObject());
		wwiseAPI.UpdateGameObject(IDs::distanceProbeObjectID, *game.GetPlayerGameObject());
		wwiseAPI.UpdateGameObject(IDs::musicObjectID, emitterGO);
		wwiseAPI.UpdateGameObject(IDs::lightBulbID, lightBulbGO);
		wwiseAPI.SetPlayerIsInRoom(game.IsPlayerInRoom());
		game.SetDiffractionPaths(wwiseAPI.GetDiffraction(1));
		game.SetLightFlickerValue(wwiseAPI.GetRTPCValueGameObject(AK::GAME_PARAMETERS::LIGHT_FLICKER, IDs::lightBulbID));
		wwiseAPI.RenderAudio();
	}
	game.DeInit();


	std::cout << "Program terminated" << std::endl;
	wwiseAPI.DeInit();
	return 0;

}