#pragma once
#include "Component.h"
#include "..\GameObject\GameObject.h"
#include <cstdint>
class WwisePortalComponent : public Component
{
public:
	~WwisePortalComponent() override;

	virtual void Init(GameObject* in_gameObject) override;
	virtual void Update(GameObject* in_gameObject) override;

	void InitPortal(const GameObject* in_gameObject, const uint64_t& in_backRoomID, const uint64_t& in_frontRoomID);
	void InitPortal(const GameObject* in_gameObject, const uint64_t& in_backRoomID);

	void TogglePortalState(GameObject* in_gameObject);

	const bool& GetIsEnabled() const;

private:
	bool isEnabled = true;
	uint64_t frontRoomID;
	uint64_t backRoomID;
};

