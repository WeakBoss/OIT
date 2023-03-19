#include "ParticleSystem.h"
#include "ParticleSystemCommon.h"
#include <glm/gtc/type_ptr.hpp>
using namespace hiveGraphics;

//**************************************************************************************************
//FUNCTION:
void CParticleSystem::init() {
    genBuffers();


    /* VectorField generator */
    if (m_EnableVectorfield) {
        m_pVectorField = std::make_shared<CVectorField>();
        m_pVectorField->init(128u, 128u, 64u);
        m_pVectorField->generateValues("velocities.dat");
    }

    /* Compute Shaders */
    
    m_pComputeShaders.Emission = std::make_shared<CShader>("../OpenGLFramework/shaders/cs_emission.glsl");
    m_pComputeShaders.UpdateArgs = std::make_shared<CShader>("../OpenGLFramework/shaders/cs_update_args.glsl");
    m_pComputeShaders.Simulation = std::make_shared<CShader>("../OpenGLFramework/shaders/cs_simulation.glsl");
    m_pComputeShaders.FillIndices = std::make_shared<CShader>("../OpenGLFramework/shaders/cs_fill_indices.glsl");
    m_pComputeShaders.CalculateDp = std::make_shared<CShader>("../OpenGLFramework/shaders/cs_calculate_dp.glsl");
    m_pComputeShaders.Sort = std::make_shared<CShader>("../OpenGLFramework/shaders/cs_sort_step.glsl");
    m_pComputeShaders.SortFinal = std::make_shared<CShader>("../OpenGLFramework/shaders/cs_sort_final.glsl");

 
    //ulocation_.calculate_dp.view = GetUniformLocation(m_pComputeShaders.calculate_dp, "uViewMatrix");

    //ulocation_.sort_step.blockWidth = GetUniformLocation(m_pComputeShaders.sort_step, "uBlockWidth");
    //ulocation_.sort_step.maxBlockWidth = GetUniformLocation(m_pComputeShaders.sort_step, "uMaxBlockWidth");

    /* One time uniform setting */
    m_pComputeShaders.Simulation->setIntUniformValue("uPerlinNoisePermutationSeed", rand());

    

    /* Setup rendering buffers */
    genVBO();




}

//**************************************************************************************************
//FUNCTION:
void CParticleSystem::deinit() {
    m_pAppendConsumeBuffer->deinit();
    m_pRandBuffer->deinit();

    if (m_EnableVectorfield) {
        m_pVectorField->deinit();
    }

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SimulationParamsBuffer);
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0u);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ParticleProportionBuffer);
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0u);

    glDeleteBuffers(1u, &m_IndirectBuffer);
    glDeleteBuffers(1u, &m_DotProductBuffer);
    glDeleteBuffers(1u, &m_SortIndicesBuffer);
    glDeleteBuffers(1u, &m_SimulationParamsBuffer);
    glDeleteBuffers(1u, &m_ParticleProportionBuffer);
    glDeleteVertexArrays(1u, &m_VAO);
}

//**************************************************************************************************
//FUNCTION:
void CParticleSystem::update(const float vDeltaTime, glm::mat4x4 const& vViewMat) {
 
    /* Update random buffer with new values */
 

    m_pAppendConsumeBuffer->bindAttributes();
    m_pAppendConsumeBuffer->bindAtomics();
    m_pRandBuffer->bind();
    bindSimulationParameters();
    {
        /* Emission stage : write in buffer A */
        emission(vDeltaTime);

        /* Simulation stage : read buffer A, write buffer B */
        simulation(vDeltaTime);
    }
    m_pRandBuffer->unbind();
    m_pAppendConsumeBuffer->unbindAtomics();
    unbindSimulationParameters();

    /* Sort particles for alpha-blending. */
    if (m_EnableSorting && m_IsSimulated) {
        sortParticles(vViewMat);
    }

    m_pAppendConsumeBuffer->unbindAttributes();

    /* PostProcess stage */
    swapBuffer();

 
}

