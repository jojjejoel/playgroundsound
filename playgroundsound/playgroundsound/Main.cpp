#include <iostream>
#include "RayLibThirdPerson.h"
#include "WwiseAPI.h"
#include "Wwise_IDs.h"
int main()
{
    RayLibThirdPerson rayLibThird;
    rayLibThird.Init();

    WwiseAPI wwiseAPI;
    wwiseAPI.Init();

	wwiseAPI.LoadBank(AK::BANKS::INIT);

	wwiseAPI.LoadBank(AK::BANKS::MAIN);



	wwiseAPI.AddListener();
    
    //wwiseAPI.AddGeometry(rayLibThird.GetSoundBlockingObjects()[0]);
    wwiseAPI.AddGeometry(rayLibThird.GetWalls()[0]);

    wwiseAPI.AddRoomGeometry(rayLibThird.GetSoundBlockingObjects()[0]);
   /* wwiseAPI.AddGeometry(rayLibThird.GetWalls()[1], 0);
    wwiseAPI.AddGeometry(rayLibThird.GetWalls()[2], 0);
    wwiseAPI.AddGeometry(rayLibThird.GetWalls()[3], 0);*/

    wwiseAPI.AddRoom();
    wwiseAPI.AddPortals(*rayLibThird.GetSoundBlockingObjects()[1], *rayLibThird.GetSoundBlockingObjects()[1]);
    wwiseAPI.RenderAudio();
    wwiseAPI.PostEvent(AK::EVENTS::GOOD_OLD_DAYS, 1);
    GameObject emitterGO;
    emitterGO.SetForward({ 0,0,1 });
    emitterGO.SetUp({ 0,1,0 });
    emitterGO.SetScale({ 1,1,1 });
    emitterGO.SetPosition({ 0,0,0 });
    
    int a = 0;
    while (a >= 0)  
    {
        rayLibThird.Run();
        //wwiseAPI.UpdateListenerGO(*rayLibThird.GetCameraGameObject());
        wwiseAPI.UpdateListenerGO(*rayLibThird.GetCameraGameObject());
        wwiseAPI.UpdateDistanceProbeGO(*rayLibThird.GetPlayerGameObject());
        wwiseAPI.UpdateEmitterGO(emitterGO);
        wwiseAPI.SetPlayerIsInRoom(rayLibThird.IsPlayerInRoom());
        rayLibThird.SetDiffractionPaths(wwiseAPI.GetDiffraction(1));
        wwiseAPI.RenderAudio();
    }
    rayLibThird.DeInit();


    std::cout << "Program terminated" << std::endl;
    wwiseAPI.DeInit();
    return 0;

}