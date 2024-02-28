


#include "DiffractionPath.h"
#include <memory>
#include "GameObject.h"
#include <vector>

struct Camera3D;
struct Vector3;
struct Texture;
struct Model;
struct Color;
struct BoundingBox;
struct RayCollision;
struct Mesh;
struct Model;



class RayLibThirdPerson {
public:
    void Run();
    RayCollision CheckCollisions();
    void SetDiffractionPaths(const std::vector<DiffractionPath> diffractionPaths);
    void Init();
    void AddObject(const GoTransform& transform);
    void DeInit();
    const std::shared_ptr<GameObject> GetCameraGameObject();
    const std::shared_ptr<GameObject> GetPlayerGameObject();
    const std::vector<std::shared_ptr<GameObject>>& GetSoundBlockingObjects();
    BoundingBox CalculateBoundingBox(const Vector3& center, const float& width, const float& height, const float& length) const;
    bool IsPlayerInRoom();
private:
    std::shared_ptr<Camera3D> camera;
    std::shared_ptr<GameObject> cameraGameObject;
    std::shared_ptr<GameObject> playerGameObject;

    std::vector<std::shared_ptr<GameObject>> soundBlockingObjects;

    std::vector<std::shared_ptr<Model>> models;

    std::vector<std::shared_ptr<BoundingBox>> boundingBoxes;
    int cameraMode;

    float upX = 1.0f;
    float upY = 1.0f;
    float upZ = -1.0f;

    float forwardX = -1.0f;
    float forwardY = -1.0f;
    float forwardZ = 1.0f;

    bool leftHand = true;

    std::vector<DiffractionPath> diffractionPaths;
};