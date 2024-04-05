#include <iostream>
#include "Game.h"
#include "WwiseAPI.h"
#include "Wwise_IDs.h"
#include <functional>
#include "GameObjectIDs.h"
int main()
{

	WwiseAPI wwiseAPI;
	wwiseAPI.Init();
	wwiseAPI.LoadBank(AK::BANKS::INIT);
	wwiseAPI.LoadBank(AK::BANKS::MAIN);

	Game game;
	game.Init();

	//wwiseAPI.RegisterGameObject(*game.GetGameObject(GUIDs::musicEmitterGO));
	//wwiseAPI.RegisterGameObject(*game.GetGameObject(GUIDs::cameraGO));
	//wwiseAPI.RegisterGameObject(*game.GetGameObject(GUIDs::playerTruckGO));
	//wwiseAPI.AddListener();

	//wwiseAPI.AddGeometry(game.GetWalls()[0]);

	//wwiseAPI.AddRoomGeometry(game.GetGameObject(GUIDs::roomCubeGO));

	//wwiseAPI.AddRoom();
	//wwiseAPI.AddPortal(*game.GetGameObject(GUIDs::portalGO));
	/*wwiseAPI.RenderAudio();
	wwiseAPI.PostEvent(AK::EVENTS::ENERGY, GUIDs::musicEmitterGO);
	wwiseAPI.SetCallbackFunctionBeat(std::bind(&Game::MusicBeat, &game));
	wwiseAPI.SetCallbackFunctionBar(std::bind(&Game::MusicBar, &game));

	game.AssignRtpcFunction(std::bind(&WwiseAPI::SetPlaybackSpeed, &wwiseAPI, std::placeholders::_1));

	wwiseAPI.RegisterGameObject(*game.GetGameObject(GUIDs::lightBulbGO));
	wwiseAPI.PostEvent(AK::EVENTS::LIGHT_FLICKER, GUIDs::lightBulbGO);*/
	
	while (true)
	{
		game.Run();

		/*for (const auto& gameobject : game.GetAllGameObjects())
		{
			wwiseAPI.UpdateGameObject(*gameobject.second);
		}*/

		game.SetDiffractionPaths(wwiseAPI.GetDiffraction(GUIDs::musicEmitterGO));
		game.SetLightFlickerValue(wwiseAPI.GetRTPCValueGameObject(AK::GAME_PARAMETERS::LIGHT_FLICKER, GUIDs::lightBulbGO));
		wwiseAPI.RenderAudio();
	}
	game.DeInit();


	std::cout << "Program terminated" << std::endl;
	wwiseAPI.DeInit();
	return 0;

}