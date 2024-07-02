#include "ApplicationTerminator.h"

#include <Windows.h>

void ApplicationTerminator::Kill()
{
    TerminateProcess(GetCurrentProcess(), 0);
}