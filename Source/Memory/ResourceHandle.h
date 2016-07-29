#pragma once

#include <string>
#include <type_traits>
#include <unordered_map>

#include "Resource.h"

namespace Engine
{
	namespace Memory
	{
		using ResourceMap = std::unordered_map<std::string, Resource*, Utility::HashFunction>;

		class ResourceManager;

		template <typename T, typename = std::enable_if<std::is_base_of<Resource, T>::value> >
		class ResourceHandle
		{
			friend class ResourceManager;

		public:
			ResourceHandle() :
				Map_(nullptr)
			{};

			ResourceHandle(const ResourceHandle& Handle) :
				Key_(Handle.Key_), Map_(Handle.Map_)
			{};

			~ResourceHandle();

			bool IsValid()
			{
				return (GetData_() == nullptr) ? false : true;
			}

			T* GetData()
			{
				return static_cast<T*>(GetData_());
			}

		protected:
			ResourceHandle(std::string Key, ResourceMap* Map, ResourceMap::iterator Iterator) :
				Key_(Key), Map_(Map)
			{
				if (!Iterator->second->IsA(T::StaticType))
				{
					Map_ = nullptr;
				}
			}

			Resource* GetData_()
			{
				if (Map_ != nullptr)
				{
					auto Search = Map_->find(Key_);

					if (Search != Map_->end())
					{
						return Search->second;
					}
				}

				return nullptr;
			}
			
			bool InitializedValid_;
			HashValue Key_;
			ResourceMap* Map_;
		};
	}
}