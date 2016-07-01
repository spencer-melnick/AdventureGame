#include "Window.h"

#include "../Utility/Logger.h"

namespace Engine
{
	Window::Window()
		{};

	Window::Window(Math::Vector2Di Resolution, std::string Title, bool Fullscreen) :
		Resolution_(Resolution), Title_(Title), Fullscreen_(Fullscreen)
	{};


	bool Window::Initialize()
	{
		if (InternalWindow_ != nullptr)
		{
			Utility::Logger::Warn("Cannot recreate window \"" + Title_ + "\" as it already exists");
			return false;
		}


		InternalWindow_ = SDL_CreateWindow(Title_.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
										   Resolution_.X, Resolution_.Y, SDL_WINDOW_OPENGL | ((Fullscreen_) ? SDL_WINDOW_FULLSCREEN : 0));

		if (InternalWindow_ == nullptr)
		{
			Utility::Logger::Error("Failed to create window \"" + Title_ + "\"");
			Utility::Logger::Error("SDL Error: " + std::string(SDL_GetError()));
			return false;
		}
		Utility::Logger::Verbose("Window \"" + Title_ + "\" created");

		InternalContext_ = SDL_GL_CreateContext(InternalWindow_);

		if (InternalContext_ == nullptr)
		{
			Utility::Logger::Error("Failed to create OpenGL context for window \"" + Title_ + "\"");
			Utility::Logger::Error("SDL Error: " + std::string(SDL_GetError()));

			SDL_DestroyWindow(InternalWindow_);
			InternalWindow_ = nullptr;

			return false;
		}
		Video::SetupGL();
		Utility::Logger::Verbose("OpenGL context created");
		Utility::Logger::Debug("Running OpenGL version " + std::string((char*)(glGetString(GL_VERSION))));

		return true;
	}


	void Window::Destroy()
	{
		Utility::Logger::Verbose("Destroying window  \"" + Title_ + "\"");

		if (InternalContext_ != nullptr)
		{
			SDL_GL_DeleteContext(InternalContext_);
			InternalContext_ = nullptr;
		}

		if (InternalWindow_ != nullptr)
		{
			SDL_DestroyWindow(InternalWindow_);
			InternalWindow_ = nullptr;
		}
	}


	void Window::SetFullscreen(bool Value)
	{
		Fullscreen_ = Value;

		if (InternalWindow_ != nullptr)
		{
			SDL_SetWindowFullscreen(InternalWindow_, (Fullscreen_) ? SDL_WINDOW_FULLSCREEN : 0);
		}
	}

	void Window::SetResolution(Math::Vector2Di Value)
	{
		Resolution_ = Value;

		if (InternalWindow_ != nullptr)
		{
			SDL_SetWindowSize(InternalWindow_, Resolution_.X, Resolution_.Y);
		}
	}

	void Window::SetTitle(std::string Value)
	{
		Title_ = Value;

		if (InternalWindow_ != nullptr)
		{
			SDL_SetWindowTitle(InternalWindow_, Title_.c_str());
		}
	}


	bool Window::IsFullscreen() const
	{
		return Fullscreen_;
	}

	Math::Vector2Di Window::GetResolution() const
	{
		return Resolution_;
	}

	SDL_Window* Window::GetSDLWindow() const
	{
		return InternalWindow_;
	}

	SDL_GLContext Window::GetContext() const
	{
		return InternalContext_;
	}

	std::string Window::GetTitle() const
	{
		return Title_;
	}
}
