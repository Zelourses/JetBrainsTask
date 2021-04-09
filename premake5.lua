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
    cppdialect "C++11"
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
        links {
            "opengl32.lib"
        }

    filter "system:macosx"
        linkoptions {
            "-framework OpenGL",
            "-framework Cocoa",
            "-framework IOKit",
            "-framework CoreVideo"
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