#pragma once
#include "ParticleOITPCH.h"
#include "RandomBuffer.h"
class CParticleSystemObj :public IGameObject
{
public:
    CParticleSystemObj()
    {

    }
    void initV() override;
    void updateV() override;
private:
    CRandomBuffer b;
};