//**************************************************************************************************
//FUNCTION:
void CParticleSystem::genVBO() {
    glGenVertexArrays(1u, &m_VAO);
    glBindVertexArray(m_VAO);

    GLuint const vbo = m_pAppendConsumeBuffer->getFirstStorageBuffer();

#if SPARKLE_USE_SOA_LAYOUT

    unsigned int const attrib_size = 4u * sizeof(float); // vec4
    unsigned int const attrib_buffer_size = pbuffer_->single_attrib_buffer_size();

    GLuint binding_point = 0u;
    GLuint attrib_index = 0u;

    // POSITION
    binding_point = STORAGE_BINDING_PARTICLE_POSITIONS_A;
    glBindVertexBuffer(binding_point, vbo, attrib_index * attrib_buffer_size, attrib_size);
    {
        unsigned int const num_component = 3u;
        glVertexAttribFormat(attrib_index, num_component, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(attrib_index, binding_point);
        glEnableVertexAttribArray(attrib_index);
        ++attrib_index;
    }

    // VELOCITY
    binding_point = STORAGE_BINDING_PARTICLE_VELOCITIES_A;
    glBindVertexBuffer(binding_point, vbo, attrib_index * attrib_buffer_size, attrib_size);
    {
        unsigned int const num_component = 3u;
        glVertexAttribFormat(attrib_index, num_component, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(attrib_index, binding_point);
        glEnableVertexAttribArray(attrib_index);
        ++attrib_index;
    }

    // AGE ATTRIBUTES
    binding_point = STORAGE_BINDING_PARTICLE_ATTRIBUTES_A;
    glBindVertexBuffer(binding_point, vbo, attrib_index * attrib_buffer_size, attrib_size);
    {
        unsigned int const num_component = 2u;
        glVertexAttribFormat(attrib_index, num_component, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(attrib_index, binding_point);
        glEnableVertexAttribArray(attrib_index);
        ++attrib_index;
    }

#else

    unsigned int const binding_index = 0u;
    glBindVertexBuffer(binding_index, vbo, 0u, sizeof(TParticle));
    // Particle's position
    {
        unsigned int const attrib_index = 0u;
        unsigned int const num_component = static_cast<unsigned int>((sizeof TParticle::position) / sizeof(TParticle::position[0u]));
        // Set the attribute format in Vertex Array
        glVertexAttribFormat(attrib_index, num_component, GL_FLOAT, GL_FALSE, offsetof(TParticle, position));
        // Bind attribute to a vertex buffer
        glVertexAttribBinding(attrib_index, binding_index);
        // Activate the attribute
        glEnableVertexAttribArray(attrib_index);
    }
    // velocities
    {
        unsigned int const attrib_index = 1u;
        unsigned int const num_component = static_cast<unsigned int>((sizeof TParticle::velocity) / sizeof(TParticle::velocity[0u]));
        glVertexAttribFormat(attrib_index, num_component, GL_FLOAT, GL_FALSE, offsetof(TParticle, velocity));
        glVertexAttribBinding(attrib_index, binding_index);
        glEnableVertexAttribArray(attrib_index);
    }
    // Particle's age info
    {
        unsigned int const attrib_index = 2u;
        unsigned int const num_component = 2u;
        glVertexAttribFormat(attrib_index, num_component, GL_FLOAT, GL_FALSE, offsetof(TParticle, start_age)); //
        glVertexAttribBinding(attrib_index, binding_index);
        glEnableVertexAttribArray(attrib_index);
    }
 
    {
        unsigned int const attrib_index = 3u;
        unsigned int const num_component = 1u;
        glVertexAttribFormat(attrib_index, num_component, GL_FLOAT, GL_FALSE, offsetof(TParticle, type)); //
        glVertexAttribBinding(attrib_index, binding_index);
        glEnableVertexAttribArray(attrib_index);
    }

#endif

    glBindVertexArray(0u);
}

//**************************************************************************************************
//FUNCTION:
void hiveGraphics::CParticleSystem::genBuffers()
{
    /* Append/Consume Buffer */
    unsigned int const num_attrib_buffer = (sizeof(TParticle) + sizeof(glm::vec4) - 1u) / sizeof(glm::vec4); //
    m_pAppendConsumeBuffer = std::make_shared<AppendConsumeBuffer>(m_MaxParticleCount, num_attrib_buffer);
    m_pAppendConsumeBuffer->init();

    /* Random value buffer */
    unsigned int const num_randvalues = 3u * m_MaxParticleCount;

    m_pRandBuffer = std::make_shared<CRandomBuffer>();
    m_pRandBuffer->init(num_randvalues);
    m_pRandBuffer->generateValues();

    /* Dispatch and Draw Indirect buffer */
    glGenBuffers(1u, &m_IndirectBuffer);
    glBindBuffer(GL_DISPATCH_INDIRECT_BUFFER, m_IndirectBuffer);
    TIndirectValues const default_indirect[] = { {
            // Dispatch values
            1u, 1u, 1u,
            // DrawCount
            0u,
            // Draw values
            1u, 0u, 0u,
            //padding
            0u
          } };
    glBufferStorage(GL_DISPATCH_INDIRECT_BUFFER, sizeof default_indirect, default_indirect, 0);
    glBindBuffer(GL_DISPATCH_INDIRECT_BUFFER, 0u);

    /* Storage buffers */

    // The parallel nature of the sorting algorithm needs power of two sized buffer.
    unsigned int const sort_buffer_max_count = GetClosestPowerOfTwo(m_MaxParticleCount); //

    // DotProducts buffer.
    glGenBuffers(1u, &m_DotProductBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_DotProductBuffer);
    GLuint const dp_buffer_size = sort_buffer_max_count * sizeof(GLfloat);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, dp_buffer_size, nullptr, 0);

    // Double-sized buffer for indices sorting.
    /// @note might use short instead.
    glGenBuffers(1u, &m_SortIndicesBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SortIndicesBuffer);
    GLuint const sort_indices_buffer_size = 2u * sort_buffer_max_count * sizeof(GLuint);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, sort_indices_buffer_size, nullptr, 0);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0u);

    //m_SimulationParamsBuffer

    if (0 == m_NumParticleTypes)
    {
        _WARNING(true, "m_NumParticleTypes = 0");
        addParticleType(SSimulationParameters());
    }
    glGenBuffers(1u, &m_SimulationParamsBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SimulationParamsBuffer);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(SSimulationParameters) * m_NumParticleTypes, nullptr, GL_MAP_WRITE_BIT);
    m_pMapSimulationParams = reinterpret_cast<SSimulationParameters*>(
        glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(SSimulationParameters) * m_NumParticleTypes, GL_MAP_WRITE_BIT));
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0u);

    //m_ParticleProportionBuffer
    glGenBuffers(1u, &m_ParticleProportionBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ParticleProportionBuffer);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, sizeof(float) * m_NumParticleTypes, nullptr, GL_MAP_WRITE_BIT);
    m_pMapParticleProportion = reinterpret_cast<float*>(
        glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(float) * m_NumParticleTypes, GL_MAP_WRITE_BIT));
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0u);

}

