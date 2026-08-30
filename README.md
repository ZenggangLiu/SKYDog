
                                                                                        
        *          .               *                              .               *     
        ███████╗██╗  ██╗██╗   ██╗        ██████╗  ██████╗  ██████╗         *            
        ██╔════╝██║ ██╔╝╚██╗ ██╔╝        ██╔══██╗██╔═══██╗██╔════╝                      
        ███████╗█████╔╝  ╚████╔╝         ██║  ██║██║   ██║██║  ███╗        .            
        ╚════██║██╔═██╗   ╚██╔╝          ██║  ██║██║   ██║██║   ██║                     
        ███████║██║  ██╗    ██║           ██████╔╝╚██████╔╝╚██████╔╝         *          
        ╚══════╝╚═╝  ╚═╝    ╚═╝           ╚═════╝  ╚═════╝  ╚═════╝                     
                                                                                        
        <~~~               .        SKY Dog Game                      ~~~>        *     
                                Real-Time | Cross-Platform           .                  
                                                                                        
                                                                                        
----------------------------------------------------------------------------------------
                                                                                        
                                                                                        
                                                                                        
                                  ,,                                                    
                  __           o-°°|\_____/)                                            
    Author:   (___()'`; Zee...  \_/|_)     )                                            
              /,    /`             \  __  /                                             
              \\"--\\              (_/ (_/                                              
    Created:  30/08/26  @  10:16 PM
    FileName: README.md @ RedSoUL Project
    History:
             - created by: 30/08/26: Zenggang LIU


# SKY-DoG

## 中文

### 项目简介

**SKY-DoG** 是一个用于研究、开发和验证**跨平台实时渲染技术（Cross-Platform Real-Time Rendering）**的实验性项目。

项目通过一个设计简单的 3D 游戏 **SKY-DoG** 作为实时渲染测试平台，在实际游戏场景中实现和验证现代图形渲染技术，而不是仅通过独立的 Rendering Demo 展示效果。

项目主要关注以下几个方向：

- **跨平台渲染（Cross-Platform Rendering）**
- **PBR 材质系统（Physically Based Rendering）**
- **Neural Graphics**
- **实时光线追踪（Real-Time Ray Tracing）**

SKY-DoG 游戏本身保持相对简单，使项目能够将重点集中在底层实时渲染技术、图形算法以及不同平台上的实现与性能表现。

---

## 主要目标

### 1. 跨平台渲染

构建能够运行于不同操作系统和 Graphics API 上的实时渲染框架，并尽可能保持统一的渲染架构和视觉结果。

主要包括：

- Cross-Platform Rendering Abstraction
- Cross-Platform Shader Support
- GPU Resource Management
- 不同 Graphics API 之间的渲染一致性
- Platform-Specific Performance Optimization

---

### 2. PBR 材质

实现基于物理的实时材质与光照系统，包括：

- Metallic / Roughness Workflow
- Cook-Torrance BRDF
- Image-Based Lighting (IBL)
- HDR Rendering
- Tone Mapping
- Environment Lighting
- Physically Based Material Parameters

目标是在不同平台和 Graphics API 上获得尽可能一致的 PBR 渲染结果。

---

### 3. Neural Graphics

探索机器学习技术在实时图形渲染中的应用。

可能研究的方向包括：

- Neural Sky
- Neural Global Illumination
- Neural PBR
- Neural Rendering Optimization

Neural Graphics 将作为传统实时渲染算法的补充，而不是完全替代传统 Rendering Pipeline。

---

### 4. 实时光线追踪

探索 Ray Tracing 与传统 Rasterization Pipeline 的结合，包括：

- Real-Time Ray Tracing
- Ray-Traced Shadows
- Ray-Traced Reflections
- Ray-Traced Global Illumination
- Hybrid Rasterization / Ray Tracing

重点研究在实时性能限制下，如何合理组合：

**Rasterization + Ray Tracing + Neural Graphics**

---

## SKY-DoG 游戏

SKY-DoG 是该项目使用的测试游戏。

游戏本身不是项目的主要目标，而是用于提供一个具有真实运行环境的实时渲染测试平台。

通过游戏场景可以测试：

- Dynamic Objects
- Character Rendering
- PBR Materials
- Dynamic Lighting
- Outdoor Environments
- GPU Performance
- Ray Tracing
- Neural Graphics

相比独立的 Graphics Sample，这种方式能够提供更加接近真实游戏引擎和实时应用的工作负载。

---

## 项目状态

🚧 **Work in Progress**

SKY-DoG 目前仍处于开发阶段。

项目将随着实时渲染技术的实现逐步扩展。

---

## 授权协议

本项目采用 **GNU General Public License v3.0 (GPL-3.0)** 开源。

你可以：

- 使用本项目代码
- 修改本项目代码
- 分发本项目代码

如果你分发使用、修改或派生自本项目 GPL 代码的软件，则必须按照 GPL-3.0 的要求提供相应源代码，并遵守 GPL-3.0 的许可条款。

详细授权条款请参阅项目中的 `LICENSE` 文件。

---

# English

## Overview

**SKY-DoG** is an experimental project for researching, developing, and evaluating **cross-platform real-time rendering technologies**.

The project uses a deliberately simple 3D game, **SKY-DoG**, as a practical real-time rendering testbed. Instead of demonstrating graphics techniques only through isolated rendering samples, SKY-DoG integrates them into an actual interactive environment.

The project primarily focuses on:

- **Cross-Platform Rendering**
- **Physically Based Rendering (PBR)**
- **Neural Graphics**
- **Real-Time Ray Tracing**

The game itself is intentionally kept relatively simple so that development can remain focused on rendering architecture, graphics algorithms, GPU technologies, and their performance across different platforms.

---

## Goals

### 1. Cross-Platform Rendering

Build a real-time rendering framework capable of running across different operating systems and graphics APIs while maintaining a consistent rendering architecture and visual output.

Areas of interest include:

- Cross-Platform Rendering Abstraction
- Cross-Platform Shader Support
- GPU Resource Management
- Rendering Consistency Across Graphics APIs
- Platform-Specific Performance Optimization

---

### 2. Physically Based Rendering

Implement a physically based real-time material and lighting system, including:

- Metallic / Roughness Workflow
- Cook-Torrance BRDF
- Image-Based Lighting (IBL)
- HDR Rendering
- Tone Mapping
- Environment Lighting
- Physically Based Material Parameters

One of the goals is to maintain consistent PBR results across different platforms and graphics APIs.

---

### 3. Neural Graphics

Explore the application of machine learning techniques to real-time computer graphics.

Potential research areas include:

- Neural Sky
- Neural Global Illumination
- Neural PBR
- Neural Rendering Optimization

Neural Graphics is intended to complement the traditional real-time rendering pipeline rather than completely replace it.

---

### 4. Real-Time Ray Tracing

Explore the integration of ray tracing with traditional rasterization techniques, including:

- Real-Time Ray Tracing
- Ray-Traced Shadows
- Ray-Traced Reflections
- Ray-Traced Global Illumination
- Hybrid Rasterization / Ray Tracing

A major focus is investigating practical ways to combine:

**Rasterization + Ray Tracing + Neural Graphics**

under real-time performance constraints.

---

## SKY-DoG Game

SKY-DoG is the test game used by the project.

The game itself is not the primary goal. Instead, it provides a practical environment for evaluating rendering technologies under realistic real-time workloads.

The game environment can be used to test:

- Dynamic Objects
- Character Rendering
- PBR Materials
- Dynamic Lighting
- Outdoor Environments
- GPU Performance
- Ray Tracing
- Neural Graphics

Compared with isolated graphics samples, this approach provides workloads that are closer to those found in real game engines and interactive rendering applications.

---

## Project Status

🚧 **Work in Progress**

SKY-DoG is currently under active development.

The project will gradually expand as additional rendering technologies and experiments are implemented.

---

## License

This project is open source and licensed under the **GNU General Public License v3.0 (GPL-3.0)**.

You are free to:

- Use the source code
- Modify the source code
- Redistribute the source code

However, if you distribute software that uses, modifies, or derives from GPL-covered code from this project, you must provide the corresponding source code and comply with the terms of the GPL-3.0 license.

See the `LICENSE` file for the complete license terms.