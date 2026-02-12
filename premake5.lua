workspace "Dark Matter"
    architecture "x64"
    startproject "App"

    configurations { "Debug", "Release", "Distribution" }
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- ===============================
-- Jolt Physics Engine
-- ===============================
project "Jolt"
    location "vendor/JoltPhysics"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "vendor/JoltPhysics/Jolt/**.cpp",
        "vendor/JoltPhysics/Jolt/**.h"
    }

    includedirs {
        "vendor/JoltPhysics"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"

    filter "configurations:Distribution"
        runtime "Release"
        optimize "On"

    -- ===============================
-- Core Library
-- ===============================
project "Core"
    location "Core"
    kind "StaticLib"
    language "C++"
    warnings "Extra"
    cppdialect "C++23"
    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    -- Include all headers and source recursively
    files {
        "Core/include/**.h",
        "Core/include/**.hpp",
        "Core/src/**.cpp",
        "Core/src/**.cxx",
        --"vendor/JoltPhysics/Jolt/**.h",
        --"vendor/JoltPhysics/Jolt/**.cpp"
    }

    includedirs {
        "Core/include",
        "vendor/OpenGL/GLFW/include",
        "vendor/OpenGL/GLEW-2.2/include",
        "vendor/OpenGL/glm",
        "vendor/JoltPhysics"
    }

    defines { "GLFW_STATIC", "GLEW_STATIC" }

    filter "system:windows"
        defines{ "DM_PLATFORM_WINDOWS" }

    filter "system:linux"
        defines{ "DM_PLATFORM_LINUX" }

    filter "system:macosx"
        defines{ "DM_PLATFORM_MACOS" }

    filter "configurations:Debug"
        defines{"CORE_ENABLE_ASSERTS"}
        libdirs {
            "vendor/OpenGL/GLFW/lib-vc2022",
            "vendor/OpenGL/GLEW-2.2/lib/Release/x64",
            "vendor/JoltPhysics/Build/VS2022_CL/Debug"
        }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        libdirs {
            "vendor/OpenGL/GLFW/lib-vc2022",
            "vendor/OpenGL/GLEW-2.2/lib/Release/x64",
            "vendor/JoltPhysics/Build/VS2022_CL/Release"
        }
        runtime "Release"
        optimize "On"

    filter "configurations:Distribution"
        libdirs {
            "vendor/OpenGL/GLFW/lib-vc2022",
            "vendor/OpenGL/GLEW-2.2/lib/Release/x64",
            "vendor/JoltPhysics/Build/VS2022_CL/Distribution"
        }
        runtime "Release"
        optimize "On"

-- ===============================
-- App Executable
-- ===============================
project "App"
    location "App"
    kind "ConsoleApp"
    language "C++"
    warnings "Extra"
    cppdialect "C++23"
    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    -- Include all headers, source, assets, and vendor headers
    files {
        "App/include/**.h",
        "App/include/**.hpp",
        "App/src/**.cpp",
        "App/src/**.cxx",
        "App/assets/**"
    }

    includedirs {
        "App/include",
        "Core/include",
        "vendor/OpenGL/GLFW/include",
        "vendor/OpenGL/GLEW-2.2/include",
        "vendor/OpenGL/glm",
        "vendor/JoltPhysics"
    }

    defines { "GLFW_STATIC", "GLEW_STATIC" }

    links { "Core", "glew32s", "glfw3_mt", "opengl32", "Jolt" }

    -- Copy assets folder to output directory after build
    postbuildcommands {
    '{COPYDIR} "%{prj.location}/assets" "%{cfg.targetdir}/assets"'
    }

    filter "configurations:Debug"
        libdirs {
            "vendor/OpenGL/GLFW/lib-vc2022",
            "vendor/OpenGL/GLEW-2.2/lib/Release/x64",
            "vendor/JoltPhysics/Build/VS2022_CL/Debug"
        }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        libdirs {
            "vendor/OpenGL/GLFW/lib-vc2022",
            "vendor/OpenGL/GLEW-2.2/lib/Release/x64",
            "vendor/JoltPhysics/Build/VS2022_CL/Release"
        }
        runtime "Release"
        optimize "On"

    filter "configurations:Distribution"
        libdirs {
            "vendor/OpenGL/GLFW/lib-vc2022",
            "vendor/OpenGL/GLEW-2.2/lib/Release/x64",
            "vendor/JoltPhysics/Build/VS2022_CL/Distribution"
        }
        runtime "Release"
        optimize "On"
