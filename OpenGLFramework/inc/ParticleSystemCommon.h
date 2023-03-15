#pragma once
#include <GLM/glm.hpp>
#include "Configuration.h"
#include "FRAME_EXPORTS.h"
using namespace glm;

namespace hiveGraphics
{
    enum class FRAME_DLLEXPORTS EmitterType :unsigned int
    {
        EMITTER_POINT,
        EMITTER_DISK,
        EMITTER_SPHERE,
        EMITTER_BALL,
        kNumEmitterType
    };

    enum class FRAME_DLLEXPORTS SimulationVolume :unsigned int
    {
        VOLUME_SPHERE,
        VOLUME_BOX,
        VOLUME_NONE,
        kNumSimulationVolume
    };

    struct FRAME_DLLEXPORTS SSimulationParameters
    {
        glm::vec4 emitter_position = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
        glm::vec4 emitter_direction = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

        unsigned int EmitNumPerSecond = 3000;
        float time_step_factor = 1.0f;
        float min_age = 0.0f;
        float max_age = 15.0f;

        EmitterType emitter_type = EmitterType::EMITTER_SPHERE;
        float emitter_radius = 10.0f;
        float scattering_factor = 1.0f;
        float vectorfield_factor = 1.0f;

        float curlnoise_factor = 16.0f;
        float curlnoise_scale = 128.0f;
        float velocity_factor = 5.0f;
        float padding = 0.0f;

        unsigned int enable_scattering = 1u;  //由于C++与GPU中的bool对不齐，所以用uint代替
        unsigned int enable_vectorfield = 0u;
        unsigned int enable_curlnoise = 0u;
        unsigned int enable_velocity_control = 1u;
    };


    struct TParticle
    {
        vec4 position;
        vec4 velocity;
        float start_age;
        float age;
        uint type;
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


