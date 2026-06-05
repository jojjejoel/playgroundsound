#pragma once
#include "Component.h"
#include "..\GameObject\GameObject.h"
#include <cstdint>

class WwisePortalComponent : public Component {
public:
    ~WwisePortalComponent() override;

    virtual void Init(GameObject* in_gameObject) override;
    virtual void Update(GameObject* in_gameObject) override;

    void InitPortal(const GameObject* in_gameObject, const uint64_t& in_backRoomID, const uint64_t& in_frontRoomID);
    void InitPortal(const GameObject* in_gameObject, const uint64_t& in_backRoomID);

    void TogglePortalState(const GameObject* in_gameObject);

    const bool& GetIsEnabled() const;

private:
    uint64_t frontRoomID = 0;
    uint64_t backRoomID = 0;
    bool isEnabled = true;
};
