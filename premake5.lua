
workspace "AxtEngine"
    architecture "x64"
    startproject "Editor"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

output = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--include "AxtEngine/vendor/glfw"

_includeDirs = {};
_includeDirs.glfw = "AxtEngine/vendor/glfw/include"
_includeDirs.glad = "AxtEngine/vendor/glad/include"
_includeDirs.imgui = "AxtEngine/vendor/imgui"
_includeDirs.glm = "AxtEngine/vendor/glm"
_includeDirs.stb = "AxtEngine/vendor/stb"
_includeDirs.entt = "AxtEngine/vendor/entt/single_include"

group "Vendors"
    include "AxtEngine/vendor/glfw"
    include "AxtEngine/vendor/glad"
    include "AxtEngine/vendor/imgui"
group ""

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
        "%{_includeDirs.glfw}",
        "%{_includeDirs.glad}",
        "%{_includeDirs.imgui}",
        "%{_includeDirs.glm}",
        "%{_includeDirs.stb}",
        "%{_includeDirs.entt}",
    }
    
    links {
        "GLFW",
        "glad",
        "imgui",
        "opengl32.lib",
    }

    filter "system:windows"
        systemversion "latest"

        defines {
            "AXT_BUILD_DLL";
            --"AXT_PROFILING_ENABLED";
            "GLFW_INCLUDE_NONE";
        }

        -- postbuildcommands{
        --     ("{COPY} %{cfg.buildtarget.relpath} ../bin/"..output.."/Sandbox")
        -- }

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

-- SANDBOX
project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/"..output.."/%{prj.name}")
    objdir ("bin-int/"..output.."/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        -- "AxtEngine/vendor/glm/glm/**.hpp",
        -- "AxtEngine/vendor/glm/glm/**.inl",
        --"%{_includeDirs.imgui}",

    }

    includedirs {
        "AxtEngine/vendor/spdlog/include",
        "AxtEngine/src",
        "%{_includeDirs.glm}",
        "%{_includeDirs.imgui}",
        "%{_includeDirs.glad}",

    }

    links {
        "AxtEngine"
    }

    filter "system:windows"
        systemversion "latest"

        defines {
            --"AXT_PLATFORM_WINDOWS";
        }

    filter "configurations:Debug"
        defines "AXT_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "AXT_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "AXT_DIST"
        runtime "Release"
        optimize "On"
-- SANDBOX

-- EDITOR
project "Editor"
    location "Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/"..output.."/%{prj.name}")
    objdir ("bin-int/"..output.."/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs {
        "AxtEngine/vendor/spdlog/include",
        "AxtEngine/src",
        "%{_includeDirs.glm}",
        "%{_includeDirs.imgui}",
        "%{_includeDirs.glad}",
        "%{_includeDirs.entt}",
    }

    links {
        "AxtEngine"
    }

    filter "system:windows"
        systemversion "latest"

        defines {
            
        }

    filter "configurations:Debug"
        defines "AXT_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "AXT_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "AXT_DIST"
        runtime "Release"
        optimize "On"
-- EDITOR
