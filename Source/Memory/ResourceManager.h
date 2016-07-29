#pragma once

#include <string>
#include <unordered_map>
#include <type_traits>

#include "../Utility/Hash.h"
#include "StackAllocator.h"
#include "Resource.h"
#include "ResourceHandle.h"

namespace Engine
{
	namespace Memory
	{
		class ResourceManager
		{
		public:
			template <typename T>
			ResourceHandle<T> GetResource(std::string Filename)
			{
				ResourceMap::iterator Search = Resources_.find(Filename)
				return ResourceHandle<T>(Filename, );
			}

		private:
			StackAllocator Allocator_;
		};
	}
}