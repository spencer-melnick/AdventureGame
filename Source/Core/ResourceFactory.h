#pragma once

#include <string>

#include "../Utility/Hash.h"

namespace Engine
{
	class IResourceFactory
	{
	public:
		virtual void* Load(std::string Filename) const = 0;
		virtual void Unload(void* Data) const = 0;
		virtual HashValue GetType() const = 0;
	};

	class EmptyResourceFactory : public IResourceFactory
	{
		virtual void* Load(std::string Filename) const override final;
		virtual void Unload(void* Data) const override final;
		virtual HashValue GetType() const override final;
	};
}