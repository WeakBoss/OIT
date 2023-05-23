### 使用粒子系统进行烟雾模拟
实现基于 GPU 的粒子系统，使用 ShaderStorageBuffer 存储粒子的状态信息，并在
ComputeShader 中更新，充分利用 GPU 的并行计算能力。使用半透明烟雾粒子纹理，结合 Screen-Aligned 公告板实现烟雾效果。

![](gif/cloud.gif)