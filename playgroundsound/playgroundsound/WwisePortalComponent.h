#pragma once
#include "Component.h"
#include "GameObject.h"
#include <cstdint>
class WwisePortalComponent : public Component
{
public:
	virtual void Init(GameObject* in_gameObject) override;
	virtual void Update(GameObject* in_gameObject) override;

	void InitPortal(const GameObject* in_gameObject, const uint64_t& in_backRoomID, const uint64_t& in_frontRoomID, const bool& isEnabled = true);
	void InitPortal(const GameObject* in_gameObject, const uint64_t& in_backRoomID);

	void SetPortalEnabled(GameObject* in_gameObject, const bool& isEnabled);

private:
	uint64_t frontRoomID;
	uint64_t backRoomID;
};

