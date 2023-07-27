# Axt Editor

Axt Editor = Abstract Editor. It's the best I could come up with.

A simple 3D scene editor. It allows you to place and edit cubes and planes.

Progress screenshots can be found in the [media folder.](https://github.com/dubtcs/AxtEngine/tree/main/media)

<details>
<summary>Screenshot</summary>
![Latest Screenshot](media/Screenshot%202023-05-10.png)
</details>

## Download + Build

Visual Studio 2022 is recommended, as that's the IDE I use to make it in. Premake is used to generate project files.

1. Download the repository with `git clone --recursive https://github.com/dubtcs/AxtEngine.git`

2. Run `GENERATE.bat` to create VS2022 solution and project files.
    - You can use this any time you make changes to the premake5.lua file or just want new solutions.

## Goals

AxtEngine is being created as an excersise in graphics programming and game engine development.

### Current Features 
   - [Custom ECS](https://github.com/dubtcs/nECS)
   - 3D + 2D Rendering
   - Scene Graph / Game World instancing
   - Project saving/loading

### Future Development
   - Further development of the rendering engine has been moved to [Next Engine](https://github.com/dubtcs/Next)
