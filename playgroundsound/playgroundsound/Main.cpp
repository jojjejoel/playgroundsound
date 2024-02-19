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

	wwiseAPI.AddListener(listenerObjectID, "ListenerObject", distanceProbeObjectID, "ListenerObject_DistanceProbe");

    wwiseAPI.PostEvent(AK::EVENTS::GOOD_OLD_DAYS, musicObjectID, "Test");
    
    


    
    int a = 1;
    while (a > 0)  
    {
        rayLibThird.Run();
        wwiseAPI.UpdateGameObject(listenerObjectID, *rayLibThird.GetCameraGameObject());
        wwiseAPI.UpdateGameObject(distanceProbeObjectID, *rayLibThird.GetPlayerGameObject());
        wwiseAPI.RenderAudio();
    }
    rayLibThird.DeInit();

    
    /*RayLibTopDown rayLibTopDown;
   rayLibTopDown.Run();*/


    //std::cin >> a;


    std::cout << "Program terminated" << std::endl;
    wwiseAPI.DeInit();
    return 0;

}