


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


class RayLibThirdPerson {
public:
    void Run();
    RayCollision CheckCollisions();
    void Init();
    void DeInit();
    const std::shared_ptr<GameObject> GetCameraGameObject();
    const std::shared_ptr<GameObject> GetPlayerGameObject();
    BoundingBox CalculateBoundingBox(const Vector3& center, const float& width, const float& height, const float& length) const;
private:
    std::shared_ptr<Camera3D> camera;
    std::shared_ptr<GameObject> cameraGameObject;
    std::shared_ptr<GameObject> playerGameObject;
    std::vector<std::shared_ptr<Vector3>> positions;
    std::vector<float> heights;
    std::vector<std::shared_ptr<Color>> colors;

    std::vector<std::shared_ptr<BoundingBox>> boundingBoxes;
    int cameraMode;

    float upX = 1.0f;
    float upY = 1.0f;
    float upZ = -1.0f;

    float forwardX = -1.0f;
    float forwardY = -1.0f;
    float forwardZ = 1.0f;

    bool leftHand = true;
};