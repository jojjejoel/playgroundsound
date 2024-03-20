#pragma once
#include "GoVector3.h"
struct GoTransform {
	GoVector3 position{ 0,0,0 };
	GoVector3 forward{ 0,0,1 };
	GoVector3 up{ 0,1,0 };
	GoVector3 scale{ 1,1,1 };
};
