//#include "RaylibWrapper.h"
#include "WwiseWrapper.h"
#include <iostream>

int main(void)
{
     //Wwise Init
    WwiseWrapper wwiseWrapper;

    if (wwiseWrapper.Init())
    {
        std::cout << "WwiseWrapper initialized" << std::endl;
    }
    else
    {
        std::cout << "ERROR" << std::endl;
    }

    AkGameObjectID gameObjectIDTest = 1;

    if (AK::SoundEngine::RegisterGameObj(gameObjectIDTest, "Test") == AK_Success)
    {
        std::cout << "GameObject registered" << std::endl;
    }
    
    if (AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_TEST, gameObjectIDTest) == AK_Success)
    {
        std::cout << "Posted event!" << std::endl;
    }
    else
    {
        std::cout << "Post event FAILED" << std::endl;
    }

    
    int a = 0;

    AK::SoundEngine::RenderAudio();
    

    std::cin >> a;

    std::cout << "Program terminated" << std::endl;
    //RaylibWrapper raylibWrapper;

    //raylibWrapper.Run();
    return 0;
}