//**************************************************************************************************
//FUNCTION:
void CParticleSystem::emission(const float vDeltaTime) {
    
    const  unsigned int EmitNum = getBatchEmitNum(vDeltaTime);
    if (!EmitNum) {
        return;
    }

    m_pComputeShaders.Emission->activeShader();
    m_pComputeShaders.Emission->setuIntUniformValue("uEmitCount", EmitNum);
    m_pComputeShaders.Emission->setuIntUniformValue("uNumParticleType", m_NumParticleTypes);
    m_pComputeShaders.Emission->setuIntUniformValue("uRandomuint", m_pRandBuffer->getRandomInt(0,100));
    

    unsigned int const nGroups = GetThreadsGroupCount(EmitNum);
    glDispatchCompute(nGroups, 1u, 1u);
    glUseProgram(0u);

    glMemoryBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

    /* Number of particles expected to be simulated. */
    m_NumAliveParticles += EmitNum;

 

}

//**************************************************************************************************
//FUNCTION:
void CParticleSystem::simulation(float const vTimeStep) {
    if (m_NumAliveParticles == 0u) {
        m_IsSimulated = false;
        return;
    }

    /* Update Indirect arguments buffer for simulation dispatch and draw indirect. */
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_INDIRECT_ARGS, m_IndirectBuffer);

    m_pComputeShaders.UpdateArgs->activeShader();
    {
        glDispatchCompute(1u, 1u, 1u);
    }
    glUseProgram(0u);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_INDIRECT_ARGS, 0u);

    /* Synchronize the indirect argument buffer */
    glMemoryBarrier(GL_COMMAND_BARRIER_BIT);  // 屏障后通过Draw*indrect命令从缓冲区对象获取的命令数据将反映屏障前着色器写入的数据。

    /* Simulation Kernel */
    if (m_EnableVectorfield) {
        glBindTexture(GL_TEXTURE_3D, m_pVectorField->texture_id());
    }
    m_pComputeShaders.Simulation->activeShader();
    {
        m_pComputeShaders.Simulation->setuIntUniformValue("uNumAliveParticles", m_NumAliveParticles);
        m_pComputeShaders.Simulation->setFloatUniformValue("uTimeStep", vTimeStep);
        m_pComputeShaders.Simulation->setIntUniformValue("uVectorFieldSampler", 0);
   
 

        glBindBuffer(GL_DISPATCH_INDIRECT_BUFFER, m_IndirectBuffer);
        glDispatchComputeIndirect(0);
        glBindBuffer(GL_DISPATCH_INDIRECT_BUFFER, 0u);
    }
    glUseProgram(0u);

    glBindTexture(GL_TEXTURE_3D, 0u);

    /* Synchronize operations on buffers. */
    glMemoryBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT | GL_BUFFER_UPDATE_BARRIER_BIT);
    //GL_BUFFER_UPDATE_BARRIER_BIT保证之后使用glMapBuffer的使用的屏障

    /* Retrieve the number of alive particle to be used in the next frame. */
    /// @note Needed if we want to emit new particles.
    m_NumAliveParticles = m_pAppendConsumeBuffer->getNumAliveParticlesFromDevice();

    m_IsSimulated = true;
}

