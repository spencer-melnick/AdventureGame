#pragma once

#include <string>
#include <type_traits>

#include "../Utility/Hash.h"

namespace Engine
{
	namespace Memory
	{
		using ResourceType = HashValue;

		class Resource
		{
		public:
			Resource(const std::string& Filename, const ResourceType& Type);

			std::string GetFilename() const;
			ResourceType GetType() const;

			bool IsA(const ResourceType& Type) const;

		private:
			std::string Filename_;
			ResourceType Type_;
		};

		template <ResourceType T>
		class DerivedResource
		{
		public:
			DerivedResource(const std::string& Filename)
				: Resource(Filename, T)
			{};

			constexpr static ResourceType StaticType = T;
		};
	}
}