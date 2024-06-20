#include "DiffractionManager.h"
#include <AK/SpatialAudio/Common/AkSpatialAudio.h>

std::vector<DiffractionPath> DiffractionManager::GetDiffractionPath(const uint64_t& emitterID)
{
	AkVector64 emitterPos;
	AkVector64 listenerPos;
	AkDiffractionPathInfo akDiffractionPaths[8];
	AkUInt32 numberOfPaths = 8;
	AK::SpatialAudio::QueryDiffractionPaths(emitterID, 0, listenerPos, emitterPos, akDiffractionPaths, numberOfPaths);

	int numLines = 0;
	std::vector<DiffractionPath> diffractionPaths;
	for (AkUInt32 path = 0; path < numberOfPaths; path++)
	{
		DiffractionPath diffractionPath;
		AkDiffractionPathInfo akDiffractionPath = akDiffractionPaths[path];
		for (size_t i = 0; i < akDiffractionPath.nodeCount; i++)
		{
			diffractionPath.nodes[i].x = static_cast<float>(akDiffractionPath.nodes[i].X);
			diffractionPath.nodes[i].y = static_cast<float>(akDiffractionPath.nodes[i].Y);
			diffractionPath.nodes[i].z = static_cast<float>(-akDiffractionPath.nodes[i].Z);
		}
		diffractionPath.emitterPos.x = static_cast<float>(emitterPos.X);
		diffractionPath.emitterPos.y = static_cast<float>(emitterPos.Y);
		diffractionPath.emitterPos.z = static_cast<float>(emitterPos.Z);
		for (size_t i = 0; i < akDiffractionPath.nodeCount; i++)
		{
			diffractionPath.angles[i] = akDiffractionPath.angles[i];
		}
		for (size_t i = 0; i < akDiffractionPath.nodeCount; i++)
		{

			diffractionPath.portals[i] = static_cast<int>(akDiffractionPath.portals[i]);
		}
		for (size_t i = 0; i < akDiffractionPath.nodeCount; i++)
		{

			diffractionPath.rooms[i] = static_cast<int>(akDiffractionPath.rooms[i]);
		}
		diffractionPath.virtualPos.forward.x = -akDiffractionPath.virtualPos.OrientationFront().X;
		diffractionPath.virtualPos.forward.y = -akDiffractionPath.virtualPos.OrientationFront().Y;
		diffractionPath.virtualPos.forward.z = akDiffractionPath.virtualPos.OrientationFront().Z;
		diffractionPath.virtualPos.up.x = akDiffractionPath.virtualPos.OrientationTop().X;
		diffractionPath.virtualPos.up.y = akDiffractionPath.virtualPos.OrientationTop().Y;
		diffractionPath.virtualPos.up.z = -akDiffractionPath.virtualPos.OrientationTop().Z;
		diffractionPath.virtualPos.position.x = static_cast<float>(akDiffractionPath.virtualPos.Position().X);
		diffractionPath.virtualPos.position.y = static_cast<float>(akDiffractionPath.virtualPos.Position().Y);
		diffractionPath.virtualPos.position.z = static_cast<float>(-akDiffractionPath.virtualPos.Position().Z);
		diffractionPath.nodeCount = akDiffractionPath.nodeCount;
		diffractionPath.diffraction = akDiffractionPath.diffraction;
		diffractionPath.totLength = akDiffractionPath.totLength;
		diffractionPath.obstructionValue = akDiffractionPath.obstructionValue;
		diffractionPaths.emplace_back(diffractionPath);
	}
	return diffractionPaths;
}