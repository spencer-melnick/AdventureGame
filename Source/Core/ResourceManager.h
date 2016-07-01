#pragma once

#include <string>
#include <vector>
#include <map>

#include "../Utility/Hash.h"
#include "ResourceFactory.h"

namespace Engine
{
	using ResourceType = HashValue;

	class ResourceHandle;

	// ResourceHeaders keep track of all ResourceReferences and notify
	// the references when the original is destroyed
	class ResourceHeader
	{
		friend class ResourceHandle;

	public:
		ResourceHeader(std::string Filename, ResourceType Type, IResourceFactory& Factory);
		~ResourceHeader();

		ResourceType GetType() const;
		HashValue GetHashedName() const;
		void* GetData() const;
		IResourceFactory& GetFactory() const;
		std::string GetFilename() const;

		bool IsA(ResourceType Type) const;
		bool HasReferences() const;

	protected:
		void AddReference(ResourceHandle* Reference);

		void RemoveReference(ResourceHandle* Reference);

	private:
		ResourceType Type_;
		HashValue HashedFilename_;
		void* Data_;
		IResourceFactory& Factory_;
		std::string Filename_;
		std::vector<ResourceHandle*> References_;
	};


	// ResourceHandles are safe reference accessors of IResource
	// derived classes - they provide a validity check before accessing
	// the relevant resource
	class ResourceHandle
	{
		friend class ResourceHeader;

	public:
		ResourceHandle();
		ResourceHandle(ResourceHeader* Owner);
		ResourceHandle(const ResourceHandle& Source);
		~ResourceHandle();

		bool IsValid() const;

		ResourceHeader* GetResource() const;

		template <typename T> T* GetData() const
		{
			if (Valid_)
			{
				if (Owner_->IsA(T::GetResourceType()))
				{
					T* Data = static_cast<T*>(Owner_->GetData());
					return Data;
				}
			}

			return nullptr;
		}

	protected:
		void Invalidate();

	private:
		bool Valid_;
		ResourceHeader* Owner_;

	};

	class ResourceManager
	{
	public:
		// The main resource allocation function: calls the proper OnAllocate function if the
		// resource has not been loaded yet, otherwise returns a ResourceReference referencing
		// the appropriate resource. If an error occurs, and empty ResourceReference is returned
		ResourceHandle GetResource(std::string Filename);

		// Using only the hashed name, it is impossible to allocate a new resource from the
		// expected filename - this will only return an empty ResourceReference if the resource
		// is not found
		ResourceHandle GetResource(size_t HashedFilename);

		void DeleteResource(std::string Filename);
		void DeleteResource(HashValue HashedFilename);

		// Destroys all resources without any references
		void ClearUnusedResources();

		// Destroys all the resources with their proper deallocators and clears the resource dictionary
		void ClearResources();

	private:
		ResourceHandle AllocateResource_(std::string Filename);

		std::map<HashValue, ResourceHeader> Resources_;
	};
}