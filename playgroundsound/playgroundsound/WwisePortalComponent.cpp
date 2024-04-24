#include "WwisePortalComponent.h"
#include "AK/SpatialAudio/Common/AkSpatialAudio.h"
#include "AK/SpatialAudio/Common/AkSpatialAudioTypes.h"

void WwisePortalComponent::Init(GameObject* in_gameObject)
{
	
}

void WwisePortalComponent::Update(GameObject* in_gameObject)
{
}

void WwisePortalComponent::InitPortal(const GameObject* in_gameObject, const unsigned int& in_backRoomID, const unsigned int& in_frontRoomID)
{
	frontRoomID = in_frontRoomID;
	backRoomID = in_backRoomID;
	AKRESULT result;

	AkPortalParams paramsPortal;

	GO_Vector3 position = in_gameObject->m_transform.position;
	paramsPortal.Transform.SetPosition(position.x, position.y, -position.z);

	GO_Vector3 forward = in_gameObject->m_transform.forward.Normalized();
	GO_Vector3 up = in_gameObject->m_transform.up.Normalized();
	paramsPortal.Transform.SetOrientation({ forward.x, forward.y, -forward.z }, { -up.x, -up.y, up.z });
	paramsPortal.Extent.halfWidth = in_gameObject->m_transform.scale.x / 2.f;
	paramsPortal.Extent.halfHeight = in_gameObject->m_transform.scale.y / 2.f;
	paramsPortal.Extent.halfDepth = in_gameObject->m_transform.scale.z / 2.f;

	paramsPortal.bEnabled = true;
	paramsPortal.FrontRoom = frontRoomID;
	paramsPortal.BackRoom = backRoomID;

	result = AK::SpatialAudio::SetPortal(in_gameObject->m_id, paramsPortal, in_gameObject->m_name.c_str());
}

void WwisePortalComponent::InitPortal(const GameObject* in_gameObject, const unsigned int& in_backRoomID)
{
	InitPortal(in_gameObject, in_backRoomID, AK::SpatialAudio::kOutdoorRoomID);
}
