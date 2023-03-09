#pragma once
#include <random>

class CRandomBuffer
{
public:
    CRandomBuffer() :
        m_NumElements(0u),
        m_GLRandomBuffer(0u),
        m_Mt19937(m_RandomDevice()),
        m_Min(0.0f),
        m_Max(1.0f)
    {}
    void bind();
    void unbind();
    void init(unsigned int const vNumlements);
    void deinit();
    void generateValues();

private:
    float m_Min;
    float m_Max;
    unsigned int m_GLRandomBuffer;
    unsigned int m_NumElements;
    std::random_device m_RandomDevice;
    std::mt19937 m_Mt19937;
};