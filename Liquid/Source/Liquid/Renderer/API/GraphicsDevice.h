#pragma once

#include "Liquid/Renderer/RenderResource.h"

namespace Liquid {

	enum class GraphicsDeviceVendor
	{
		None = 0,

		Nvidia = 0x10DE,
		AMD = 0x1002,
		Intel = 0x8086,
		Apple = 0x106B,
		ImgTec = 0x1010,
		ARM = 0x13B5,
		Broadcom = 0x14E4,
		Qualcomm = 0x5143,
		Vivante = 0x7a05,
		VeriSilicon = 0x1EB1,

		// Vulkan Vendor IDs
		Kazan = 0x10003,
		Codeplay = 0x10004,
		Mesa = 0x10005,

		// Microsoft Basic Render Driver
		Microsoft = 0x1414
	};

	struct GraphicsDeviceCapabiltiies
	{
		uint32 MaxSamplers;
	};

	struct GraphicsDeviceInfo
	{
		GraphicsDeviceVendor Vendor;
		String Renderer;
		String PlatformVersion;

		uint64 DedicatedVideoMemory;
		uint64 DedicatedSystemMemory;
		uint64 SharedSystemMemory;
	};

	struct GraphicsDeviceCreateInfo
	{
		bool EnableDebugLayers = false;
	};

	class GraphicsDevice : public RenderResource
	{
	public:
		virtual const GraphicsDeviceInfo& GetInfo() const = 0;

		RENDER_RESOURCE_TYPE(GraphicsDevice);

		static Ref<GraphicsDevice> Select(const GraphicsDeviceCreateInfo& createInfo);
	};

	namespace GraphicsDeviceUtils {

		static const char* VendorToString(GraphicsDeviceVendor vendor)
		{
			switch (vendor)
			{
			case GraphicsDeviceVendor::Nvidia:      return "NVIDIA";
			case GraphicsDeviceVendor::AMD:         return "AMD";
			case GraphicsDeviceVendor::Intel:       return "Intel";
			case GraphicsDeviceVendor::Apple:       return "Apple";
			case GraphicsDeviceVendor::ImgTec:      return "ImgTec";
			case GraphicsDeviceVendor::ARM:         return "ARM";
			case GraphicsDeviceVendor::Broadcom:    return "Broadcom";
			case GraphicsDeviceVendor::Qualcomm:    return "Qualcomm";
			case GraphicsDeviceVendor::Vivante:     return "Vivante";
			case GraphicsDeviceVendor::VeriSilicon: return "VeriSilicon";
			case GraphicsDeviceVendor::Microsoft:   return "Microsoft";
			}
			return "UnknownDeviceVendor";
		}

	}

}