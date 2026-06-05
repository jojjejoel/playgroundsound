#include "ApplicationTerminator.h"

// Prevent Windows.h from defining symbols that conflict with raylib
#define NOMINMAX
#define NOGDI
#define NOUSER
#include <Windows.h>
#undef CloseWindow
#undef ShowCursor

void ApplicationTerminator::Kill()
{
    // Utilizing ApplicationTerminator as an interim solution for Wwise DeInit issues. 
    // Future improvements will focus on implementing a graceful exit via WwiseAPI.
    TerminateProcess(GetCurrentProcess(), 0);
}