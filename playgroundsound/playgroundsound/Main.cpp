#include "RaylibWrapper.h"
#include "WwiseWrapper.h"
#include <iostream>

int main(void)
{
    // Wwise Init
    //WwiseWrapper wwiseWrapper;

    //if (wwiseWrapper.Init())
    //{
    //    //std::cout << "WwiseWrapper initialized" << std::endl;
    //}
    
    
    RaylibWrapper raylibWrapper;

    raylibWrapper.Run();
    return 0;
}