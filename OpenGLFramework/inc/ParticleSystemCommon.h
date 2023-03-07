#pragma once
#include <GLM/glm.hpp>
#include "Configuration.h"
#include "FRAME_EXPORTS.h"
using namespace glm;

namespace hiveGraphics
{
    enum EmitterType
    {
        EMITTER_POINT,
        EMITTER_DISK,
        EMITTER_SPHERE,
        EMITTER_BALL,
        kNumEmitterType
    };

    enum SimulationVolume
    {
        VOLUME_SPHERE,
        VOLUME_BOX,
        VOLUME_NONE,
        kNumSimulationVolume
    };

    struct SimulationParameters_t
    {
        float time_step_factor = 1.0f;
        float min_age = 50.0f;
        float max_age = 100.0f;
        EmitterType emitter_type = EmitterType::EMITTER_SPHERE;
        float emitter_position[3] = { 0.0f, 0.0f, 0.0f };
        float emitter_direction[3] = { 0.0f, 1.0f, 0.0f };
        float emitter_radius = 32.0f;
        SimulationVolume bounding_volume = SimulationVolume::VOLUME_SPHERE;
        float bounding_volume_size = 256.0f;

        float scattering_factor = 1.0f;
        float vectorfield_factor = 1.0f;
        float curlnoise_factor = 16.0f;
        float curlnoise_scale = 128.0f;
        float velocity_factor = 8.0f;

        bool enable_scattering = false;
        bool enable_vectorfield = false;
        bool enable_curlnoise = true;
        bool enable_velocity_control = true;
    };


    struct TParticle
    {
        vec4 position;
        vec4 velocity;
        float start_age;
        float age;
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


