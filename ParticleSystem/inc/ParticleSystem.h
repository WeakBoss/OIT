#pragma once
#include "ParticleSystemCommon.h"
#include "RandomBuffer.h"
#include "AppendConsumeBuffer.h"
#include "VectorField.h"
class GPUParticle
{
public:

    GPUParticle() :
        num_alive_particles_(0u),
        pbuffer_(nullptr),
        gl_indirect_buffer_id_(0u),
        gl_dp_buffer_id_(0u),
        gl_sort_indices_buffer_id_(0u),
        vao_(0u),
        query_time_(0u),
        simulated_(false),
        enable_sorting_(false),
        enable_vectorfield_(true)
    {}
    void init();
    void deinit();

    void update(float const dt, glm::mat4x4 const& view);
    void render(glm::mat4x4 const& view, glm::mat4x4 const& viewProj);
    inline SimulationParameters_t& simulation_parameters() 
    {
        return simulation_params_;
    }

    inline RenderingParameters_t& rendering_parameters() 
    {
        return rendering_params_;
    }

    inline const glm::uvec3& vectorfield_dimensions() const 
    {
        return vectorfield_.dimensions();
    }

    inline void enable_sorting(bool status) { enable_sorting_ = status; }
    inline void enable_vectorfield(bool status) { enable_vectorfield_ = status; }
private:
    unsigned int const kThreadsGroupWidth = PARTICLE_SYSTEM_KERNEL_GROUP_WIDTH;
    unsigned int const kMaxParticleCount = MAX_NUM_PARTICLES;
    unsigned int const kBatchEmitCount = MAX_NUM_PARTICLES_PER_BATCH;

    unsigned int GetThreadsGroupCount(unsigned int const nthreads) 
    {
        return (nthreads + kThreadsGroupWidth - 1u) / kThreadsGroupWidth;
    }

    unsigned int FloorParticleCount(unsigned int const nparticles) 
    {
        return kThreadsGroupWidth * (nparticles / kThreadsGroupWidth);
    }

    void _setup_render();

    void _emission(unsigned int const count);
    void _simulation(float const time_step);
    void _postprocess();
    void _sorting(glm::mat4x4 const& view);

    SimulationParameters_t simulation_params_;
    RenderingParameters_t rendering_params_;

    unsigned int num_alive_particles_;              //< number of particle written and rendered on last frame.
    AppendConsumeBuffer* pbuffer_;                  //< Append / Consume buffer for particles.
    CRandomBuffer randbuffer_;                       //< StorageBuffer to hold random values.
    VectorField vectorfield_;                       //< Vector field handler.

    struct
    {
        GLuint emission;
        GLuint update_args;
        GLuint simulation;
        GLuint fill_indices;
        GLuint calculate_dp;
        GLuint sort_step;
        GLuint sort_final;
        GLuint render_point_sprite;
        GLuint render_stretched_sprite;
    } pgm_;                                         //< Pipeline's shaders.

    struct
    {
        struct
        {
            GLint emitCount;
            GLint emitterType;
            GLint emitterPosition;
            GLint emitterDirection;
            GLint emitterRadius;
            GLint particleMinAge;
            GLint particleMaxAge;
        } emission;
        struct 
        {
            GLint timeStep;
            GLint vectorFieldSampler;
            GLint bboxSize;
            GLint boundingVolume;

            GLint scatteringFactor;
            GLint vectorFieldFactor;
            GLint curlNoiseFactor;
            GLint curlNoiseScale;
            GLint velocityFactor;
            GLint enableScattering;
            GLint enableVectorField;
            GLint enableCurlNoise;
            GLint enableVelocityControl;
        } simulation;
        struct 
        {
            GLint view;
        } calculate_dp;
        struct
        {
            GLint blockWidth;
            GLint maxBlockWidth;
        } sort_step;
        struct
        {
            GLint mvp;
            GLint minParticleSize;
            GLint maxParticleSize;
            GLint colorMode;
            GLint birthGradient;
            GLint deathGradient;
            GLint fadeCoefficient;
        } render_point_sprite;
        struct 
        {
            GLint view;
            GLint mvp;
            GLint colorMode;
            GLint birthGradient;
            GLint deathGradient;
            GLint spriteStretchFactor;
            GLint fadeCoefficient;
        } render_stretched_sprite;
    } ulocation_;                                   //< Programs uniform location.


    GLuint gl_indirect_buffer_id_;                  //< Indirect Dispatch / Draw buffer.
    GLuint gl_dp_buffer_id_;                        //< DotProduct buffer.
    GLuint gl_sort_indices_buffer_id_;              //< indices buffer (for sorting).

    GLuint vao_;                                    //< VAO for rendering.
    GLuint query_time_;                             //< QueryObject for benchmarking.

    bool simulated_;                                //< True if particles has been simulated.
    bool enable_sorting_;                           //< True if back-to-front sort is enabled.
    bool enable_vectorfield_;                       //< True if the vector field is used.
};
