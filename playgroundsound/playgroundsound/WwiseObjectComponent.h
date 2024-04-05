#pragma once
#include "Component.h"
#include "GameObject.h"
#include <AK/SoundEngine/Common/AkTypes.h>
class WwiseObjectComponent : public Component
{
public:
	virtual void Init(GameObject* in_gameObject) override;
	virtual void Update(GameObject* in_gameObject) override;

	void PostEvent(const AkUniqueID& eventID);

	void RegisterAsListener();

	void RegisterAsDistanceProbe(const AkGameObjectID& listenerID);

	void SetRTPC(const AkRtpcID& rtpcID, const AkRtpcValue& rtpcValue);

private:
	AkGameObjectID akGameObjectID;
	const char* gameObjectName;
};

