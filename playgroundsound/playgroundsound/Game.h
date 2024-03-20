


#include "DiffractionPath.h"
#include <memory>
#include "OldGameObject.h"
#include <vector>
#include <functional>
#include <map>
#include <string>

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
    void InputControls();
    void DrawDiffractionPaths();
    void UpdateBlinkingLight();
    void MusicBeat();
    void MusicBar();
    void SetDiffractionPaths(const std::vector<DiffractionPath> diffractionPaths);
    void Init();
    void AddGameObject(std::shared_ptr<Model> model, const unsigned int& goID , const std::string_view name, const GoVector3& in_position = {0,0,0}, const GoVector3& in_scale = {1,1,1});
    void ConvertVertices(const std::shared_ptr<Model>& model, OldGameObject& gameObject);
    void ConvertTriangles(const std::shared_ptr <Model>& model, OldGameObject& gameObject);
    void AddWall(const GoTransform& transform, const float& radians);
    void SetLightFlickerValue(const float& value);
    void DeInit();
    const std::shared_ptr<OldGameObject>& GetGameObject(const unsigned int& goID);
    const std::vector<std::shared_ptr<OldGameObject>>& GetWalls();
    BoundingBox CalculateBoundingBox(const Vector3& center, const float& width, const float& height, const float& length) const;
    bool IsGameObjectInRoom(const std::shared_ptr<OldGameObject>& gameObject);

    void AssignRtpcFunction(std::function<void(const float&)> function);

    const std::map<unsigned int, std::shared_ptr<OldGameObject>>& GetAllGameObjects();;

private:
    void DrawGameObject(std::shared_ptr<OldGameObject> gameObject);
    Vector3 ConvertGoVector3(const GoVector3& in_goVector);
    std::shared_ptr<Camera3D> camera;
    std::shared_ptr<OldGameObject> cameraGameObject;
    std::shared_ptr<OldGameObject> playerGameObject;

    std::map<std::string, std::shared_ptr<Model>> models;
    std::map<std::string, std::shared_ptr<Shader>> shaders;

    std::vector<std::shared_ptr<BoundingBox>> boundingBoxes;

    std::vector<std::shared_ptr<OldGameObject>> roomWalls;


    std::map<unsigned int, std::shared_ptr<OldGameObject>> gameObjects;




    int beatValue = 0;
    float barValue = 0;

    std::vector<DiffractionPath> diffractionPaths;

    float lightFlickerValue = 0;

    bool updateFirstLight = true;
    bool updateSecondLight = false;

    float playbackSpeed = 1;


    std::function<void(const float&)> setRtpcFunction;
};