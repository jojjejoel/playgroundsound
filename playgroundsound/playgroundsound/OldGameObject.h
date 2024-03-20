#pragma once
#include <cmath>
#include "Mesh.h"
#include <memory>
#include <string>
#include "GoVector3.h"
#include "GoTransform.h"
#include "Triangle.h"
struct Model;

class OldGameObject
{
public:
	const GoVector3& GetPosition() const;
	const GoVector3& GetUp() const;
	const GoVector3& GetForward() const;
	const GoVector3& GetScale() const;
	const std::shared_ptr<Model> GetModel() const;
	void SetTransform(const GoTransform& transform);
	void SetModel(std::shared_ptr<Model> in_model);
	void SetPosition(const GoVector3& position);
	void SetForward(const GoVector3& in_position);
	void SetUp(const GoVector3& in_position);

	void SetScale(const GoVector3& in_scale);

    GoVector3 GetNormalizedForward() const;
    GoVector3 GetNormalizedUp() const;
    
	Playground::Mesh mesh;
	std::vector<Triangle> triangles;

	void SetScaleMultiplier(const float& in_scaleMultiplier);
	const float& GetScaleMultiplier();

	void SetID(const unsigned int& in_iD);
	const unsigned int& GetID() const;

	const bool& GetIsWwiseRegistered() const;
	void SetIsWwiseRegistered(const bool& in_isWwiseRegistered);

	void SetName(const std::string_view in_name);
	std::string_view GetName() const;

	bool isWwiseRegistered = false;

	void SetRoomID(const unsigned int& roomID);
	const unsigned int& GetRoomID() const;
private:
	GoVector3 GetNormalized(float x, float y, float z) const;

	GoTransform transform;

	std::shared_ptr<Model> model;

	float scaleMultiplier = 1.0f;

	unsigned int iD;
	unsigned int roomID;

	std::string name;

};

