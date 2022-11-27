#pragma once

namespace Liquid {

	class Texture : public RefCounted
	{
	public:
		virtual ~Texture() {}

		static Ref<Texture> Create(const String& filepath);
	};

}