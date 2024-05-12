#pragma once
#include "Component.h"
//#include <AK/SoundEngine/Common/AkTypes.h>
//#include <AK/SpatialAudio/Common/AkSpatialAudioTypes.h>

#include "Triangle.h"
#include <cstdint>
#include "DiffractionPath.h"
#include <vector>

#define MAX_DIFFRACTIONPATHS 10

class DiffractionManager
{
public:
	std::vector<DiffractionPath> GetDiffractionPath(const uint64_t& emitterID);
};

