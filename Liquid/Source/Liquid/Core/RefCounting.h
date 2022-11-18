#pragma once

#include "PrimitiveTypes.h"

#ifdef LQ_PLATFORM_WINDOWS
#include <intrin.h>
#endif

namespace Liquid {

	class RefCounted
	{
	public:
		inline uint32 IncrementReferenceCount() const
		{
#ifdef LQ_PLATFORM_WINDOWS
			return static_cast<uint32>(::_InterlockedIncrement(reinterpret_cast<long*>(&m_RefCount)));
#else
			m_RefCount++;
			return m_RefCount;
#endif
		}

		inline uint32 DecrementReferenceCount() const
		{
#ifdef LQ_PLATFORM_WINDOWS
			return static_cast<uint32>(::_InterlockedDecrement(reinterpret_cast<long*>(&m_RefCount)));
#else
			m_RefCount--;
			return m_RefCount;
#endif
		}

		uint32 GetReferenceCount() const
		{
			return static_cast<uint32>(m_RefCount);
		}
	private:
		mutable int32 m_RefCount = 0;
	};

	template<typename ReferencedType, typename ReferencedBaseType = RefCounted>
	class Ref
	{
	public:
		Ref()
			: m_Reference(nullptr)
		{
		}

		Ref(std::nullptr_t)
			: m_Reference(nullptr)
		{
		}

		Ref(ReferencedType* reference)
			: m_Reference(reference)
		{
			IncrementReferenceCount();
		}

		Ref(const Ref<ReferencedType, ReferencedBaseType>& other)
			: m_Reference(other.m_Reference)
		{
			IncrementReferenceCount();
		}

		Ref(const Ref<ReferencedType, ReferencedBaseType>&& other)
		{
			m_Reference = other.m_Reference;
			other->m_Reference = nullptr;
		}

		template<typename TOther, typename TOtherBase>
		Ref(const Ref<TOther, TOtherBase>& other)
		{
			m_Reference = static_cast<ReferencedType*>(other.m_Reference);
			IncrementReferenceCount();
		}

		template<typename TOther, typename TOtherBase>
		Ref(Ref<TOther, TOtherBase>&& other)
		{
			m_Reference = static_cast<ReferencedType*>(other.m_Reference);
			other.m_Reference = nullptr;
		}

		~Ref()
		{
			DecrementReferenceCount();
		}

		Ref& operator=(std::nullptr_t)
		{
			DecrementReferenceCount();
			m_Reference = nullptr;
			return *this;
		}

		Ref& operator=(const Ref<ReferencedType>& other)
		{
			other.IncrementReferenceCount();
			DecrementReferenceCount();
			m_Reference = other.m_Reference;
			return *this;
		}

		template<typename TOther>
		Ref& operator=(const Ref<TOther>& other)
		{
			DecrementReferenceCount();
			m_Reference = other.m_Reference;
			other.m_Reference = nullptr;
			return *this;
		}

		operator bool() { return m_Reference != nullptr; }
		operator bool() const { return m_Reference != nullptr; }

		ReferencedType* operator->() { return m_Reference; }
		const ReferencedType* operator->() const { return m_Reference; }

		ReferencedType& operator*() { return *m_Reference; }
		const ReferencedType& operator*() const { return *m_Reference; }

		ReferencedType* Get() { return m_Reference; }
		const ReferencedType* Get() const { return m_Reference; }

		template<typename TOther>
		Ref<TOther, ReferencedBaseType> As() const
		{
			return Ref<TOther, ReferencedBaseType>(*this);
		}

		template<typename... Args>
		static Ref<ReferencedType, ReferencedBaseType> Create(Args&&... args)
		{
			return Ref<ReferencedType, ReferencedBaseType>(new ReferencedType(std::forward<Args>(args)...));
		}
	private:
		void IncrementReferenceCount() const
		{
			if (m_Reference)
				m_Reference->IncrementReferenceCount();
		}

		void DecrementReferenceCount() const
		{
			if (!m_Reference)
				return;

			m_Reference->DecrementReferenceCount();

			if (m_Reference->GetReferenceCount() == 0)
			{
				delete m_Reference;
				m_Reference = nullptr;
			}
		}
	private:
		mutable ReferencedType* m_Reference;

		template<class TOther, class TOtherBase>
		friend class Ref;
	};

}