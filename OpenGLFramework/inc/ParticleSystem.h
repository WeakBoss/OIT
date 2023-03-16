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
        unsigned int Padding;
    };
    //struct DrawArraysIndirectCommand {
    //    GLuint count;
    //    GLuint instanceCount;  1
    //    GLuint first;   0
    //    GLuint baseInstance;  0
    //};

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
        unsigned int getNumAliveParticles() const { return m_NumAliveParticles; }


        inline void enableSorting(bool vStatus) { m_EnableSorting = vStatus; }
        inline void enableVectorfield(bool vStatus) { m_EnableVectorfield = vStatus; }

        void addParticleType(const SSimulationParameters& vSimulationParams)
        {
            m_SimulationParamsSet.push_back(vSimulationParams);
            m_NumParticleTypes++;
            m_TotalEmitNumPerSecond += vSimulationParams.EmitNumPerSecond;
        }
        void addParticleType(const std::vector<SSimulationParameters>& vSimulationParamsSet)
        {
            for (auto SP : vSimulationParamsSet)
                addParticleType(SP);
        }
        void setSimulationParameters(const SSimulationParameters& vSimulationParameters, int vIndex)
        {
            if (vIndex < m_SimulationParamsSet.size())
            {
                m_IsSimulationParamsUpdated = true;
                m_SimulationParamsSet[vIndex] = vSimulationParameters;
            }
            else
                _WARNING(true, "vIndex out of range");
        }
        void setSimulationParameters(const std::vector<SSimulationParameters>& vSimulationParameters)
        {
            for (int i = 0; i < vSimulationParameters.size(); i++)
                setSimulationParameters(vSimulationParameters[i], i);
                 
        }
    private:
        void genVBO();
        void emission(const float vDeltaTime);
        void simulation(float const vTimeStep);
        void swapBuffer();
        void sortParticles(glm::mat4x4 const& vViewMat);
        void bindSimulationParameters();
        void unbindSimulationParameters();
        unsigned int getBatchEmitNum(const float vDeltaTime);

        unsigned int GetThreadsGroupCount(unsigned int const vNumthreads)
        {
            return (vNumthreads + m_ThreadsGroupWidth - 1u) / m_ThreadsGroupWidth;
        }

        unsigned int FloorParticleCount(unsigned int const vNparticles)
        {
            return m_ThreadsGroupWidth * (vNparticles / m_ThreadsGroupWidth);
        }

        unsigned int const m_ThreadsGroupWidth = PARTICLE_SYSTEM_KERNEL_GROUP_WIDTH;
        unsigned int const m_MaxParticleCount = FloorParticleCount(MAX_NUM_PARTICLES);
        unsigned int const m_MaxEmitCountPerBatch = MAX_NUM_PARTICLES_PER_BATCH;


        unsigned int m_NumParticleTypes = 0;
        std::vector<SSimulationParameters> m_SimulationParamsSet;
        bool m_IsSimulationParamsUpdated = true;
        GLuint m_SimulationParamsBuffer;
        GLuint m_ParticleProportionBuffer;

        unsigned int m_NumAliveParticles = 0;;
        unsigned int m_TotalEmitNumPerSecond = 0;
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

