#pragma once
#include "Component.h"
#include "GameObject.h"
struct Model;
struct Shader;

class RenderComponent : public Component
{
public:
    virtual void Init() override;

    virtual void Update(GameObject* parent) override;


    void SetModel(Model* in_model);
private:
    Model* model;
};

