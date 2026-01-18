workspace "Dark Matter"
    architecture "x64"
    startproject "App"

    configurations { "Debug", "Release", "Dist" }
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- ===============================
-- Core Library
-- ===============================
project "Core"
    location "Core"
    kind "StaticLib"
    language "C++"
    warnings "Extra"
    cppdialect "C++23"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    -- Include all headers and source recursively
    files {
        "Core/include/**.h",
        "Core/include/**.hpp",
        "Core/src/**.cpp",
        "Core/src/**.cxx",
        -- Optional: vendor headers in solution (read-only)
        "vendor/OpenGL/GLFW/include/glfw3.h.h",
        "vendor/OpenGL/GLEW-2.2/include/glew.h"
    }

    includedirs {
        "Core/include",
        "Core/vendor/glm",
        "vendor/OpenGL/GLFW/include",
        "vendor/OpenGL/GLEW-2.2/include"
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
        symbols "On"

    filter "configurations:Release"
        optimize "On"

    filter "configurations:Dist"
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

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    -- Include all headers, source, assets, and vendor headers
    files {
        "App/include/**.h",
        "App/include/**.hpp",
        "App/src/**.cpp",
        "App/src/**.cxx",
        "App/assets/**",
        "vendor/OpenGL/GLFW/include/glfw3.h",
        "vendor/OpenGL/GLEW-2.2/include/glew.h"
    }

    includedirs {
        "App/include",
        "Core/include",
        "Core/vendor/glm",
        "vendor/OpenGL/GLFW/include",
        "vendor/OpenGL/GLEW-2.2/include"
    }

    defines { "GLFW_STATIC", "GLEW_STATIC" }

    libdirs {
        "vendor/OpenGL/GLFW/lib-vc2022",
        "vendor/OpenGL/GLEW-2.2/lib/Release/x64"
    }

    links { "Core", "glew32s", "glfw3", "opengl32" }

    -- Copy assets folder to output directory after build
    postbuildcommands {
    '{COPYDIR} "%{prj.location}/assets" "%{cfg.targetdir}/assets"'
    }

    filter "configurations:Debug"
        symbols "On"
        libdirs { "vendor/OpenGL/GLEW-2.2/lib/Debug/x64" }

    filter "configurations:Release or configurations:Dist"
        optimize "On"
        libdirs { "vendor/OpenGL/GLEW-2.2/lib/Release/x64" }
