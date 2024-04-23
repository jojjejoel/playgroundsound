#pragma once
#include "Component.h"
#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/SpatialAudio/Common/AkSpatialAudioTypes.h>
class WwisePortalComponent : public Component
{
public:
	virtual void Init(GameObject* in_gameObject) override;
	virtual void Update(GameObject* in_gameObject) override;

private:
	unsigned int frontRoomID;
	unsigned int backRoomID;
};

