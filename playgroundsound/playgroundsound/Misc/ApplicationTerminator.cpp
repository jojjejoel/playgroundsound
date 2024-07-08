#include "ApplicationTerminator.h"

#include <Windows.h>

void ApplicationTerminator::Kill()
{
    // Utilizing ApplicationTerminator as an interim solution for Wwise DeInit issues. 
    // Future improvements will focus on implementing a graceful exit via WwiseAPI.
    TerminateProcess(GetCurrentProcess(), 0);
}