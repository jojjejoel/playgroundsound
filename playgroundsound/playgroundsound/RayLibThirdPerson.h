


#include <memory>
#include "GameObject.h"
#include <vector>
struct Camera3D;
struct Vector3;
struct Texture;
struct Model;
struct Color;


class RayLibThirdPerson {
public:
    void Run();
    void Init();
    void DeInit();
    const std::shared_ptr<GameObject> GetCameraGameObject();
private:
    std::shared_ptr<Camera3D> camera;
    std::shared_ptr<GameObject> cameraGameObject;
    std::vector<std::shared_ptr<Vector3>> positions;
    std::vector<float> heights;
    std::vector<std::shared_ptr<Color>> colors;
    int cameraMode;
};