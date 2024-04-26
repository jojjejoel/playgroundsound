#pragma once
#include "Component.h"
#include "GameObject.h"
struct Model;
struct Shader;

#include "GoVertex.h"
#include "Triangle.h"

class RenderComponent : public Component
{
public:
    virtual void Init(GameObject* in_gameObject) override;

    virtual void Update(GameObject* in_gameObject) override;


    void SetModel(Model* in_model, bool initRoom = false);


    const std::vector<GoVertex>& GetVertices() const;
    const std::vector<Triangle>& GetTriangles() const;

private:
    void InitRoomGeometry();
    Model* model;

    std::vector<Triangle> m_triangles;
    std::vector<GoVertex> m_vertices;
};

