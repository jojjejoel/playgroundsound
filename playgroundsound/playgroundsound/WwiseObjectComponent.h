#pragma once
#include "Component.h"
#include "GO_Vector3.h"

class WwiseObjectComponent : public Component
{
public:
	virtual void Init(GameObject* in_gameObject) override;
	virtual void Update(GameObject* in_gameObject) override;

	void PostEvent(const unsigned int& eventID);

	void RegisterAsListener();

	void RegisterAsDistanceProbe(const unsigned int& listenerID);

	void SetRTPC(const unsigned int& rtpcID, const float& rtpcValue);

	void SetRoomID(const unsigned int& in_akRoomID);

	const GO_Vector3& GetPosition();

private:
	unsigned int akGameObjectID;
	const char* gameObjectName;

	unsigned int currentRoomID;

	GO_Vector3 m_position;
};

