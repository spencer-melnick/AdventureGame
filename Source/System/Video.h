#pragma once

#include <vector>

#include <SDL.h>
#include <SDL_image.h>

#include <SDL_opengl.h>

#include "../Math/Vector2D.h"

namespace Engine
{
	namespace Video
	{
		// Initializes SDL2 and SDL2 Image - returns false on failure
		bool Initialize();

		void SetGLAttributes();
		// Sets the proper attributes for the creation of a OpenGL context

		// Enables the proper OpenGL states for use after the context is created
		void SetupGL();

		// Quits both SDL2 and SDL2 Image
		void Quit();

		std::vector<Math::Vector2Di> GetSupportedResolutions(const int DisplayIndex = 0);
	}
}