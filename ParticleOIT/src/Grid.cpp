#include "ParticleOITPCH.h"
#include "Grid.h"

//**************************************************************************************************
//FUNCTION:
void CGridObj::initV()
{
    // size taken in world space
    float const world_size = 1.0f;

    unsigned int const& res = 32u; //static_cast<unsigned int>(gpu_particle_->simulation_box_size()) / 2u;
    unsigned int const& num_vertices = 4u * (res + 1u);
    unsigned int const num_component = 2u;
    unsigned int const buffersize = num_vertices * num_component;
    std::vector<float> vertices(buffersize);

    m_Resolution = res;
    m_NumVertices = static_cast<GLsizei>(num_vertices); //

    float const cell_padding = world_size / res;
    float const offset = cell_padding * (res / 2.0f);

    for (unsigned int i = 0u; i <= res; ++i) {
        unsigned int const index = 4u * num_component * i;
        float const cursor = cell_padding * i - offset;

        // horizontal lines
        vertices[index + 0u] = -offset;
        vertices[index + 1u] = cursor;
        vertices[index + 2u] = +offset;
        vertices[index + 3u] = cursor;
        // vertical lines
        vertices[index + 4u] = cursor;
        vertices[index + 5u] = -offset;
        vertices[index + 6u] = cursor;
        vertices[index + 7u] = +offset;
    }

    // Allocate Storage.
    glGenBuffers(1u, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    GLsizeiptr const bytesize = static_cast<GLsizeiptr>(vertices.size() * sizeof(vertices[0u]));
    glBufferStorage(GL_ARRAY_BUFFER, bytesize, vertices.data(), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0u);

    // Set attribute locations.
    glGenVertexArrays(1u, &m_VAO);
    glBindVertexArray(m_VAO);
    {
        size_t const attrib_size = num_component * sizeof(vertices[0u]);
        glBindVertexBuffer(0, m_VBO, 0u, attrib_size);
        {
            size_t const attrib_pos = 0u;
            glVertexAttribFormat(attrib_pos, num_component, GL_FLOAT, GL_FALSE, 0);
            glVertexAttribBinding(attrib_pos, 0);
            glEnableVertexAttribArray(attrib_pos);
        }
    }
    glBindVertexArray(0u);
}
//**************************************************************************************************
//FUNCTION:
void CGridObj::updateV()
{

}
