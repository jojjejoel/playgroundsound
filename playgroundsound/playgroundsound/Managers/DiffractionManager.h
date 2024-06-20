#pragma once
#include "..\Components\Component.h"

#include "..\Triangle.h"
#include <cstdint>
#include "..\DiffractionPath.h"
#include <vector>

#define MAX_DIFFRACTIONPATHS 10

class DiffractionManager
{
public:
	std::vector<DiffractionPath> GetDiffractionPath(const uint64_t& emitterID);
};

