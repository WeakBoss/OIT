#ifndef PARTICLE_SYSTEM_COMMON_H
#define PARTICLE_SYSTEM_COMMON_H


#define MAX_NUM_PARTICLES   (1u << 18u)
#define PARTICLE_SYSTEM_KERNEL_GROUP_WIDTH       (256u) //16*16         待发射数量达到256才会一次性发射，因此若发射速率太小，调小此参数
#define MAX_NUM_PARTICLES_PER_BATCH             std::max(256u, (MAX_NUM_PARTICLES >> 4u))


#define STORAGE_BINDING_PARTICLES_FIRST                  0
#define STORAGE_BINDING_PARTICLES_SECOND                 1
#define STORAGE_BINDING_RANDOM_VALUES                    2
#define STORAGE_BINDING_INDIRECT_ARGS                    3

#define STORAGE_BINDING_DOT_PRODUCTS                     4
#define STORAGE_BINDING_INDICES_FIRST                    5
#define STORAGE_BINDING_INDICES_SECOND                   6

#define STORAGE_SIMULATE_PARAMETER                       7
#define STORAGE_PARTICLE_PROPORTION                      8

#define ATOMIC_COUNTER_BINDING_FIRST                     0
#define ATOMIC_COUNTER_BINDING_SECOND                    1



#ifndef SPARKLE_USE_SOA_LAYOUT
#define SPARKLE_USE_SOA_LAYOUT              FALSE
#endif

#endif

