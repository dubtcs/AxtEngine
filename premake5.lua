
workspace "AxtEngine"
    architecture "x64"
    startproject "Sandbox"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

local output = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

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
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/src"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "10.0.19041.0"

        defines {
            "AXT_BUILD_DLL";
            "AXT_PLATFORM_WINDOWS";
        }

        postbuildcommands{
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/"..output.."/Sandbox")
        }

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

    filter "configurations:Debug"
        defines "AXT_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "AXT_RELEASE"
        symbols "On"

    filter "configurations:Dist"
        defines "AXT_DIST"
        symbols "On"