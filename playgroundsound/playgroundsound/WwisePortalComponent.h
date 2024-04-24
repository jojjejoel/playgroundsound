#pragma once
#include "Component.h"
#include "GameObject.h"
class WwisePortalComponent : public Component
{
public:
	virtual void Init(GameObject* in_gameObject) override;
	virtual void Update(GameObject* in_gameObject) override;

	void InitPortal(const GameObject* in_gameObject, const unsigned int& in_backRoomID, const unsigned int& in_frontRoomID);
	void InitPortal(const GameObject* in_gameObject, const unsigned int& in_backRoomID);

private:
	unsigned int frontRoomID;
	unsigned int backRoomID;
};

