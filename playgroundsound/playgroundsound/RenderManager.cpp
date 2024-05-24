#include "RenderManager.h"

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "rlights.h"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            120
#endif

#define SHADOWMAP_RESOLUTION 1024

Light* light;

void RenderManager::SetCamera(CameraComponent* in_camera)
{
	camera = in_camera;
}

void RenderManager::Init()
{
	const int screenWidth = 1600;
	const int screenHeight = 900;

	InitWindow(screenWidth, screenHeight, "PlaygroundSound");
	SetConfigFlags(FLAG_MSAA_4X_HINT);



	LoadModels();


	shaders.insert(std::make_pair("shadow", std::make_shared<Shader>(LoadShader(TextFormat("Resources/Shaders/glsl%i/shadowmap.vs", GLSL_VERSION),
		TextFormat("Resources/Shaders/glsl%i/shadowmap.fs", GLSL_VERSION)))));
	Shader& shadowShader = *shaders["shadow"];
	shadowShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shadowShader, "viewPos");
	Vector3 lightDir = Vector3Normalize({ -0.35f, -10.0f, 0.35f });
	lightColor = { 255,0,255 };
	Vector4 lightColorNormalized = ColorNormalize({(unsigned char)lightColor.x, (unsigned char)lightColor.y, (unsigned char)lightColor.z, 255});
	lightDirLoc = GetShaderLocation(shadowShader, "lightDir");
	lightColLoc = GetShaderLocation(shadowShader, "lightColor");
	SetShaderValue(shadowShader, lightDirLoc, &lightDir, SHADER_UNIFORM_VEC3);
	SetShaderValue(shadowShader, lightColLoc, &lightColorNormalized, SHADER_UNIFORM_VEC4);
	int ambientLoc = GetShaderLocation(shadowShader, "ambient");
	float ambient[4] = { 1.1f, 0.1f, 0.1f, 1.0f };
	SetShaderValue(shadowShader, ambientLoc, ambient, SHADER_UNIFORM_VEC4);
	lightVPLoc = GetShaderLocation(shadowShader, "lightVP");
	shadowMapLoc = GetShaderLocation(shadowShader, "shadowMap");
	int shadowMapResolution = SHADOWMAP_RESOLUTION;
	SetShaderValue(shadowShader, GetShaderLocation(shadowShader, "shadowMapResolution"), &shadowMapResolution, SHADER_UNIFORM_INT);

	shadowMap = LoadShadowmapRenderTexture(SHADOWMAP_RESOLUTION, SHADOWMAP_RESOLUTION);

	lightCam = new Camera3D({ 0 });
	lightCam->position = Vector3Scale(lightDir, -15.0f);
	lightCam->target = Vector3Zero();
	lightCam->projection = CAMERA_ORTHOGRAPHIC;
	lightCam->up = { 0.0f, 1.0f, 0.0f };
	lightCam->fovy = 20.0f;

	for (auto& model : models) {
		model.second->materials[0].shader = *shaders["shadow"];
	}
}

void RenderManager::LoadModels()
{
	models.insert(std::make_pair("truck_green", std::make_shared<Model>(LoadModel("Resources/Models/truck_green.obj"))));

	models.insert(std::make_pair("WallTop", std::make_shared<Model>(LoadModelFromMesh(GenMeshCube(40, 3, 40)))));

	models.insert(std::make_pair("RoomCube", std::make_shared<Model>(LoadModelFromMesh(GenMeshCube(10, 10, 10)))));

	models.insert(std::make_pair("PortalCube", std::make_shared<Model>(LoadModelFromMesh(GenMeshCube(1, 1, 1)))));

	models.insert(std::make_pair("MusicCube", std::make_shared<Model>(LoadModelFromMesh(GenMeshCube(2, 2, 2)))));

	Model model = LoadModelFromMesh(GenMeshPlane(10, 10, 10, 20));
	model.transform = MatrixMultiply(MatrixRotateZ(1.5708f), model.transform);
	models.insert(std::make_pair("RoomWall", std::make_shared<Model>(model)));

	models.insert(std::make_pair("RoomWallSide", std::make_shared<Model>(LoadModelFromMesh(GenMeshCube(0.1f, 10, 10)))));

	models.insert(std::make_pair("RoomWallFront", std::make_shared<Model>(LoadModelFromMesh(GenMeshCube(10, 10, 0.1f)))));

	models.insert(std::make_pair("RoomWallTop", std::make_shared<Model>(LoadModelFromMesh(GenMeshCube(10, 0.1f, 10)))));
}

void RenderManager::StartRender()
{
	BeginDrawing();

	//ClearBackground(RAYWHITE);
	BeginMode3D(*camera->camera3D);
}

