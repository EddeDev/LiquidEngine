#pragma once

namespace Liquid {

	enum class GraphicsDeviceVendor : uint32
	{
		None = 0,

		Nvidia = 0x10DE,
		AMD = 0x1002,
		Intel = 0x8086,
		Microsoft = 0x1414 // Microsoft Basic Render Driver
	};

	// TODO: capabilities

	struct GraphicsDeviceInfo
	{
		GraphicsDeviceVendor Vendor;

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