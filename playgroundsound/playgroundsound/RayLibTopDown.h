#pragma once

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

// Custom Blend Modes
#define RLGL_SRC_ALPHA 0x0302
#define RLGL_MIN 0x8007
#define RLGL_MAX 0x8008

#define MAX_BOXES     20
#define MAX_SHADOWS   MAX_BOXES*3         // MAX_BOXES *3. Each box can cast up to two shadow volumes for the edges it is away from, and one for the box itself
#define MAX_LIGHTS    16

// Shadow geometry type



class RayLibTopDown
{

    float sinValue = 0;

    typedef struct ShadowGeometry {
        Vector2 vertices[4];
    } ShadowGeometry;

    // Light info type
    typedef struct LightInfo {
        bool active;                // Is this light slot active?
        bool dirty;                 // Does this light need to be updated?
        bool valid;                 // Is this light in a valid position?

        Vector2 position;           // Light position
        RenderTexture mask;         // Alpha mask for the light
        float outerRadius;          // The distance the light touches
        Rectangle bounds;           // A cached rectangle of the light bounds to help with culling

        ShadowGeometry shadows[MAX_SHADOWS];
        int shadowCount;
    } LightInfo;


    LightInfo lights[MAX_LIGHTS] = { 0 };

    void MoveLight(int slot, float x, float y);
    void ComputeShadowVolumeForEdge(int slot, Vector2 sp, Vector2 ep);
    void DrawLightMask(int slot);
    void SetupLight(int slot, float x, float y, float radius);
    bool UpdateLight(int slot, Rectangle* boxes, int count);
    void SetupBoxes(Rectangle* boxes, int* count);
public: int Run();
};

