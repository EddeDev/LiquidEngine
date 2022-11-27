#pragma once

namespace Liquid {

	enum class GraphicsDeviceVendor : uint32
	{
		None = 0,

		AMD = 0x1002,
		Intel = 0x8086,
		Nvidia = 0x10DE,
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

	class GraphicsDevice : public RefCounted
	{
	public:
		virtual ~GraphicsDevice() {}

		virtual const GraphicsDeviceInfo& GetInfo() const = 0;

		static Ref<GraphicsDevice> Select(const GraphicsDeviceCreateInfo& createInfo);
	};

	namespace GraphicsDeviceUtils {

		static const char* VendorToString(GraphicsDeviceVendor vendor)
		{
			switch (vendor)
			{
			case GraphicsDeviceVendor::Nvidia:    return "NVIDIA";
			case GraphicsDeviceVendor::AMD:       return "AMD";
			case GraphicsDeviceVendor::Intel:     return "Intel";
			case GraphicsDeviceVendor::Microsoft: return "Microsoft";
			}
			return "UnknownDeviceVendor";
		}

	}

}