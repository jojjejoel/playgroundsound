#pragma once

// Include raylib first to establish function declarations before Windows.h
#include "src/raylib.h"

#include "DiffractionPath.h"
#include <memory>
#include <vector>
#include <string>

#include "Managers/GameObjectManager.h"
#include "GameObject/GameObject.h"

#include "Managers/WwiseRoomManager.h"

#include "Managers/DiffractionManager.h"
#include "Managers/RenderManager.h"

struct BoundingBox;

// Room wall configuration for organized wall management
struct RoomWalls {
    GameObject* back = nullptr;
    GameObject* right = nullptr;
    GameObject* left = nullptr;
    GameObject* top = nullptr;
    GameObject* bottom = nullptr;
};

class Game {
public:
    ~Game();

    void Run(bool& shouldExit);
    void ControlCarSfx();
    void UpdateBouncingCube();
    void Init();
    void DeInit();

private:
    void ControlPortalState();
    void ControlPlaybackSpeed();
    void DrawDiffractionPaths();
    void UpdateBlinkingLight();
    void MusicBeat();
    void MusicBar(const float& barDuration);
    void SetDiffractionPaths(const std::vector<DiffractionPath> diffractionPaths);
    void AddGameObjects();
    GameObject* AddRoomWall(std::string_view modelName, std::string_view gameObjectName, const GO_Vector3& position);

    // Helper methods for DrawDiffractionPaths
    Vector3 ConvertToVector3(const GO_Vector3& goVec) const;
    Color GetDiffractionColor(float diffractionValue) const;
    void DrawPathNodes(const DiffractionPath& path, const Vector3& listenerPos, const Color& color);

    // Music timing and playback constants
    static constexpr int numberOfBeatsInBar = 4;
    static constexpr float bouncingCubeBaseHeight = 0.2f;
    static constexpr float bouncingCubeHeightMultiplier = 3.0f;
    static constexpr float minPlaybackSpeed = 0.0f;
    static constexpr float maxPlaybackSpeed = 4.0f;
    static constexpr float diffracationSphereRadius = 0.2f;
    static constexpr int diffracationSphereSegments = 10;

    // Light color constants for beat visualization
    static constexpr uint8_t colorMax = 255;
    static constexpr uint8_t colorMin = 0;

    std::vector<std::shared_ptr<BoundingBox>> boundingBoxes;
    std::vector<DiffractionPath> diffractionPaths;

    // Music state
    int beatValue = -1;
    float timeLeftOnBar = 0.0f;
    float barDuration = 0.0f;
    float playbackSpeed = 1.0f;

    // Managers
    GameObjectManager gameObjectManager;
    RenderManager renderManager;
    WwiseRoomManager wwiseRoomManager;
    DiffractionManager diffractionManager;

    // Primary game objects
    GameObject* truckObjPtr = nullptr;
    GameObject* cameraObjPtr = nullptr;
    GameObject* musicEmitterObjPtr = nullptr;
    GameObject* portalCubeObjPtr = nullptr;
    GameObject* roomCubeObjPtr = nullptr;
    GameObject* roomWallObjPtr = nullptr;

    // Room walls organized in struct
    RoomWalls roomWalls;
};