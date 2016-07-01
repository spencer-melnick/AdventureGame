#pragma once

#include <vector>
#include <string>

#include <imgui.h>

namespace Engine
{
	namespace Utility
	{
		class Logger
		{
		public:
			static void Warn(std::string Message);
			static void Error(std::string Message);
			static void Debug(std::string Message);
			static void Verbose(std::string Message); // Identical to Debug() but only runs if _DEBUG is defined

			static void Render(bool* Open);

			static void Dump(std::string Filename);

		protected:
			struct MessageInfo_
			{
				enum class MessageLevel
				{
					MESSAGE_WARNING,
					MESSAGE_ERROR,
					MESSAGE_DEBUG,
					MESSAGE_VERBOSE,
					NUM_LEVELS
				};

				std::string String;
				MessageLevel Level;

				MessageInfo_(std::string Message, MessageLevel Type) :
					String(Message), Level(Type)
				{};
			};

		private:
			static std::vector<MessageInfo_>& GetLog();
		};
	}
}