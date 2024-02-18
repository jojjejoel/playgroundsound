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


	AkGameObjectID gameObjectIDTest = 1;
	AkGameObjectID listenerGameObject = 2;

	wwiseAPI.AddListener(listenerGameObject, "Listener");

    wwiseAPI.PostEvent(AK::EVENTS::GOOD_OLD_DAYS, gameObjectIDTest, "Test");
    
    


    
    int a = 1;
    while (a > 0)  
    {
        rayLibThird.Run();
        wwiseAPI.UpdateGameObject(listenerGameObject, *rayLibThird.GetCameraGameObject());
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