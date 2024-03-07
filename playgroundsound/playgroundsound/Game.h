


#include "DiffractionPath.h"
#include <memory>
#include "GameObject.h"
#include <vector>
#include <functional>

struct Camera3D;
struct Vector3;
struct Texture;
struct Model;
struct Color;
struct BoundingBox;
struct RayCollision;
struct Mesh;
struct Model;
struct Shader;


class Game {
public:
    void Run();
    void DrawDiffractionPaths();
    void UpdateBlinkingLight();
    void MusicBeat();
    void MusicBar();
    void SetDiffractionPaths(const std::vector<DiffractionPath> diffractionPaths);
    void Init();
    void AddCube(const GoTransform& transform, const std::shared_ptr<Shader>& shader);
    void ConvertVertices(std::shared_ptr<Model>& model, std::shared_ptr<GameObject>& gameObject);
    void ConvertTriangles(std::shared_ptr<Model>& model, std::shared_ptr<GameObject>& gameObject);
    void AddWall(const GoTransform& transform, const float& radians);
    void SetLightFlickerValue(const float& value);
    void DeInit();
    const std::shared_ptr<GameObject> GetLightBulbGameObject();
    const std::shared_ptr<GameObject> GetCameraGameObject();
    const std::shared_ptr<GameObject> GetPlayerGameObject();
    const std::vector<std::shared_ptr<GameObject>>& GetSoundBlockingObjects();
    const std::vector<std::shared_ptr<GameObject>>& GetWalls();
    BoundingBox CalculateBoundingBox(const Vector3& center, const float& width, const float& height, const float& length) const;
    bool IsGameObjectInRoom(const std::shared_ptr<GameObject>& gameObject);

    void AssignRtpcFunction(std::function<void (const float&)> function);

private:
    std::shared_ptr<Camera3D> camera;
    std::shared_ptr<GameObject> cameraGameObject;
    std::shared_ptr<GameObject> playerGameObject;

    std::vector<std::shared_ptr<GameObject>> soundBlockingObjects;

    std::vector<std::shared_ptr<Model>> models;

    std::vector<std::shared_ptr<BoundingBox>> boundingBoxes;

    std::vector<std::shared_ptr<GameObject>> roomWalls;


    float upX = 1.0f;
    float upY = 1.0f;
    float upZ = -1.0f;

    float forwardX = -1.0f;
    float forwardY = -1.0f;
    float forwardZ = 1.0f;


    int beatValue = 0;
    float barValue = 0;

    std::vector<DiffractionPath> diffractionPaths;

    float lightFlickerValue = 0;

    bool updateFirstLight = true;
    bool updateSecondLight = false;

    float playbackSpeed = 1;
    //std::shared_ptr<Light> light;


    std::function<void(const float&)> setRtpcFunction;
};