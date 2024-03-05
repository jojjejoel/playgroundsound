#include <iostream>
#include "RayLibThirdPerson.h"
#include "WwiseAPI.h"
#include "Wwise_IDs.h"
#include <functional>
int main()
{
    RayLibThirdPerson rayLibThird;
    rayLibThird.Init();

    WwiseAPI wwiseAPI;
    wwiseAPI.Init();

	wwiseAPI.LoadBank(AK::BANKS::INIT);

	wwiseAPI.LoadBank(AK::BANKS::MAIN);

	wwiseAPI.AddListener();
    
    wwiseAPI.AddGeometry(rayLibThird.GetWalls()[0]);

    wwiseAPI.AddRoomGeometry(rayLibThird.GetSoundBlockingObjects()[0]);

    wwiseAPI.AddRoom();
    wwiseAPI.AddPortals(*rayLibThird.GetSoundBlockingObjects()[1], *rayLibThird.GetSoundBlockingObjects()[1]);
    wwiseAPI.RenderAudio();
    wwiseAPI.PostEvent(AK::EVENTS::ENERGY, 1);
    GameObject emitterGO;
    emitterGO.SetForward({ 0,0,1 });
    emitterGO.SetUp({ 0,1,0 });
    emitterGO.SetScale({ 1,1,1 });
    emitterGO.SetPosition({ 0,0,0 });
    wwiseAPI.SetCallbackFunctionBeat(std::bind(&RayLibThirdPerson::MusicBeat, &rayLibThird));
    wwiseAPI.SetCallbackFunctionBar(std::bind(&RayLibThirdPerson::MusicBar, &rayLibThird));
    int a = 0;
    while (a >= 0)  
    {
        rayLibThird.Run();
        wwiseAPI.UpdateListenerGO(*rayLibThird.GetCameraGameObject());
        wwiseAPI.UpdateDistanceProbeGO(*rayLibThird.GetPlayerGameObject());
        wwiseAPI.UpdateEmitterGO(emitterGO);
        wwiseAPI.SetPlayerIsInRoom(rayLibThird.IsPlayerInRoom());
        rayLibThird.SetDiffractionPaths(wwiseAPI.GetDiffraction(1));
        //rayLibThird.SetMusicVolume(wwiseAPI.GetRTPCGlobal(AK::GAME_PARAMETERS::MUSIC_VOLUME));
        wwiseAPI.RenderAudio();
    }
    rayLibThird.DeInit();


    std::cout << "Program terminated" << std::endl;
    wwiseAPI.DeInit();
    return 0;

}