void RenderManager::Render()
{
	Vector3 cameraPos = camera->camera3D->position;
	SetShaderValue(*shaders["shadow"], shaders["shadow"]->locs[SHADER_LOC_VECTOR_VIEW], &cameraPos, SHADER_UNIFORM_VEC3);
	Vector4 lightColorNormalized = ColorNormalize({ (unsigned char)lightColor.x, (unsigned char)lightColor.y, (unsigned char)lightColor.z, 255 });
	SetShaderValue(*shaders["shadow"], lightColLoc, &lightColorNormalized, SHADER_UNIFORM_VEC4);
	//UpdateCamera(camera->camera3D, CAMERA_ORBITAL);

	
	Vector3 lightDir = Vector3Normalize({ 0.35f, -1.0f, -0.35f });
	lightCam->position = Vector3Scale(lightDir, -15.0f);
	SetShaderValue(*shaders["shadow"], lightDirLoc, &lightDir, SHADER_UNIFORM_VEC3);

	BeginDrawing();

	Matrix lightView;
	Matrix lightProj;
	BeginTextureMode(*shadowMap);
	ClearBackground(BLACK);
	BeginMode3D(*lightCam);
	lightView = rlGetMatrixModelview();
	lightProj = rlGetMatrixProjection();
	
	for (auto& renderObject : objectsToRender) {
		renderObject->GetComponent<RenderComponent>().Draw(renderObject);
	}

	EndMode3D();
	EndTextureMode();
	Matrix lightViewProj = MatrixMultiply(lightView, lightProj);

	ClearBackground(BLACK);

	SetShaderValueMatrix(*shaders["shadow"], lightVPLoc, lightViewProj);

	rlEnableShader(shaders["shadow"]->id);
	int slot = 10; // Can be anything 0 to 15, but 0 will probably be taken up
	rlActiveTextureSlot(10);
	rlEnableTexture(shadowMap->depth.id);
	rlSetUniform(shadowMapLoc, &slot, SHADER_UNIFORM_INT, 1);

	BeginMode3D(*camera->camera3D);

	for (auto& renderObject : objectsToRender) {
		renderObject->GetComponent<RenderComponent>().Draw(renderObject);
	}
}

void RenderManager::EndRender()
{
	EndMode3D();

	std::string playbackSpeedString = "Playback speed: " + std::to_string(playBackSpeed);
	DrawText(playbackSpeedString.c_str(), 2, 2, 20, WHITE);
	DrawText("Press 1 to open portal", 2, 22, 20, WHITE);
	DrawText("Press 2 to close portal", 2, 42, 20, WHITE);
	DrawText("Press 3 to decrease playback speed", 2, 62, 20, WHITE);
	DrawText("Press 4 to increase playback speed", 2, 82, 20, WHITE);
	EndDrawing();
}

std::shared_ptr<Model> RenderManager::GetModel(std::string_view modelName)
{
	return models[modelName.data()];
}

void RenderManager::SetLightColor(const GO_Vector3& in_color)
{
	lightColor = in_color;
}

RenderTexture2D* RenderManager::LoadShadowmapRenderTexture(int width, int height)
{
	RenderTexture2D target = { 0 };

	target.id = rlLoadFramebuffer(0, 0); // Load an empty framebuffer
	target.texture.width = width;
	target.texture.height = height;

	if (target.id > 0)
	{
		rlEnableFramebuffer(target.id);

		// Create depth texture
		// We don't need a color texture for the shadowmap
		target.depth.id = rlLoadTextureDepth(width, height, false);
		target.depth.width = width;
		target.depth.height = height;
		target.depth.format = 19;       //DEPTH_COMPONENT_24BIT?
		target.depth.mipmaps = 1;

		// Attach depth texture to FBO
		rlFramebufferAttach(target.id, target.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);

		//// Check if fbo is complete with attachments (valid)
		if (rlFramebufferComplete(target.id)) TRACELOG(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", target.id);

		rlDisableFramebuffer();
	}
	else TRACELOG(LOG_WARNING, "FBO: Framebuffer object can not be created");

	return new RenderTexture2D(target);
}

void RenderManager::UnloadShadowmapRenderTexture(RenderTexture2D* target)
{
	if (target->id > 0)
	{
		// NOTE: Depth texture/renderbuffer is automatically
		// queried and deleted before deleting framebuffer
		rlUnloadFramebuffer(target->id);
	}
}

void RenderManager::AddRenderObject(GameObject* in_objectToRender)
{
	objectsToRender.push_back(in_objectToRender);
}

void RenderManager::SetPlaybackSpeed(const float& speed)
{
	playBackSpeed = speed;
}
