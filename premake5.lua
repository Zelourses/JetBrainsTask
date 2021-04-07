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
    kind "ConsoleApp"
    language "C++"

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
    --[[ bindirs {
        "{prj.name}/../includeLibs/"
    }
    libdirs {
        "%{prj.name}/../includeLibs/"
    } ]]
    links {
        "GLFW",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

    filter "configurations:Debug"
        defines {
        }
        symbols "On"
    
    filter "configurations:Release"
        defines {
        }
        optimize "On"
    filter "system:macosx"
        defines {
            "T_APPLE"
        }