#include <iostream>

#include <SDL_opengl.h>
#include <gl\GLU.h>

#include "Engine.h"

using namespace Engine;
using Logger = Utility::Logger;

int main(int argc, char* argv[])
{
	if (!Video::Initialize())
	{
		Logger::Dump("log.txt");
		return EXIT_FAILURE;
	}

	Window MainWindow(Math::Vector2Di(1024, 768), "AdventureGame", false);

	if (!MainWindow.Initialize())
	{
		Video::Quit();
		Logger::Dump("log.txt");
		return EXIT_FAILURE;
	}
	
	SDL_Window* InternalWindow = MainWindow.GetSDLWindow();

	ResourceManager Resources;
	GetResourceDictionary().RegisterExtension(".png", Graphics::Texture::GetFactory());

	ResourceHandle TestGuyHandle = Resources.GetResource("Resource/testguy.png");

	Math::Vector2D Vertices[] = { {-0.5f, -0.5f}, {0.5f, -0.5f}, {0.5f, 0.5f}, {-0.5f, 0.5f} };
	Math::Vector2D TexCoords[] = { {1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 1.0f} };

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(2, GL_FLOAT, 0, Vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, TexCoords);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0f, MainWindow.GetResolution().X, 0.0, MainWindow.GetResolution().Y, -1.0, 1.0);
	glViewport(0, 0, MainWindow.GetResolution().X, MainWindow.GetResolution().Y);

	glMatrixMode(GL_MODELVIEW);

	bool Done = false;
	bool ConsoleOpen = false;

	ImGui_ImplSdl_Init(InternalWindow);

	GLenum GLError = GL_NO_ERROR;

	while (!Done)
	{
		SDL_Event Event;

		while (SDL_PollEvent(&Event))
		{
			ImGui_ImplSdl_ProcessEvent(&Event);
			if (Event.type == SDL_QUIT)
				Done = true;
			if (Event.type == SDL_KEYDOWN)
			{
				if (Event.key.keysym.scancode == SDL_SCANCODE_GRAVE)
				{
					ConsoleOpen = !ConsoleOpen;
				}
			}
		}

		ImGui_ImplSdl_NewFrame(InternalWindow);

		if (ConsoleOpen)
		{
			Logger::Render(&ConsoleOpen);
		}

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Graphics::Texture* TestGuyTexture = TestGuyHandle.GetData<Graphics::Texture>();
		if (TestGuyTexture != nullptr)
		{
			glBindTexture(GL_TEXTURE_2D, TestGuyTexture->TextureID);

			glPushMatrix();
			glTranslatef(50.0f, 50.0f, 0.0f);
			glScalef(128.0f, 128.0f, 128.0f);

			glDrawArrays(GL_QUADS, 0, 4);

			glPopMatrix();
		}

		ImGui::Render();

		while ((GLError = glGetError()) != GL_NO_ERROR)
		{
			Logger::Warn("OpenGL error: " + std::string((char*)(gluGetString(GLError))));
		}

		SDL_GL_SwapWindow(InternalWindow);
	}

	ImGui_ImplSdl_Shutdown();

	Resources.ClearResources();

	MainWindow.Destroy();
	Video::Quit();
	Logger::Dump("log.txt");

	return 0;
}