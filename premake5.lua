workspace "JetbrainsTask"
    architecture "x64"

    configurations {
        "Debug",
        "Release"
    }

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

includeDir = {}
includeDir["GLFW"] = "JetbrainsTask/vendor/glfw/include"

include "JetbrainsTask/vendor/glfw"

project "JetbrainsTask"
    location "JetbrainsTask"
    language "C++"
    cppdialect "C++17"
    systemversion "latest"

    targetdir ("out/"..outputDir.. "/%{prj.name}")
    objdir ("out/build/"..outputDir.. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.c"
    }

    includedirs {
        "%{prj.name}/src",
        "%{prj.name}/src/vendor",
        "./vendorInclude",
        "%{includeDir.GLFW}"
    }
    links {
        "GLFW"
    }

    filter "system:windows"
        buildoptions {
            "/Zc:__cplusplus" --for __cplusplus macro work
        }
        links {
            "opengl32.lib"
        }
        linkoptions {
            --https://docs.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2010/f9t8842e(v=vs.100)
            "/ENTRY:mainCRTStartup" -- Wow, microsoft
        }

    filter "system:macosx"
        --Sreiosly, Apple? Why
        linkoptions {
            "-framework OpenGL",
            "-framework Cocoa",
            "-framework IOKit",
            "-framework CoreVideo"
        }
        
    filter "system:linux"
        buildoptions {
            "GL",
            "X11",
            "pthread",
            "glfw3",
            "Xrandr"
        }

    filter "configurations:Debug"
        defines {
        }
        symbols "On"
        kind "ConsoleApp"
    
    filter "configurations:Release"
        defines {
        }
        optimize "On"
        kind "WindowedApp"
    filter "system:macosx"
        defines {
            "T_APPLE"
        }