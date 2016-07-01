#include "ResourceFactory.h"

namespace Engine
{
	void* EmptyResourceFactory::Load(std::string Filename) const
	{
		return nullptr;
	}

	void EmptyResourceFactory::Unload(void* Data) const
	{
	}

	HashValue EmptyResourceFactory::GetType() const
	{
		return Utility::GetHashedString("NULL");
	}
}