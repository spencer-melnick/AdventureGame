#pragma once

#include <string>
#include <map>

#include "../Utility/Hash.h"
#include "ResourceFactory.h"

namespace Engine
{
	using ResourceType = HashValue;

	// The ResourceDictionary keeps track of all file extensions,
	// and their associated types and factories
	class ResourceDictionary
	{
	public:
		void RegisterExtension(std::string Extension, IResourceFactory& Factory);
		IResourceFactory& GetFactory(std::string Extension) const;

	private:
		std::map<std::string, IResourceFactory&> Extensions_;
	};

	ResourceDictionary& GetResourceDictionary();
}