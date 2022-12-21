workspace "Liquid"
    configurations { "Debug", "Release", "Shipping" }
    architecture "x64"
    flags "MultiProcessorCompile"
    startproject "Liquid"

    targetdir ("Build/Bin/%{cfg.buildcfg}/%{prj.name}")
    objdir ("Build/Obj/%{cfg.buildcfg}/%{prj.name}")

VulkanSDKPath = os.getenv("VULKAN_SDK")

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
        "%{prj.name}/Source/**.h",

        "%{prj.name}/Libraries/GLM/glm/**.hpp",
        "%{prj.name}/Libraries/GLM/glm/**.inl"
    }

    includedirs
    {
        "%{prj.name}/Source",
        "%{prj.name}/Libraries/GLFW/include",
        "%{prj.name}/Libraries/Spdlog/include",
        "%{prj.name}/Libraries/ImGui",
        "%{prj.name}/Libraries/STB",
        "%{prj.name}/Libraries/GLM",

        -- Vulkan
        "%{VulkanSDKPath}/Include"
    }

    defines
    {
        "GLFW_INCLUDE_NONE",
        "_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS"
    }

    links
    {
        "GLFW",
        "ImGui",
        "STB",

        -- Vulkan
        "%{VulkanSDKPath}/Lib/vulkan-1.lib",
        "%{VulkanSDKPath}/Lib/VkLayer_utils.lib"
    }

    filter "system:windows"
        systemversion "latest"
        defines "LQ_PLATFORM_WINDOWS"

        defines
        {
            "LQ_PLATFORM_WINDOWS",
            "_CRT_SECURE_NO_WARNINGS"
        }

        links
        {
            "Winmm.lib",
            "Ws2_32.lib",
            "Version.lib",
            "Bcrypt.lib",
            "Urlmon.lib",

            "d3d11.lib",
            "d3d12.lib",
            "dxgi.lib",
            "dxguid.lib",
            "d3dcompiler.lib"
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

        links
        {
            -- ShaderC
            "%{VulkanSDKPath}/Lib/shaderc_sharedd.lib",

            -- SPIR-V
            "%{VulkanSDKPath}/Lib/spirv-cross-cored.lib",
            "%{VulkanSDKPath}/Lib/spirv-cross-hlsld.lib",
            "%{VulkanSDKPath}/Lib/spirv-cross-glsld.lib",
            "%{VulkanSDKPath}/Lib/SPIRV-Toolsd.lib"
        }

    filter "configurations:Release"
        kind "ConsoleApp"
        defines { "LQ_BUILD_RELEASE", "NDEBUG" }
        runtime "Release"
        optimize "On"

        links
        {
            -- ShaderC
            "%{VulkanSDKPath}/Lib/shaderc_shared.lib",

            -- SPIR-V
            "%{VulkanSDKPath}/Lib/spirv-cross-core.lib",
            "%{VulkanSDKPath}/Lib/spirv-cross-hlsl.lib",
            "%{VulkanSDKPath}/Lib/spirv-cross-glsl.lib"
        }
    
    filter "configurations:Shipping"
        kind "WindowedApp"
        defines { "LQ_BUILD_SHIPPING", "NDEBUG" }
        runtime "Release"
        optimize "On"
        symbols "Off"

        links
        {
            -- ShaderC
            "%{VulkanSDKPath}/Lib/shaderc_shared.lib",

            -- SPIR-V
            "%{VulkanSDKPath}/Lib/spirv-cross-core.lib",
            "%{VulkanSDKPath}/Lib/spirv-cross-hlsl.lib",
            "%{VulkanSDKPath}/Lib/spirv-cross-glsl.lib"
        }

group "Libraries"
    include "Liquid/Libraries/GLFW"
    include "Liquid/Libraries/ImGui"
    include "Liquid/Libraries/STB"
group ""