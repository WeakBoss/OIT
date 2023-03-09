#pragma once
#include <GLM/glm.hpp>
#include <GL/glew.h>
class CVectorField {
public:
    CVectorField()
        : mTextureId(0u)
    {}

    void init(unsigned int const width, unsigned int const height, unsigned int const depth);
    void deinit();

    /// Generate vector datas.
    /// load them from filename if it exists, otherwise compute them and save on disk.
    void generateValues(char const* filename);

    inline GLuint texture_id() const {
        return mTextureId;
    }

private:
    glm::uvec3 m_dimensions;
    glm::vec3 m_position;
    GLuint mTextureId;

    glm::vec3 generateVector(glm::vec3 const& p) const;
};