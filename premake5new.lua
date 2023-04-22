
-- output location of build files
output = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}";

-- include directories
local include_root = "AxtEngine/vendor/"
local includes  = {};
includes.glfw    = include_root + "glfw2/include";
includes.glad    = include_root + "glad/include";
includes.imgui   = include_root + "imgui";
includes.glm     = include_root + "glm";
includes.std     = include_root + "stb";
includes.necs    = include_root + "necs3/include";
includes.yaml    = include_root + "yaml/include";
include.imguizmo = include_root + "ImGuizmo";

-- AxtEngine root workspace
workspace "AxtEngine"
    architecture "x64"
    startproject "Editor"

    configurations {"Debug", "Release", "Dist"}

    -- External vs projects for build
    group "Vendors"
        include "AxtEngine/vendor/glfw2"
        include "AxtEngine/vendor/glad"
        include "AxtEngine/vendor/imgui"
        include "AxtEngine/vendor/necs3"
        include "AxtEngine/vendor/yaml"
    group ""

    -- AxtEngine API Project
    project "AxtEngine"
        location "AxtEngine"
        kind "StaticLib"
        language "C++"
        cppdialect "C++20"
        staticruntime "On"

        targetdir ("bin/"..output.."/%{prj.name}")
        objdir ("bin-int/"..output.."/%{prj.name}")

        pchheader "pch.h"
        pchsource "AxtEngine/src/pch.cpp"

        files {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp",
            "%{prj.name}/vendor/glm/glm/**.hpp",
            "%{prj.name}/vendor/glm/glm/**.inl",
            "%{prj.name}/vendor/stb/stb_image.cpp",
            "%{prj.name}/vendor/stb/stb_image.h",
        
            "%{prj.name}/vendor/ImGuizmo/**.cpp",
            "%{prj.name}/vendor/ImGuizmo/**.h",
        }

        defines {
            "_CRT_SECURE_NO_WARNINGS",
        }

        excludes {
            "AxtEngine/src/axt/gui/ImGuiBuilder.cpp",
        }

        includedirs {
            "%{prj.name}/src",
            "%{prj.name}/vendor/spdlog/include",
            "%{includes.glfw}",
            "%{includes.glad}",
            "%{includes.imgui}",
            "%{includes.glm}",
            "%{includes.stb}",
            "%{includes.entt}",
            "%{includes.necs}",
            "%{includes.yaml}",
            "%{includes.imguizmo}",
        }

        links {
            "GLFW",
            "glad",
            "imgui",
            "necs",
            "opengl32.lib",
            "yaml"
        }

        filter "files:vendor/ImGuizmo/**.cpp"
            flags { "NoPCH" }

        filter "system:windows"
            systemversion "latest"

            defines {
                "AXT_BUILD_DLL",
                "GLFW_INCLUDE_NONE",
                "YAML_CPP_STATIC_DEFINE"
            }

        filter "configurations:Debug"
            defines {"AXT_DEBUG", "AXT_ENABLE_ASSERTS"}
            runtime "Debug"
            symbols "On"

        filter "configurations:Release"
            defines {"AXT_RELEASE", "AXT_ENABLE_ASSERTS"}
            runtime "Release"
            optimize "On"

        filter "configurations:Dist"
            defines {"AXT_DIST"}
            runtime "Release"
            optimize "On"