#include "Logger.h"

#include <fstream>

namespace Engine
{
	namespace Utility
	{
		void Logger::Warn(std::string Message)
		{
			GetLog().emplace_back("Warning: " + Message, MessageInfo_::MessageLevel::MESSAGE_WARNING);
		}

		void Logger::Error(std::string Message)
		{
			GetLog().emplace_back("Error: " + Message, MessageInfo_::MessageLevel::MESSAGE_ERROR);
		}

		void Logger::Debug(std::string Message)
		{
			GetLog().emplace_back(Message, MessageInfo_::MessageLevel::MESSAGE_DEBUG);
		}

		void Logger::Verbose(std::string Message)
		{
			#ifdef _DEBUG
			GetLog().emplace_back(Message, MessageInfo_::MessageLevel::MESSAGE_VERBOSE);
			#endif
		}

		void Logger::Render(bool* Open)
		{
			if (*Open)
			{
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImColor(15, 15, 15));
				ImGui::SetNextWindowSize(ImVec2(400.0f, 300.0f), ImGuiSetCond_Once);

				ImGui::Begin("Debug Console", Open, ImGuiWindowFlags_NoSavedSettings);

				for (auto& i : GetLog())
				{
					switch (i.Level)
					{
					case MessageInfo_::MessageLevel::MESSAGE_WARNING:
						ImGui::PushStyleColor(ImGuiCol_Text, ImColor(255, 195, 0));
						break;

					case MessageInfo_::MessageLevel::MESSAGE_ERROR:
						ImGui::PushStyleColor(ImGuiCol_Text, ImColor(148, 49, 38));
						break;

					case MessageInfo_::MessageLevel::MESSAGE_DEBUG:
						ImGui::PushStyleColor(ImGuiCol_Text, ImColor(229, 232, 232));
						break;

					default:
						ImGui::PushStyleColor(ImGuiCol_Text, ImColor(100, 100, 100));
						break;
					}

					ImGui::Text(i.String.c_str());
					ImGui::PopStyleColor();
				}

				ImGui::End();
				ImGui::PopStyleColor();
			}
		}

		void Logger::Dump(std::string Filename)
		{
			std::ofstream File(Filename);

			if (File.is_open())
			{
				for (auto& i : GetLog())
				{
					#ifndef _DEBUG
					if (i.Level != MessageInfo_::MessageLevel::MESSAGE_DEBUG)
					#endif
						File << i.String << "\n";
				}
			}

			File.close();
		}

		std::vector<Logger::MessageInfo_>& Logger::GetLog()
		{
			static std::vector<Logger::MessageInfo_> Log;
			return Log;
		}
	}
}