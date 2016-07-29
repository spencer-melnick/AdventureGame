#pragma once

#include "Resource.h"

#include "ResourceHandle.h"

namespace Engine
{
	namespace Memory
	{
		Resource::Resource(const std::string& Filename, const ResourceType& Type) :
			Filename_(Filename), Type_(Type)
		{}

		std::string Resource::GetFilename() const
		{
			return Filename_;
		}

		ResourceType Resource::GetType() const
		{
			return Type_;
		}

		bool Resource::IsA(const ResourceType& Type) const
		{
			return Type_ == Type;
		}
	}
}