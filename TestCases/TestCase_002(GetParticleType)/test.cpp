 #include <gtest/gtest.h>
#include <random>
#include <vector>
typedef unsigned int uint;
std::random_device rd; // 用于随机数种子
std::mt19937 gen(rd()); // 随机数生成器
std::uniform_real_distribution<float> dis(0.0, 1.0); // 0 到 99 之间的分布器

float step(float edge, float value) {
    return (value < edge) ? 0.0 : 1.0;
}

class testGetParticleType : public testing::Test {

protected:
    void SetUp() override
    {

    }

    void TearDown() override
    {

    }
    uint getParticleType(uint vId)
    {
        const float single_rand = dis(gen);
        uint particle_type = 0;
        for (uint i = 0; i < uNumParticleType - 1; i++)
        {
            particle_type += step(particleProportion[i], single_rand);
        }
        return particle_type;
    }
    std::vector<float> particleProportion ;
    uint uNumParticleType;
    uint uNumParticles;
};
TEST_F(testGetParticleType, 5types)
{
     particleProportion =  { 0.1,0.2,0.3,0.1,0.3 };
     std::vector<float> tempparticleProportion = particleProportion;
     uNumParticleType = 5;
     uNumParticles = 90000;

    float numMuitiParticleType[5] = { 0,0,0,0,0 };
    for (uint i = 1; i < uNumParticleType; i++)
        particleProportion[i] += particleProportion[i - 1];
    for (uint i = 0; i < uNumParticles; i++)
        numMuitiParticleType[getParticleType(i)]++;
    for (auto& v : numMuitiParticleType)
        v /= uNumParticles;
    for (uint i = 0; i < uNumParticleType; i++)
    {
        float a = std::fabs(numMuitiParticleType[i] - tempparticleProportion[i]);
        ASSERT_LE(a, 0.01);
    }
}
TEST_F(testGetParticleType, 1types)
{
    particleProportion = { 1 };
    std::vector<float> tempparticleProportion = particleProportion;
    uNumParticleType = 1;
    uNumParticles = 90000;

    float numMuitiParticleType[1] = { 0};
    for (uint i = 1; i < uNumParticleType; i++)
        particleProportion[i] += particleProportion[i - 1];
    for (uint i = 0; i < uNumParticles; i++)
        numMuitiParticleType[getParticleType(i)]++;
    for (auto& v : numMuitiParticleType)
        v /= uNumParticles;
    for (uint i = 0; i < uNumParticleType; i++)
    {
        float a = std::fabs(numMuitiParticleType[i] - tempparticleProportion[i]);
        ASSERT_LE(a, 0.01);
    }
}

int main(int argc, char** argv) {
    printf("Running main() from %s\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}