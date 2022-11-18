workspace "Liquid"
    configurations { "Debug", "Release", "Shipping" }
    architecture "x64"
    flags "MultiProcessorCompile"
    startproject "Liquid"

    targetdir ("Build/Bin/%{cfg.buildcfg}/%{prj.name}")
    objdir ("Build/Obj/%{cfg.buildcfg}/%{prj.name}")

project "Liquid"
    language "C++"
    cppdialect "C++latest"
    location "Liquid"
    staticruntime "Off"

    pchheader "LiquidPCH.h"
    pchsource "%{prj.name}/Source/LiquidPCH.cpp"

    files
    {
        "%{prj.name}/Source/**.cpp",
        "%{prj.name}/Source/**.h"
    }

    includedirs
    {
        "%{prj.name}/Source",
        "%{prj.name}/Libraries/spdlog/include"
    }

    filter "system:windows"
        systemversion "latest"
        defines "LQ_PLATFORM_WINDOWS"

        links
        {
            "Winmm.lib",
            "Ws2_32.lib",
            "Version.lib",
            "Bcrypt.lib"
        }

    filter "system:macosx"
        systemversion "latest"
        defines "LQ_PLATFORM_MAC"

    filter "system:linux"
        systemversion "latest"
        defines "LQ_PLATFORM_LINUX"

    filter "configurations:Debug"
        kind "ConsoleApp"
        defines "LQ_BUILD_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        kind "ConsoleApp"
        defines { "LQ_BUILD_RELEASE", "NDEBUG" }
        runtime "Release"
        optimize "On"
    
    filter "configurations:Shipping"
        kind "WindowedApp"
        defines { "LQ_BUILD_SHIPPING", "NDEBUG" }
        runtime "Release"
        optimize "On"
        symbols "Off"