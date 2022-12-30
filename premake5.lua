
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
_includeDirs["glm"] = "AxtEngine/vendor/glm"

group "Vendors"
    include "AxtEngine/vendor/glfw"
    include "AxtEngine/vendor/glad"
    include "AxtEngine/vendor/imgui"
group ""

project "AxtEngine"
    location "AxtEngine"
    kind "SharedLib"
    language "C++"
    staticruntime "Off"

    targetdir ("bin/"..output.."/%{prj.name}")
    objdir ("bin-int/"..output.."/%{prj.name}")

    pchheader "pch.h"
    pchsource "AxtEngine/src/pch.cpp"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
    }

    includedirs {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{_includeDirs.glfw}",
        "%{_includeDirs.glad}",
        "%{_includeDirs.imgui}",
        "%{_includeDirs.glm}",

    }
    
    links {
        "GLFW",
        "glad",
        "imgui",
        "opengl32.lib",
    }

    filter "system:windows"
        cppdialect "C++20"
        systemversion "latest"

        defines {
            "AXT_BUILD_DLL";
            "AXT_PLATFORM_WINDOWS";
            "GLFW_INCLUDE_NONE";
        }

        postbuildcommands{
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/"..output.."/Sandbox")
        }

    filter "configurations:Debug"
        defines {"AXT_DEBUG"}
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines {"AXT_RELEASE"}
        runtime "Release"
        symbols "On"

    filter "configurations:Dist"
        defines {"AXT_DIST"}
        runtime "Release"
        symbols "On"

-- SANDBOX
project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    staticruntime "Off"

    targetdir ("bin/"..output.."/%{prj.name}")
    objdir ("bin-int/"..output.."/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "AxtEngine/vendor/glm/glm/**.hpp",
        "AxtEngine/vendor/glm/glm/**.inl"

    }

    includedirs {
        "AxtEngine/vendor/spdlog/include",
        "AxtEngine/src",
        "%{_includeDirs.glm}",

    }

    links {
        "AxtEngine"
    }

    filter "system:windows"
        cppdialect "C++20"
        systemversion "10.0.19041.0"

        defines {
            "AXT_PLATFORM_WINDOWS";
        }

	buildoptions "/MDd"
    
    filter "configurations:Debug"
        defines "AXT_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "AXT_RELEASE"
        runtime "Release"
        symbols "On"

    filter "configurations:Dist"
        defines "AXT_DIST"
        runtime "Release"
        symbols "On"