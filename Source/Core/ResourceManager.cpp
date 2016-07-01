#include "ResourceManager.h"

#include <algorithm>

#include "../Utility/Logger.h"
#include "ResourceDictionary.h"

using Logger = Engine::Utility::Logger;

namespace Engine
{
	ResourceHeader::ResourceHeader(std::string Filename, ResourceType Type, IResourceFactory& Factory) :
		Filename_(Filename), Type_(Type), Factory_(Factory)
	{
		HashedFilename_ = Utility::GetHashedString(Filename_.c_str());
		Data_ = Factory_.Load(Filename_);
	}

	ResourceHeader::~ResourceHeader()
	{
		for (auto i : References_)
		{
			i->Invalidate();
		}

		if (Data_ != nullptr)
			Factory_.Unload(Data_);
	}

	ResourceType ResourceHeader::GetType() const
	{
		return Type_;
	}

	HashValue ResourceHeader::GetHashedName() const
	{
		return HashedFilename_;
	}

	void* ResourceHeader::GetData() const
	{
		return Data_;
	}

	IResourceFactory& ResourceHeader::GetFactory() const
	{
		return Factory_;
	}

	std::string ResourceHeader::GetFilename() const
	{
		return Filename_;
	}

	bool ResourceHeader::IsA(ResourceType Type) const
	{
		return (Type == Type_);
	}

	bool ResourceHeader::HasReferences() const
	{
		return (!References_.empty());
	}


	void ResourceHeader::AddReference(ResourceHandle* Reference)
	{
		References_.push_back(Reference);
	}

	void ResourceHeader::RemoveReference(ResourceHandle* Reference)
	{
		References_.erase(std::remove(References_.begin(), References_.end(), Reference));
	}



	ResourceHandle::ResourceHandle() :
		Valid_(false), Owner_(nullptr)
	{};

	ResourceHandle::ResourceHandle(ResourceHeader* Owner) :
		Valid_(true), Owner_(Owner)
	{
		if (Owner_)
			Owner_->AddReference(this);
	};

	ResourceHandle::ResourceHandle(const ResourceHandle& Source) :
		ResourceHandle(Source.Owner_)
	{};

	ResourceHandle::~ResourceHandle()
	{
		if (Owner_)
			Owner_->RemoveReference(this);
	}

	bool ResourceHandle::IsValid() const
	{
		return Valid_;
	}

	ResourceHeader* ResourceHandle::GetResource() const
	{
		return Owner_;
	}

	void ResourceHandle::Invalidate()
	{
		Valid_ = false;
		Owner_ = nullptr;
	}



	ResourceHandle ResourceManager::GetResource(std::string Filename)
	{
		HashValue HashedFilename = Utility::GetHashedString(Filename.c_str());

		auto Result = Resources_.find(HashedFilename);

		if (Result == Resources_.end())
		{
			Logger::Verbose("\"" + Filename + "\" was not previously loaded, attempting to load the resource from the filesystem");

			return AllocateResource_(Filename);
		}

		return ResourceHandle(&(Result->second));
	}

	ResourceHandle ResourceManager::GetResource(size_t HashedFilename)
	{
		auto Result = Resources_.find(HashedFilename);

		if (Result == Resources_.end())
		{
			Logger::Warn("Resource unable to be found");

			return ResourceHandle();
		}

		return ResourceHandle(&(Result->second));
	}

	void ResourceManager::DeleteResource(std::string Filename)
	{
		DeleteResource(Utility::GetHashedString(Filename.c_str()));
	}

	void ResourceManager::DeleteResource(HashValue HashedFilename)
	{
		auto Result = Resources_.find(HashedFilename);

		if (Result != Resources_.end())
		{
			ResourceHeader& FoundResource = Result->second;
			Logger::Verbose("\"" + FoundResource.GetFilename() + "\" is being deleted; unloading resources");

			const IResourceFactory& Allocator = FoundResource.GetFactory();

			if (Allocator.GetType() == Utility::GetHashedString("NULL"))
			{
				Logger::Warn("No valid factory found for \"" + FoundResource.GetFilename() + "\"; unable to properly free resources");
			}
			else
			{
				Allocator.Unload(FoundResource.GetData());
			}

			Resources_.erase(Result);
		}
		else
		{
			Logger::Warn("Unable to free resource: invalid name");
		}
	}

	void ResourceManager::ClearUnusedResources()
	{
		for (auto i = Resources_.begin(); i != Resources_.end(); )
		{
			if (i->second.HasReferences())
			{
				i++;
			}
			else
			{
				i = Resources_.erase(i);
			}
		}
	}

	void ResourceManager::ClearResources()
	{
		Resources_.clear();
	}

	ResourceHandle ResourceManager::AllocateResource_(std::string Filename)
	{
		std::string Extension = Filename.substr(Filename.rfind('.'));
		if (Extension == Filename)
		{
			Logger::Warn("\"" + Filename + "\" does not have a file extension");
			return ResourceHandle();
		}

		IResourceFactory& Factory = GetResourceDictionary().GetFactory(Extension);
		ResourceType Type = Factory.GetType();

		if (Type == Utility::GetHashedString("NULL"))
		{
			Logger::Warn("Cannot load resource \"" + Filename + "\": \"" + Extension + "\" is not a valid file extension");
			return ResourceHandle();
		}

		auto Result = Resources_.emplace(std::piecewise_construct, std::forward_as_tuple(Utility::GetHashedString(Filename.c_str())), std::forward_as_tuple(Filename, Type, Factory));

		if (Result.second == false || Result.first->second.GetData() == nullptr)
		{
			Logger::Warn("Failed to load resource \"" + Filename + "\"");
			Resources_.erase(Result.first);

			return ResourceHandle();
		}

		return ResourceHandle(&(Result.first->second));
	}
}