//**************************************************************************************************
//FUNCTION:
void CParticleSystem::sortParticles(glm::mat4x4 const& vViewMat) {
    /** @note there is probably some remaining issues on kernels boundaries.*/

    /* The algorithm works on buffer sized in power of two. */
    unsigned int const max_elem_count = GetClosestPowerOfTwo(m_NumAliveParticles);

    /* 1) Intialize indices and dotproducts buffers. */

    // Fill first part of the indices buffer with continuous indices.
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_INDICES_FIRST, m_SortIndicesBuffer);

    m_pComputeShaders.FillIndices->activeShader();
    {
        unsigned int const num_groups = GetThreadsGroupCount(max_elem_count); //
        glDispatchCompute(num_groups, 1u, 1u);
    }
    glUseProgram(0u);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_INDICES_FIRST, 0u);

    // Clear the dot product buffer.
    float const clear_value = -FLT_MAX;
    glClearNamedBufferSubData(
        m_DotProductBuffer, GL_R32F, 0u, max_elem_count * sizeof(GLfloat), GL_RED, GL_FLOAT, &clear_value
    );

    // Compute dot products of particles toward the camera.
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_DOT_PRODUCTS, m_DotProductBuffer);
    m_pComputeShaders.CalculateDp->activeShader();
    {
        /// @note No kernel boundaries check performed.
        unsigned int const num_groups = GetThreadsGroupCount(m_NumAliveParticles); 
        m_pComputeShaders.CalculateDp->setMat4UniformValue("uViewMatrix", glm::value_ptr(vViewMat));
        glDispatchCompute(num_groups, 1u, 1u);
    }
    glUseProgram(0u);

    /* Synchronize the indices and dotproducts buffers. */
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    /* 2) Sort particle indices through their dot products. */
    // [might be able to optimise early steps with one kernel, when max_block_width <= kernel_size]
    unsigned int const num_threads = max_elem_count / 2u;
    unsigned int const num_groups = GetThreadsGroupCount(num_threads);

    unsigned int const nsteps = GetNumTrailingBits(max_elem_count);
    unsigned int const indices_half_size = max_elem_count * sizeof(GLuint);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_DOT_PRODUCTS, m_DotProductBuffer);
    m_pComputeShaders.Sort->activeShader();
    unsigned int binding = 0u;
    for (unsigned int step = 0u; step < nsteps; ++step) {
        for (unsigned int stage = 0u; stage < step + 1u; ++stage) {
            // bind read / write indices buffers.
            unsigned int const offset_read = indices_half_size * binding;
            unsigned int const offset_write = indices_half_size * (binding ^ 1u);
            glBindBufferRange(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_INDICES_FIRST, m_SortIndicesBuffer, offset_read, indices_half_size);
            glBindBufferRange(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_INDICES_SECOND, m_SortIndicesBuffer, offset_write, indices_half_size);
            binding ^= 1u;

            // compute kernel parameters.
            GLuint const block_width = 2u << (step - stage);
            GLuint const max_block_width = 2u << step;

            m_pComputeShaders.Sort->setIntUniformValue("uBlockWidth", block_width);
            m_pComputeShaders.Sort->setIntUniformValue("uMaxBlockWidth", max_block_width);
            glDispatchCompute(num_groups, 1u, 1u);
            glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
        }
    }
    glUseProgram(0u);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_DOT_PRODUCTS, 0u);

    /* 3) Sort particles datas with their sorted indices. */
    // bind the sorted indices to the first binding slot.
    GLuint const sorted_offset_read = indices_half_size * binding;
    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_INDICES_FIRST, m_SortIndicesBuffer, sorted_offset_read, indices_half_size);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_INDICES_SECOND, 0u);

    m_pComputeShaders.SortFinal->activeShader();
    {
        /// @note could use the DispatchIndirect buffer.
        unsigned int const num_groups = GetThreadsGroupCount(m_NumAliveParticles);
        glDispatchCompute(num_groups, 1u, 1u);
    }
    glUseProgram(0u);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, STORAGE_BINDING_INDICES_FIRST, 0u);

}

