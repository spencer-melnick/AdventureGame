#pragma once

#include <string>

#include "Video.h"
#include "../Math/Vector2D.h"

namespace Engine
{
	// A small wrapper for the SDL_Window and SDL_Renderer classes
	class Window
	{
	public:
		// IMPORTANT: Does not create actual window on screen or in SDL;
		// only sets parameters - use Initialze() instead
		Window();
		Window(Math::Vector2Di Resolution, std::string Title, bool Fullscreen = false);

		// Actually creates the window and renderer using SDL2 - returns false on failure
		// Cleans itself up if any error occurs
		bool Initialize();

		// Destroys the window and associated SDL2 renderer
		void Destroy();


		void SetFullscreen(bool Value);

		void SetResolution(Math::Vector2Di Value);

		void SetTitle(std::string Value);



		bool IsFullscreen() const;

		Math::Vector2Di GetResolution() const;

		SDL_Window* GetSDLWindow() const;

		SDL_GLContext GetContext() const;

		std::string GetTitle() const;

	private:
		bool Fullscreen_ = false;
		Math::Vector2Di Resolution_;

		SDL_Window* InternalWindow_ = nullptr;
		SDL_GLContext InternalContext_ = nullptr;

		std::string Title_;
	};
}