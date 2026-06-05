#include "RenderManager.h"

#include <ranges>
#include <src/raylib.h>
#include <src/raymath.h>
#include <src/rlgl.h>

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            120
#endif

#define SHADOWMAP_RESOLUTION 1024

void RenderManager::SetCamera(CameraComponent* in_camera) {
    camera = in_camera;
}

void RenderManager::Init() {
    constexpr int screenWidth = 1600;
    constexpr int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "PlaygroundSound");
    SetConfigFlags(FLAG_MSAA_4X_HINT);


    LoadModels();


    shaders.insert(std::make_pair("shadow", std::make_shared<Shader>(LoadShader(
                                      TextFormat("Resources/Shaders/glsl%i/shadowmap.vs", GLSL_VERSION),
                                      TextFormat("Resources/Shaders/glsl%i/shadowmap.fs", GLSL_VERSION)))));
    const Shader& shadowShader = *shaders["shadow"];
    shadowShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shadowShader, "viewPos");
    const Vector3 lightDir = Vector3Normalize({-0.35f, -10.0f, 0.35f});
    lightColor = {255, 0, 255};
    const Vector4 lightColorNormalized = ColorNormalize({
        static_cast<unsigned char>(lightColor.x), static_cast<unsigned char>(lightColor.y), static_cast<unsigned char>(lightColor.z), 255
    });
    lightDirLoc = GetShaderLocation(shadowShader, "lightDir");
    lightColLoc = GetShaderLocation(shadowShader, "lightColor");
    SetShaderValue(shadowShader, lightDirLoc, &lightDir, SHADER_UNIFORM_VEC3);
    SetShaderValue(shadowShader, lightColLoc, &lightColorNormalized, SHADER_UNIFORM_VEC4);
    const int ambientLoc = GetShaderLocation(shadowShader, "ambient");
    constexpr float ambient[4] = {1.1f, 0.1f, 0.1f, 1.0f};
    SetShaderValue(shadowShader, ambientLoc, ambient, SHADER_UNIFORM_VEC4);
    lightVPLoc = GetShaderLocation(shadowShader, "lightVP");
    shadowMapLoc = GetShaderLocation(shadowShader, "shadowMap");
    constexpr int shadowMapResolution = SHADOWMAP_RESOLUTION;
    SetShaderValue(shadowShader, GetShaderLocation(shadowShader, "shadowMapResolution"), &shadowMapResolution,
                   SHADER_UNIFORM_INT);

    shadowMap = LoadShadowmapRenderTexture(SHADOWMAP_RESOLUTION, SHADOWMAP_RESOLUTION);

    lightCam = std::make_shared<Camera3D>();
    lightCam->position = Vector3Scale(lightDir, -15.0f);
    lightCam->target = Vector3Zero();
    lightCam->projection = CAMERA_ORTHOGRAPHIC;
    lightCam->up = {0.0f, 1.0f, 0.0f};
    lightCam->fovy = 20.0f;

    for (const auto& val : models | std::views::values) {
        val->materials[0].shader = *shaders["shadow"];
    }
}

void RenderManager::LoadModels() {
    models.insert(std::make_pair("truck_green",
                                 std::make_shared<Model>(LoadModel("Resources/Models/truck_green.obj"))));

    models.insert(std::make_pair("WallTop", std::make_shared<Model>(LoadModelFromMesh(GenMeshCube(40, 3, 40)))));

    models.insert(std::make_pair("RoomCube", std::make_shared<Model>(LoadModelFromMesh(GenMeshCube(10, 10, 10)))));

    models.insert(std::make_pair("PortalCube", std::make_shared<Model>(LoadModelFromMesh(GenMeshCube(1, 1, 1)))));

    models.insert(std::make_pair("MusicCube", std::make_shared<Model>(LoadModelFromMesh(GenMeshCube(2, 2, 2)))));

    Model model = LoadModelFromMesh(GenMeshPlane(10, 10, 10, 20));
    model.transform = MatrixMultiply(MatrixRotateZ(1.5708f), model.transform);
    models.insert(std::make_pair("RoomWall", std::make_shared<Model>(model)));

    models.insert(std::make_pair("RoomWallSide",
                                 std::make_shared<Model>(LoadModelFromMesh(GenMeshCube(0.1f, 10, 10)))));

    models.insert(
        std::make_pair("RoomWallFront", std::make_shared<Model>(LoadModelFromMesh(GenMeshCube(10, 10, 0.1f)))));

    models.insert(std::make_pair("RoomWallTop", std::make_shared<Model>(LoadModelFromMesh(GenMeshCube(10, 0.1f, 10)))));
}

void RenderManager::StartRender() const {
    BeginDrawing();

    BeginMode3D(*camera->camera3D);
}

