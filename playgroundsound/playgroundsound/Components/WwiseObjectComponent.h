#pragma once
#include "Component.h"
#include "..\GameObject\GO_Vector3.h"
#include <functional>

class WwiseObjectComponent : public Component
{
public:
	virtual void Init(GameObject* in_gameObject) override;
	virtual void Update(GameObject* in_gameObject) override;

	void PostEvent(const unsigned int& eventID);
	void PostMusicEvent(const unsigned int&, std::function<void()> callbackFuncBar, std::function<void()> in_callbackFuncBeat);
	void RegisterAsListener();
	void RegisterAsDistanceProbe(const unsigned int& listenerID);
	void SetRTPC(const unsigned int& rtpcID, const float& rtpcValue);
	void SetRoomID(const unsigned int& in_akRoomID);

	const GO_Vector3& GetPosition();

	const float& GetGameParamValueGlobal(const unsigned int& rtpcID);

	std::function<void()> callbackFuntionBar;
	std::function<void()> callbackFuntionBeat;
private:
	unsigned int akGameObjectID;
	const char* gameObjectName;

	unsigned int currentRoomID;

	GO_Vector3 m_position;
};

