#include <iostream>
//#include "RayLibTopDown.h"
#include "RayLibThirdPerson.h"
#include "RayLibMaze.h"
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


	AkGameObjectID musicObjectID = 1;
	AkGameObjectID listenerObjectID = 2;
    AkGameObjectID distanceProbeObjectID = 3;


    wwiseAPI.PostEvent(AK::EVENTS::GOOD_OLD_DAYS, musicObjectID, "Test");
	wwiseAPI.AddListener(listenerObjectID, musicObjectID, "ListenerObject", distanceProbeObjectID, "ListenerObject_DistanceProbe");
    
    wwiseAPI.RenderAudio();
    wwiseAPI.AddGeometry(rayLibThird.GetSoundBlockingObjects()[0]);

    wwiseAPI.AddRoom();
    //wwiseAPI.AddPortals(*rayLibThird.GetSoundBlockingObjects()[1], *rayLibThird.GetSoundBlockingObjects()[2]);

    
    int a = 1;
    while (a > 0)  
    {
        rayLibThird.Run();
        wwiseAPI.UpdateGameObject(listenerObjectID, *rayLibThird.GetCameraGameObject());
        wwiseAPI.UpdateGameObject(distanceProbeObjectID, *rayLibThird.GetPlayerGameObject());
        rayLibThird.SetDiffractionPaths(wwiseAPI.GetDiffraction(musicObjectID));
        wwiseAPI.RenderAudio();
    }
    rayLibThird.DeInit();


    std::cout << "Program terminated" << std::endl;
    wwiseAPI.DeInit();
    return 0;

}