//**************************************************************************************************
//FUNCTION:
void CParticleSystem::swapBuffer() {
    if (m_IsSimulated) {
        /* Swap atomic counter to have number of alive particles in the first slot */
        m_pAppendConsumeBuffer->swapAtomics();

        /* Copy non sorted alive particles back to the first buffer. */
        if (!m_EnableSorting) {
            m_pAppendConsumeBuffer->swapStorage();
        }
    }

    /* Copy the number of alive particles to the indirect buffer for drawing. */
    /// @note We use the update_args kernel instead, loosing 1 frame of accuracy.
    glCopyNamedBufferSubData(
        m_pAppendConsumeBuffer->getFirstAtomicBufferId(), m_IndirectBuffer, 0u, offsetof(TIndirectValues, DrawCount), sizeof(GLuint)
    );
}

//**************************************************************************************************
//FUNCTION:
void CParticleSystem::bindSimulationParameters()
{
 
    _WRONG(m_pMapSimulationParams == nullptr || m_pMapParticleProportion == nullptr, "Empty Ptr of Buffer！");

    std::memcpy(m_pMapSimulationParams, m_SimulationParamsSet.data(), sizeof(SSimulationParameters) * m_NumParticleTypes);
   
    unsigned int TotalEmitNumPerSecond = 0u;
    std::for_each(m_SimulationParamsSet.begin(), m_SimulationParamsSet.end(),
        [&TotalEmitNumPerSecond](auto SP) {TotalEmitNumPerSecond += SP.emit_num_per_second; });

    std::vector<float> ParticleProportion;
    for (SSimulationParameters& SP : m_SimulationParamsSet)
        ParticleProportion.push_back(1.0f * SP.emit_num_per_second / TotalEmitNumPerSecond);
    for (int i = 1; i < m_NumParticleTypes; i++)
        ParticleProportion[i] += ParticleProportion[i - 1];

    std::memcpy(m_pMapParticleProportion, ParticleProportion.data(), sizeof(float) * m_NumParticleTypes);
 
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, STORAGE_PARTICLE_PROPORTION, m_ParticleProportionBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, STORAGE_SIMULATE_PARAMETER, m_SimulationParamsBuffer);
   
}
//**************************************************************************************************
//FUNCTION:
void  CParticleSystem::unbindSimulationParameters()
{
    glBindBufferBase(GL_UNIFORM_BUFFER, STORAGE_SIMULATE_PARAMETER, 0u);
    glBindBufferBase(GL_UNIFORM_BUFFER, STORAGE_PARTICLE_PROPORTION, 0u);
}

//**************************************************************************************************
//FUNCTION:
unsigned int CParticleSystem::getBatchEmitNum(const float vDeltaTime)
{
    static float AccumulaticeTime = 0;

    AccumulaticeTime += vDeltaTime;

    float AccumulaticeCount = AccumulaticeTime * m_TotalEmitNumPerSecond;
    if (AccumulaticeCount >= PARTICLE_SYSTEM_KERNEL_GROUP_WIDTH)
    {
        unsigned int const num_dead_particles = m_MaxParticleCount - m_NumAliveParticles;
        unsigned int EmitNum = FloorParticleCount(unsigned int(AccumulaticeCount));
 
        EmitNum = std::min(EmitNum, std::min(m_MaxEmitCountPerBatch, num_dead_particles));

        AccumulaticeTime = (AccumulaticeCount - EmitNum)/ m_TotalEmitNumPerSecond;
        return EmitNum;
    }
    else
        return 0;
   
}
