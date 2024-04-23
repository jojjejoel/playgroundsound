#pragma once
#include <cmath>
#include "Mesh.h"
#include <memory>
#include <string>
#include "GO_Vector3.h"
#include "GO_Transform.h"
#include "Triangle.h"
struct Model;

class OldGameObject
{
public:
	const GO_Vector3& GetPosition() const;
	const GO_Vector3& GetUp() const;
	const GO_Vector3& GetForward() const;
	const GO_Vector3& GetScale() const;
	const std::shared_ptr<Model> GetModel() const;
	void SetTransform(const GO_Transform& transform);
	void SetModel(std::shared_ptr<Model> in_model);
	void SetPosition(const GO_Vector3& position);
	void SetForward(const GO_Vector3& in_position);
	void SetUp(const GO_Vector3& in_position);

	void SetScale(const GO_Vector3& in_scale);

    GO_Vector3 GetNormalizedForward() const;
    GO_Vector3 GetNormalizedUp() const;
    
	GoMesh mesh;
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
	GO_Vector3 GetNormalized(float x, float y, float z) const;

	GO_Transform transform;

	std::shared_ptr<Model> model;

	float scaleMultiplier = 1.0f;

	unsigned int iD;
	unsigned int roomID;

	std::string name;

};

