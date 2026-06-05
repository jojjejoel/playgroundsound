#pragma once
#include <AK/SoundEngine/Common/AkTypes.h>

#include "../GameObject/GO_Vector3.h"
#include <src/raylib.h>

namespace VectorConversions {
    // Coordinate system transformation option
    enum class CoordinateFlip {
        None,
        FlipZ // For Wwise Y-up to Raylib Z-up conversion
    };

    // AkVector64 (Wwise SDK double precision) → GO_Vector3
    inline GO_Vector3 AkToGO(const AkVector64& akVec,
                             const CoordinateFlip flip = CoordinateFlip::None) {
        const float z = flip == CoordinateFlip::FlipZ
                            ? -static_cast<float>(akVec.Z)
                            : static_cast<float>(akVec.Z);
        return GO_Vector3{
            static_cast<float>(akVec.X),
            static_cast<float>(akVec.Y),
            z
        };
    }

    inline Vector3 GOToRaylib(const GO_Vector3& goVec) {
        return Vector3{goVec.x, goVec.y, goVec.z};
    }

    inline AkVector64 GOToAk(const GO_Vector3& goVec,
                             const CoordinateFlip flip = CoordinateFlip::None) {
        const double z = flip == CoordinateFlip::FlipZ
                             ? -static_cast<double>(goVec.z)
                             : static_cast<double>(goVec.z);
        return AkVector64{goVec.x, goVec.y, z};
    }
}
