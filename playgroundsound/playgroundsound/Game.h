#pragma once
#include "DiffractionPath.h"
#include <memory>
#include <vector>
#include <functional>
#include <map>
#include <string>

#include "Managers/GameObjectManager.h"
#include "GameObject/GameObject.h"

#include "Managers/WwiseRoomManager.h"

#include "Managers/DiffractionManager.h"
#include "Managers/RenderManager.h"

struct BoundingBox;

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
    void AddRoomWalls(GameObject* in_roomWallObjPtr, std::string_view modelName, std::string_view gameObjectName, const GO_Vector3& position);

    std::vector<std::shared_ptr<BoundingBox>> boundingBoxes;


    // Assumption is made that only time signature 4/4 will be used.
    static constexpr int numberOfBeatsInBar = 4;
    int beatValue = 0;
    float timeLeftOnBar = 0;
    float barDuration = 0;
    float playbackSpeed = 1;
    const float barDecreaseSpeedMultiplier = 0.37f;
    std::function<void(const float&)> setRtpcFunction;

    std::vector<DiffractionPath> diffractionPaths;

    GameObjectManager gameObjectManager;
    RenderManager renderManager;
    WwiseRoomManager wwiseRoomManager;
    DiffractionManager diffractionManager;

    GameObject* truckObjPtr;
    GameObject* cameraObjPtr;
    GameObject* musicEmitterObjPtr;
    GameObject* portalCubeObjPtr;
    GameObject* roomCubeObjPtr;
    GameObject* lightBulbObjPtr;
    GameObject* roomWallBackObjPtr;
    GameObject* roomWallRightObjPtr;
    GameObject* roomWallLeftObjPtr;
    GameObject* roomWallTopObjPtr;
    GameObject* roomWallBottomObjPtr;
    GameObject* roomWallObjPtr;
};