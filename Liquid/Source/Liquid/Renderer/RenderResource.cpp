#include "LiquidPCH.h"
#include "RenderResource.h"

#include "RenderThread.h"

namespace Liquid {

	RenderResourceMap RenderResourceManager::s_GlobalReferences;

	uint32 RenderResourceManager::GetResourceCount()
	{
		uint32 result = 0;
		for (const auto& [type, list] : s_GlobalReferences)
			result += static_cast<uint32>(list.size());
		return result;
	}

	uint32 RenderResourceManager::GetResourceCount(RenderResourceType type)
	{
		if (s_GlobalReferences.find(type) == s_GlobalReferences.end())
			return 0;

		RenderResourceList& list = s_GlobalReferences.at(type);
		if (list.empty())
		{
			LQ_CHECK(false);
			return 0;
		}

		return static_cast<uint32>(list.size());
	}

	bool RenderResourceManager::HasResource(RenderResource* resource)
	{
		for (const auto& [type, list] : s_GlobalReferences)
		{
			if (std::find(list.begin(), list.end(), resource) != list.end())
				return true;
		}
		return false;
	}

	void RenderResourceManager::OnResourceAdded(RenderResource* resource)
	{
		LQ_ASSERT(!HasResource(resource), "Render resource already exists!");

		s_GlobalReferences[resource->GetType()].push_back(resource);
	}

	RenderResource::RenderResource()
	{
		RenderResource* instance = this;
		RT_SUBMIT(RenderResource)([instance]()
		{
			instance->m_Time = std::time(nullptr);
			RenderResourceManager::OnResourceAdded(instance);
		});
	}

}