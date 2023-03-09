#pragma once
#include <GLM/glm.hpp>
#include "Configuration.h"
#include "FRAME_EXPORTS.h"
using namespace glm;

namespace hiveGraphics
{
    enum FRAME_DLLEXPORTS EmitterType
    {
        EMITTER_POINT,
        EMITTER_DISK,
        EMITTER_SPHERE,
        EMITTER_BALL,
        kNumEmitterType
    };

    enum FRAME_DLLEXPORTS SimulationVolume
    {
        VOLUME_SPHERE,
        VOLUME_BOX,
        VOLUME_NONE,
        kNumSimulationVolume
    };

    struct FRAME_DLLEXPORTS SSimulationParameters
    {
        int EmitNumPerSecond = 500;
        float time_step_factor = 1.0f;
        float min_age = 0.0f;
        float max_age = 15.0f;
        EmitterType emitter_type = EmitterType::EMITTER_SPHERE;
        float emitter_position[3] = { 20.0f, 0.0f, 0.0f };
        float emitter_direction[3] = { 0.0f, 1.0f, 0.0f };
        float emitter_radius = 10.0f;

        float scattering_factor = 1.0f;
        float vectorfield_factor = 1.0f;
        float curlnoise_factor = 16.0f;
        float curlnoise_scale = 128.0f;
        float velocity_factor = 5.0f;

        bool enable_scattering = true;
        bool enable_vectorfield = false;
        bool enable_curlnoise = false;
        bool enable_velocity_control = true;
    };


    struct TParticle
    {
        vec4 position;
        vec4 velocity;
        float start_age;
        float age;
        float _padding0;
        uint id;
    };

    inline void SwapUint(unsigned int& a, unsigned int& b)
    {
        a ^= b;
        b ^= a;
        a ^= b;
    }

    inline unsigned int GetNumTrailingBits(unsigned int const n)
    {
        unsigned int r = 0u;
        for (unsigned int i = n; i > 1u; i >>= 1u) ++r;
        return r;
    }

    inline unsigned int GetClosestPowerOfTwo(unsigned int const n) {
        unsigned int r = 1u;
        for (unsigned int i = 0u; r < n; r <<= 1u) ++i;
        return r;
    }
}


