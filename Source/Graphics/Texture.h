#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>

#include "../Core/Resource.h"
#include "../Math/Vector2D.h"

namespace Engine
{
	namespace Graphics
	{
		class TextureFactory : public IResourceFactory
		{
		public:
			virtual void* Load(std::string Filename) const final override;
			virtual void Unload(void* Data) const final override;
			virtual ResourceType GetType() const final override;
		};

		struct Texture
		{
			static IResourceFactory& GetFactory();
			static ResourceType GetResourceType();

			GLuint TextureID;
			Math::Vector2Di Dimensions;
		};
	}
}