#pragma once
#include "Component.h"
#include "..\GameObject\GameObject.h"
struct Model;
struct Shader;

#include "..\GameObject\GoVertex.h"
#include "..\Triangle.h"

class RenderComponent : public Component
{
public:
    virtual void Init(GameObject* in_gameObject) override;

    virtual void Update(GameObject* in_gameObject) override;
    void Draw(const GameObject* in_gameObject) const;


    void SetModel(Model* in_model, const bool& in_shouldRender = true, const bool& initRoom = false, const GO_Vector3& in_color = {255,255,255});


    const std::vector<GoVertex>& GetVertices() const;
    const std::vector<Triangle>& GetTriangles() const;

    void SetShouldRender(const bool& in_shouldRender);

private:
    void InitRoomGeometry();
    Model* model = nullptr;

    std::vector<Triangle> m_triangles;
    std::vector<GoVertex> m_vertices;
    GO_Vector3 color = {255,255,255};
    bool shouldRender = false;
};

