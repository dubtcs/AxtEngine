
workspace "AxtEngine"
    architecture "x64"
    startproject "Sandbox"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

output = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--include "AxtEngine/vendor/glfw"

_includeDirs = {};
_includeDirs["glfw"] = "AxtEngine/vendor/glfw/include"
_includeDirs["glad"] = "AxtEngine/vendor/glad/include"
_includeDirs["imgui"] = "AxtEngine/vendor/imgui"

include "AxtEngine/vendor/glfw"
include "AxtEngine/vendor/glad"
include "AxtEngine/vendor/imgui"

project "AxtEngine"
    location "AxtEngine"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/"..output.."/%{prj.name}")
    objdir ("bin-int/"..output.."/%{prj.name}")

    pchheader "pch.h"
    pchsource "AxtEngine/src/pch.cpp"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{_includeDirs.glfw}",
        "%{_includeDirs.glad}",
        "%{_includeDirs.imgui}"
    }
    
    links {
        "GLFW",
        "glad",
        "imgui",
        "opengl32.lib",
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines {
            "AXT_BUILD_DLL";
            "AXT_PLATFORM_WINDOWS";
            "GLFW_INCLUDE_NONE";
        }

        postbuildcommands{
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/"..output.."/Sandbox")
        }

	buildoptions "/MDd"

    filter "configurations:Debug"
        defines {"AXT_DEBUG"}
        symbols "On"

    filter "configurations:Release"
        defines {"AXT_RELEASE"}
        symbols "On"

    filter "configurations:Dist"
        defines {"AXT_DIST"}
        symbols "On"

-- SANDBOX
project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/"..output.."/%{prj.name}")
    objdir ("bin-int/"..output.."/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "AxtEngine/vendor/spdlog/include",
        "AxtEngine/src"
    }

    links {
        "AxtEngine"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "10.0.19041.0"

        defines {
            "AXT_PLATFORM_WINDOWS";
        }

	buildoptions "/MDd"
    
    filter "configurations:Debug"
        defines "AXT_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "AXT_RELEASE"
        symbols "On"

    filter "configurations:Dist"
        defines "AXT_DIST"
        symbols "On"