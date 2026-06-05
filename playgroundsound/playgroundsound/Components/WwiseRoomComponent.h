#pragma once
#include "Component.h"

#include "..\Triangle.h"
#include "..\GameObject\GameObject.h"

#include "..\GameObject\GO_BoundingBox.h"
#include <cstdint>


class WwiseRoomComponent : public Component {
public:
    WwiseRoomComponent() = default;
    ~WwiseRoomComponent() override;
    WwiseRoomComponent(const WwiseRoomComponent&) = delete;
    WwiseRoomComponent& operator=(const WwiseRoomComponent&) = delete;
    WwiseRoomComponent(WwiseRoomComponent&&) = default;
    WwiseRoomComponent& operator=(WwiseRoomComponent&&) = default;

    void Init(GameObject* in_gameObject) override;
    void Update(GameObject* in_gameObject) override;

    static void InitRoom(GameObject* in_gameObject);
    static void InitRoomGeometry(GameObject* roomObj);

    static void GenerateWalls(const GameObject* gameObject, const uint64_t& roomID, const uint64_t& wallSidesGeometryID,
                              const uint64_t& wallCeilingFloorGeometryID, const uint64_t& wallInstance1,
                              const uint64_t& wallInstance2, const uint64_t& wallInstance3,
                              const uint64_t& wallInstance4, const uint64_t& wallInstance5,
                              const uint64_t& wallInstance6);
    void SetBoundingBox(const GO_Vector3& center, const float& width, const float& length, const float& height);

    const GO_BoundingBox& GetBoundingBox() const;
    static const unsigned int& GetRoomID();

private:
    GO_BoundingBox boundingBox = {};

    static constexpr unsigned int ROOM = 200;
    static constexpr unsigned int GEOMETRY_ROOM = 400;
    static constexpr unsigned int GEOMETRY_ROOM_INSTANCE = 401;

    static constexpr unsigned int GEOMETRY_WALL_SIDES = 1000;
    static constexpr unsigned int GEOMETRY_WALL_CEILINGFLOOR = 1001;
    static constexpr unsigned int GEOMETRY_WALL_INSTANCE_1 = 2000;
    static constexpr unsigned int GEOMETRY_WALL_INSTANCE_2 = 2001;
    static constexpr unsigned int GEOMETRY_WALL_INSTANCE_3 = 2002;
    static constexpr unsigned int GEOMETRY_WALL_INSTANCE_4 = 2003;
    static constexpr unsigned int GEOMETRY_WALL_INSTANCE_5 = 2004;
    static constexpr unsigned int GEOMETRY_WALL_INSTANCE_6 = 2005;
    static constexpr unsigned int GEOMETRY_WALL_INSTANCE_OUTSIDE_1 = 2006;
    static constexpr unsigned int GEOMETRY_WALL_INSTANCE_OUTSIDE_2 = 2007;
    static constexpr unsigned int GEOMETRY_WALL_INSTANCE_OUTSIDE_3 = 2008;
    static constexpr unsigned int GEOMETRY_WALL_INSTANCE_OUTSIDE_4 = 2009;
    static constexpr unsigned int GEOMETRY_WALL_INSTANCE_OUTSIDE_5 = 2010;
    static constexpr unsigned int GEOMETRY_WALL_INSTANCE_OUTSIDE_6 = 2011;
};
