#include <iostream>
#include "Game.h"
#include "WwiseAPI.h"
#include "Wwise_IDs.h"
#include <functional>
#include "AkInstanceIDs.h"
#include "GameObjectIDs.h"
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

	wwiseAPI.AddRoomGeometry(game.GetGameObject(GUIDs::roomCubeGO));

	wwiseAPI.AddRoom();
	wwiseAPI.AddPortal(*game.GetGameObject(GUIDs::portalGO));
	wwiseAPI.RenderAudio();
	wwiseAPI.PostEvent(AK::EVENTS::ENERGY, IDs::musicEmitterGO);
	wwiseAPI.SetCallbackFunctionBeat(std::bind(&Game::MusicBeat, &game));
	wwiseAPI.SetCallbackFunctionBar(std::bind(&Game::MusicBar, &game));

	game.AssignRtpcFunction(std::bind(&WwiseAPI::SetPlaybackSpeed, &wwiseAPI, std::placeholders::_1));

	wwiseAPI.RegisterGameObject(IDs::lightBulbGO, "Light_Bulb");
	wwiseAPI.PostEvent(AK::EVENTS::LIGHT_FLICKER, IDs::lightBulbGO);
	

	int a = 0;
	while (a >= 0)
	{
		game.Run();
		wwiseAPI.UpdateGameObject(IDs::cameraGO, *game.GetGameObject(GUIDs::cameraGO));
		wwiseAPI.UpdateGameObject(IDs::playerTruckGO, *game.GetGameObject(GUIDs::playerTruckGO));
		wwiseAPI.UpdateGameObject(IDs::musicEmitterGO, *game.GetGameObject(GUIDs::musicEmitterGO));
		wwiseAPI.UpdateGameObject(IDs::lightBulbGO, *game.GetGameObject(GUIDs::lightBulbGO));
		wwiseAPI.SetGameObjectIsInRoom(IDs::cameraGO, game.IsGameObjectInRoom(game.GetGameObject(GUIDs::cameraGO)));
		wwiseAPI.SetGameObjectIsInRoom(IDs::playerTruckGO, game.IsGameObjectInRoom(game.GetGameObject(GUIDs::playerTruckGO)));
		wwiseAPI.SetGameObjectIsInRoom(IDs::lightBulbGO, game.IsGameObjectInRoom(game.GetGameObject(GUIDs::lightBulbGO)));
		game.SetDiffractionPaths(wwiseAPI.GetDiffraction(IDs::musicEmitterGO));
		game.SetLightFlickerValue(wwiseAPI.GetRTPCValueGameObject(AK::GAME_PARAMETERS::LIGHT_FLICKER, IDs::lightBulbGO));
		wwiseAPI.RenderAudio();
	}
	game.DeInit();


	std::cout << "Program terminated" << std::endl;
	wwiseAPI.DeInit();
	return 0;

}