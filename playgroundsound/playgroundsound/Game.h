


#include "DiffractionPath.h"
#include <memory>
#include <vector>
#include <functional>
#include <map>
#include <string>

#include "GameObjectManager.h"
#include "GameObject.h"

#include "WwiseRoomManager.h"

#include "DiffractionManager.h"
#include "RenderManager.h"

//struct Vector3;
//struct Texture;
//struct Color;
struct BoundingBox;
//struct RayCollision;
//struct Mesh;


class Game {
public:
    ~Game();

    void Run();
    void Init();
    void DeInit();

private:
    void DrawDiffractionPaths();
    void UpdateBlinkingLight();
    void MusicBeat();
    void MusicBar();
    void SetDiffractionPaths(const std::vector<DiffractionPath> diffractionPaths);
    void SetLightFlickerValue(const float& value);
    void AddGameObjects();
    void AddRoomWalls(GameObject* in_roomWallObjPtr, std::string_view modelName, std::string_view gameObjectName, const GO_Vector3& position);

    std::vector<std::shared_ptr<BoundingBox>> boundingBoxes;

    int beatValue = 0;
    float barValue = 0;

    std::vector<DiffractionPath> diffractionPaths;

    float lightFlickerValue = 0;

    bool updateFirstLight = true;
    bool updateSecondLight = false;

    float playbackSpeed = 1;

    std::function<void(const float&)> setRtpcFunction;

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