#pragma once
#include "Component.h"
#include "..\GameObject\GO_Vector3.h"
#include <functional>

class WwiseObjectComponent : public Component
{
public:
	WwiseObjectComponent() = default;
	~WwiseObjectComponent() override;
	WwiseObjectComponent(const WwiseObjectComponent&) = delete;
	WwiseObjectComponent& operator=(const WwiseObjectComponent&) = delete;
	WwiseObjectComponent(WwiseObjectComponent&&) = default;
	WwiseObjectComponent& operator=(WwiseObjectComponent&&) = default;
	
	void Init(GameObject* in_gameObject) override;
	void Update(GameObject* in_gameObject) override;

	void PostEvent(const unsigned int& eventID) const;
	void PostMusicEvent(const unsigned int&, std::function<void(float)> callbackFuncBar, std::function<void()> in_callbackFuncBeat);
	void RegisterAsListener() const;
	void RegisterAsDistanceProbe(const unsigned int& listenerID) const;
	void SetRTPC(const unsigned int& rtpcID, const float& rtpcValue) const;
	void SetRoomID(const unsigned int& in_akRoomID);

	const GO_Vector3& GetPosition() const;

	float GetGameParamValueGlobal(const unsigned int& rtpcID) const;

	std::function<void(float)> callbackFuntionBar;
	std::function<void()> callbackFuntionBeat;
private:
	unsigned int akGameObjectID = 0;
	const char* gameObjectName = nullptr;

	unsigned int currentRoomID = 0;

	GO_Vector3 m_position = {0,0,0};
};

