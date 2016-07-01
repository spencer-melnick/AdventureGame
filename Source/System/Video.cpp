#include "Video.h"

#include "../Utility/Logger.h"

namespace Engine
{
	namespace Video
	{
		bool Initialize()
		{
			Utility::Logger::Verbose("Initializing video systems");
			const int SdlImageFlags = IMG_INIT_JPG | IMG_INIT_PNG;

			if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
			{
				Utility::Logger::Error("SDL2 initialization failed");
				Utility::Logger::Error("SDL Error: " + std::string(SDL_GetError()));
				return false;
			}
			Utility::Logger::Verbose("SDL2 initialization suceeded");

			if ((IMG_Init(SdlImageFlags) & SdlImageFlags) != SdlImageFlags)
			{
				Utility::Logger::Error("SDL_image initialization failed");
				Utility::Logger::Error("SDL Error: " + std::string(IMG_GetError()));
				return false;
			}
			Utility::Logger::Verbose("SDL_image initialization suceeded");

			SetGLAttributes();

			return true;
		}

		void SetGLAttributes()
		{
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
			SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		}

		void SetupGL()
		{
			glEnable(GL_TEXTURE_2D);
			glDisable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		void Quit()
		{
			Utility::Logger::Verbose("Quitting video subsystems");

			IMG_Quit();
			SDL_Quit();
		}

		
		std::vector<Math::Vector2Di> GetSupportedResolutions(const int DisplayIndex)
		{
			std::vector<Math::Vector2Di> Resolutions;
			SDL_DisplayMode TempMode;

			for (int i = 0; i < SDL_GetNumDisplayModes(DisplayIndex); i++)
			{
				SDL_GetDisplayMode(DisplayIndex, i, &TempMode);

				if (SDL_BITSPERPIXEL(TempMode.format) == 24 || SDL_BITSPERPIXEL(TempMode.format) == 32)
				{
					bool Duplicate = false;

					for (auto x : Resolutions)
					{
						if (x.X == TempMode.w && x.Y == TempMode.h)
						{
							Duplicate = true;
							break;
						}
					}

					if (!Duplicate)
					{
						Resolutions.emplace_back(TempMode.w, TempMode.h);
					}
				}
			}

			return Resolutions;
		}
	}
}

