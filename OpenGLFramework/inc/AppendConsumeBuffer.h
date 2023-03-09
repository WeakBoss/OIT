#pragma once
class AppendConsumeBuffer {
public:
    AppendConsumeBuffer(unsigned int const element_count, unsigned int const attrib_buffer_count)
        :m_StorageBufferSize(element_count* sizeof(float) * 4u* attrib_buffer_count), //总的字节数量
        m_StorageBuffers{ 0u, 0u },
        m_AtomicBuffers{ 0u, 0u }
    {}

    void init();
    void deinit();

    void bind();
    void unbind();

    void bindAttributes();
    void unbindAttributes();
    void bindAtomics();
    void unbindAtomics();

    void swapAtomics();
    void swapStorage();


    unsigned int getNumAliveParticlesFromDevice() const;
 
    unsigned int getFirstStorageBuffer() const { return m_StorageBuffers[0]; }
    unsigned int getSecondStorageBufferId() const { return m_StorageBuffers[1]; }

    unsigned int getFirstAtomicBufferId() const { return m_AtomicBuffers[0]; }
    unsigned int getSecondAtomicBufferId() const { return m_AtomicBuffers[1]; }

private:
    unsigned int const m_StorageBufferSize;            
    unsigned int m_StorageBuffers[2u];                 
    unsigned int m_AtomicBuffers[2u];               
};