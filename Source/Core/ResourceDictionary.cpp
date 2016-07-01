#include "ResourceDictionary.h"

#include "../Utility/Logger.h"

using Logger = Engine::Utility::Logger;

namespace Engine
{
	void ResourceDictionary::RegisterExtension(std::string Extension, IResourceFactory& Factory)
	{
		auto Result = Extensions_.emplace(std::piecewise_construct, std::forward_as_tuple(Extension), std::forward_as_tuple(Factory));

		if (Result.second == false)
		{
			Logger::Warn("\"" + Extension + "\" is already a registered extension");
		}
	}

	IResourceFactory& ResourceDictionary::GetFactory(std::string Extension) const
	{
		auto Result = Extensions_.find(Extension);

		if (Result == Extensions_.end())
		{
			static EmptyResourceFactory DummyFactory;

			Logger::Warn("\"" + Extension + "\" is not a registered extension");
			return DummyFactory;
		}

		return Result->second;
	}

	ResourceDictionary& GetResourceDictionary()
	{
		static ResourceDictionary Dictionary;
		return Dictionary;
	}
}