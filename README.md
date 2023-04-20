# Axt Engine

Axt Engine = Abstract Engine. It's the best I could come up with.

An in development game engine/rendering platform for Windows.

## Getting Started

Visual Studio 2022 is recommended, as that's the IDE I use to make it in. Premake is used to generate project files.

1. Download the repository with `git clone --recursive https://github.com/dubtcs/AxtEngine.git`

2. Run `GENERATE.bat` to create VS2022 solution and project files.
    - You can use this any time you make changes to the premake5.lua file or just want new solutions.

## Goals

AxtEngine is being created as an excersise in graphics programming and game engine development.

### Current Features 
   - [Custom ECS](https://github.com/dubtcs/nECS)
   - 2D Rendering
   - Scene Graph / Game World instancing
      - Separates runtime and editor views
   - Project saving/loading

### Long term goals
   - 2D Physics
   - 3D Rendering
