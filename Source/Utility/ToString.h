#pragma once

#include <string>
#include <type_traits>
#include <sstream>

namespace Engine
{
	namespace Utility
	{
		template <typename T, typename = std::enable_if<std::is_arithmetic<T>::value>>
		std::string ToString(T Value)
		{
			std::string String;
			std::stringstream Stream;

			Stream << Value;
			Stream >> String;

			return String;
		}
	}
}