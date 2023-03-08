#pragma once

#include "Interface.h"
#include "Utils.h"
#include "Common.h"
#include "Shader.h"
#include "InputManager.h"
#include "GUI.h"
#include "GameObject.h"
#include "RenderPass.h"
#include "ParticleSystemCommon.h"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace hiveGraphics;

enum RenderMode
{
    RENDERMODE_STRETCHED,
    RENDERMODE_POINTSPRITE,
    kNumRenderMode
};

enum ColorMode
{
    COLORMODE_DEFAULT,
    COLORMODE_GRADIENT,
    kNumColorMode
};

struct RenderingParameters_t
{
    RenderMode rendermode = RENDERMODE_POINTSPRITE;
    float stretched_factor = 10.0f;
    ColorMode colormode = COLORMODE_DEFAULT;
    float birth_gradient[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
    float death_gradient[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
    float min_size = 0.75f;
    float max_size = 25.0f;
    float fading_factor = 0.35f;
};