void RenderManager::Render() {
    const Vector3 cameraPos = camera->camera3D->position;
    SetShaderValue(*shaders["shadow"], shaders["shadow"]->locs[SHADER_LOC_VECTOR_VIEW], &cameraPos,
                   SHADER_UNIFORM_VEC3);
    const Vector4 lightColorNormalized = ColorNormalize({
        static_cast<unsigned char>(lightColor.x), static_cast<unsigned char>(lightColor.y), static_cast<unsigned char>(lightColor.z), 255
    });
    SetShaderValue(*shaders["shadow"], lightColLoc, &lightColorNormalized, SHADER_UNIFORM_VEC4);

    const Vector3 lightDir = Vector3Normalize({0.35f, -1.0f, -0.35f});
    lightCam->position = Vector3Scale(lightDir, -15.0f);
    SetShaderValue(*shaders["shadow"], lightDirLoc, &lightDir, SHADER_UNIFORM_VEC3);

    BeginDrawing();

    BeginTextureMode(*shadowMap);
    ClearBackground(BLACK);
    BeginMode3D(*lightCam);
    const Matrix lightView = rlGetMatrixModelview();
    const Matrix lightProj = rlGetMatrixProjection();

    for (const auto& renderObject : objectsToRender) {
        renderObject->GetComponent<RenderComponent>()->Draw(renderObject);
    }

    EndMode3D();
    EndTextureMode();
    const Matrix lightViewProj = MatrixMultiply(lightView, lightProj);

    ClearBackground(BLACK);

    SetShaderValueMatrix(*shaders["shadow"], lightVPLoc, lightViewProj);

    rlEnableShader(shaders["shadow"]->id);
    constexpr int slot = 10; // Can be anything 0 to 15, but 0 will probably be taken up
    rlActiveTextureSlot(10);
    rlEnableTexture(shadowMap->depth.id);
    rlSetUniform(shadowMapLoc, &slot, SHADER_UNIFORM_INT, 1);

    BeginMode3D(*camera->camera3D);

    for (const auto& renderObject : objectsToRender) {
        renderObject->GetComponent<RenderComponent>()->Draw(renderObject);
    }
}

void RenderManager::EndRender() const {
    EndMode3D();

    DrawUI();

    EndDrawing();
}

void RenderManager::DrawUI() const {
    DrawText("Use arrow keys to control car", 2, 2, 25, WHITE);

    const std::string portalOpenStatusStr = "Press 1 to toggle portal: " + portalIsEnabled;
    DrawText(portalOpenStatusStr.c_str(), 2, 62, 25, WHITE);

    DrawText("Press 2 to decrease playback speed", 2, 92, 25, WHITE);
    DrawText("Press 3 to increase playback speed", 2, 122, 25, WHITE);
    const std::string playbackSpeedString = "Playback speed: " + (playBackSpeed);
    DrawText(playbackSpeedString.c_str(), 2, 152, 25, WHITE);

    DrawText(beatValue.c_str(), 2, 182, 25, WHITE);
}

std::shared_ptr<Model> RenderManager::GetModel(const std::string_view modelName) {
    return models[std::string(modelName)];
}

void RenderManager::SetLightColor(const GO_Vector3& in_color) {
    lightColor = in_color;
}

std::shared_ptr<RenderTexture2D> RenderManager::LoadShadowmapRenderTexture(int width, int height) {
    RenderTexture2D target = {};

    target.id = rlLoadFramebuffer(); // Load an empty framebuffer
    target.texture.width = width;
    target.texture.height = height;

    if (target.id > 0) {
        rlEnableFramebuffer(target.id);

        // Create depth texture
        // We don't need a color texture for the shadowmap
        target.depth.id = rlLoadTextureDepth(width, height, false);
        target.depth.width = width;
        target.depth.height = height;
        target.depth.format = 19; //DEPTH_COMPONENT_24BIT?
        target.depth.mipmaps = 1;

        // Attach depth texture to FBO
        rlFramebufferAttach(target.id, target.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);

        //// Check if fbo is complete with attachments (valid)
        if (rlFramebufferComplete(target.id))
            TRACELOG(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", target.id);

        rlDisableFramebuffer();
    }
    else
        TRACELOG(LOG_WARNING, "FBO: Framebuffer object can not be created");

    return std::make_shared<RenderTexture2D>(target);
}

void RenderManager::AddRenderObject(GameObject* in_objectToRender) {
    objectsToRender.push_back(in_objectToRender);
}

void RenderManager::SetPlaybackSpeed(std::string_view in_playbackSpeed) {
    playBackSpeed = in_playbackSpeed;
}

void RenderManager::SetPortalEnabled(std::string_view in_portalIsEnabled) {
    portalIsEnabled = in_portalIsEnabled;
}

void RenderManager::SetBeatValue(std::string_view in_beatValue) {
    beatValue = in_beatValue;
}
