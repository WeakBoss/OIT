#pragma once
#include "ParticleSystemCommon.h"
#include "RandomBuffer.h"
#include "AppendConsumeBuffer.h"
#include "VectorField.h"
#include "Shader.h"
#include "FRAME_EXPORTS.h"

namespace hiveGraphics
{
    struct FRAME_DLLEXPORTS TIndirectValues
    {
        unsigned int dispatch_x;
        unsigned int dispatch_y;
        unsigned int dispatch_z;
        unsigned int draw_count;
        unsigned int draw_primCount;
        unsigned int draw_first;
        unsigned int draw_reserved;
    };
    class FRAME_DLLEXPORTS GPUParticle
    {
    public:

        GPUParticle() :
            num_alive_particles_(0u),
            pbuffer_(nullptr),
            gl_indirect_buffer_id_(0u),
            gl_dp_buffer_id_(0u),
            gl_sort_indices_buffer_id_(0u),
            vao_(0u),
            simulated_(false),
            enable_sorting_(false),
            enable_vectorfield_(true)
        {}
        void init();
        void deinit();

        void update(float const dt, glm::mat4x4 const& view);

        unsigned int getParticlePositionVAO() const { return vao_; }
        unsigned int getIndirectBuffer() const { return gl_indirect_buffer_id_; }

        void setSimulationParameters(const SimulationParameters_t& vSimulationParameters)
        {
            simulation_params_ = vSimulationParameters;
        }
        const SimulationParameters_t& getSimulationParameters() const
        {
            return simulation_params_;
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

        unsigned int num_alive_particles_;              //< number of particle written and rendered on last frame.
        AppendConsumeBuffer* pbuffer_;                  //< Append / Consume buffer for particles.
        CRandomBuffer randbuffer_;                       //< StorageBuffer to hold random values.
        VectorField vectorfield_;                       //< Vector field handler.

        struct
        {
            std::shared_ptr<CShader> emission;
            std::shared_ptr<CShader> update_args;
            std::shared_ptr<CShader> simulation;
            std::shared_ptr<CShader> fill_indices;
            std::shared_ptr<CShader> calculate_dp;
            std::shared_ptr<CShader> sort_step;
            std::shared_ptr<CShader> sort_final;
            std::shared_ptr<CShader> render_point_sprite;
            std::shared_ptr<CShader> render_stretched_sprite;
        } pgm_;                                         //< Pipeline's shaders.

        GLuint gl_indirect_buffer_id_;                  //< Indirect Dispatch / Draw buffer.
        GLuint gl_dp_buffer_id_;                        //< DotProduct buffer.
        GLuint gl_sort_indices_buffer_id_;              //< indices buffer (for sorting).

        GLuint vao_;                                    //< VAO for rendering.

        bool simulated_;                                //< True if particles has been simulated.
        bool enable_sorting_;                           //< True if back-to-front sort is enabled.
        bool enable_vectorfield_;                       //< True if the vector field is used.
    };
}

