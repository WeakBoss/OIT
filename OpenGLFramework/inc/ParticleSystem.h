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
        unsigned int DispatchX;
        unsigned int DispatchY;
        unsigned int DispatchZ;
        unsigned int DrawCount;
        unsigned int DrawPrimCount;
        unsigned int DrawFirst;
        unsigned int DrawReserved;
    };
    class FRAME_DLLEXPORTS CParticleSystem
    {
    public:

        CParticleSystem() :
            m_NumAliveParticles(0u),
 
            m_IndirectBuffer(0u),
            m_DotProductBuffer(0u),
            m_SortIndicesBuffer(0u),
            m_VAO(0u),
            m_IsSimulated(false),
            m_EnableSorting(false),
            m_EnableVectorfield(true)
        {}
        ~CParticleSystem()
        {
            deinit();
        }
        void init();
        void deinit();
        void update(float const vDeltaTime, glm::mat4x4 const& vViewMat = glm::mat4x4());

        unsigned int getParticlePositionVAO() const { return m_VAO; }
        unsigned int getIndirectBuffer() const { return m_IndirectBuffer; }

        void setSimulationParameters(const SSimulationParameters& vSimulationParameters)
        {
            mSimulationParams = vSimulationParameters;
        }
        const SSimulationParameters& getSimulationParameters() const
        {
            return mSimulationParams;
        }
        inline void enableSorting(bool vStatus) { m_EnableSorting = vStatus; }
        inline void enableVectorfield(bool vStatus) { m_EnableVectorfield = vStatus; }

        void addParticleType(const SSimulationParameters& vSimulationParams)
        {
            mSimulationParamsSet.emplace_back(std::move(vSimulationParams));
        }

    private:
        void genVBO();
        void emission(unsigned int const vCount);
        void simulation(float const vTimeStep);
        void swapBuffer();
        void sortParticles(glm::mat4x4 const& vViewMat);
        unsigned int mGetThreadsGroupCount(unsigned int const vNumthreads)
        {
            return (vNumthreads + mThreadsGroupWidth - 1u) / mThreadsGroupWidth;
            m_NumParticleTypes++;
        }

        unsigned int mFloorParticleCount(unsigned int const vNparticles)
        {
            return mThreadsGroupWidth * (vNparticles / mThreadsGroupWidth);
        }

        

        unsigned int const mThreadsGroupWidth = PARTICLE_SYSTEM_KERNEL_GROUP_WIDTH;
        unsigned int const mMaxParticleCount = MAX_NUM_PARTICLES;
        unsigned int const mBatchEmitCount = MAX_NUM_PARTICLES_PER_BATCH;

        SSimulationParameters mSimulationParams;

        unsigned int m_NumParticleTypes = 0;
        std::vector<SSimulationParameters> mSimulationParamsSet;

        unsigned int m_NumAliveParticles;             
        std::shared_ptr<AppendConsumeBuffer> m_pAppendConsumeBuffer = nullptr;                 
        std::shared_ptr<CRandomBuffer> m_pRandBuffer = nullptr;
        std::shared_ptr<CVectorField> m_pVectorField = nullptr;                     

        struct
        {
            std::shared_ptr<CShader> Emission;
            std::shared_ptr<CShader> UpdateArgs;
            std::shared_ptr<CShader> Simulation;
            std::shared_ptr<CShader> FillIndices;
            std::shared_ptr<CShader> CalculateDp;
            std::shared_ptr<CShader> Sort;
            std::shared_ptr<CShader> SortFinal;
            std::shared_ptr<CShader> RenderPointSprite;
        } m_pComputeShaders;                                       

        GLuint m_IndirectBuffer;                    
        GLuint m_DotProductBuffer;                     
        GLuint m_SortIndicesBuffer;              

        GLuint m_VAO;                                    

        bool m_IsSimulated;                         
        bool m_EnableSorting;                         
        bool m_EnableVectorfield;                       
    };
}

