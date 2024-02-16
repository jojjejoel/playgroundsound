#include <iostream>
#include "RayLibTopDown.h"
#include "RayLibMaze.h"
#include "WwiseAPI.h"
int main()
{
    WwiseAPI wwiseWrapper;
    wwiseWrapper.Init();

    RaylibMaze rayLibMaze;
    rayLibMaze.Run();

    
    /*RayLibTopDown rayLibTopDown;
   rayLibTopDown.Run();*/


    //std::cin >> a;


    std::cout << "Program terminated" << std::endl;
    return 0;

}