#include "Texture.h"

#include "../Utility/Logger.h"

using Logger = Engine::Utility::Logger;

namespace Engine
{
	namespace Graphics
	{
		void* TextureFactory::Load(std::string Filename) const
		{
			Texture* NewTexture = new Texture();
			SDL_Surface* ImageSurface = IMG_Load(Filename.c_str());

			if (ImageSurface == nullptr)
			{
				Logger::Warn("Failed to load \"" + Filename + "\" - SDL_image error: " + IMG_GetError());
				delete NewTexture;
				return nullptr;
			}

			SDL_Surface* ConvertedSurface = SDL_ConvertSurfaceFormat(ImageSurface, SDL_PIXELFORMAT_ABGR8888, 0);
			SDL_FreeSurface(ImageSurface);

			if (ConvertedSurface == nullptr)
			{
				Logger::Warn("Unable to convert \"" + Filename + "\" to RGBA4 format - SDL2 error: " + SDL_GetError());
				delete NewTexture;
				return nullptr;
			}

			glGenTextures(1, &(NewTexture->TextureID));

			if (NewTexture->TextureID == GL_INVALID_VALUE)
			{
				Logger::Warn("Failed to load \"" + Filename + "\": unable to generate new OpenGL texture");

				SDL_FreeSurface(ConvertedSurface);
				delete NewTexture;
				return nullptr;
			}

			NewTexture->Dimensions = Math::Vector2Di(ConvertedSurface->w, ConvertedSurface->h);

			glBindTexture(GL_TEXTURE_2D, NewTexture->TextureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, NewTexture->Dimensions.X, NewTexture->Dimensions.Y, 0, GL_RGBA, GL_UNSIGNED_BYTE, ConvertedSurface->pixels);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			SDL_FreeSurface(ConvertedSurface);

			Logger::Verbose("\"" + Filename + "\" was loaded as a texture");
			return NewTexture;
		}

		void TextureFactory::Unload(void* Data) const
		{
			Texture* TextureData = static_cast<Texture*>(Data);
			glDeleteTextures(1, &(TextureData->TextureID));

			Logger::Verbose("Texture data was dumped");
		}

		ResourceType TextureFactory::GetType() const
		{
			return Utility::GetHashedString("Texture");
		}


		IResourceFactory& Texture::GetFactory()
		{
			static TextureFactory Factory;
			return Factory;
		}

		ResourceType Texture::GetResourceType()
		{
			return Utility::GetHashedString("Texture");
		}
	}
}