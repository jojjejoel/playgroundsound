#pragma once
#include "Component.h"
#include "GameObject.h"
struct Model;
struct Shader;

class RenderComponent : public Component
{
public:
    virtual void Init(GameObject* in_gameObject) override;

    virtual void Update(GameObject* in_gameObject) override;


    void SetModel(Model* in_model);
private:
    Model* model;
};

