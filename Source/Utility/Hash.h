#pragma once

#include "../../External/cx_fnv1.h"

namespace Engine
{
	using HashValue = uint64_t;

	namespace Utility
	{
		constexpr HashValue GetHashedString(const char* String)
		{
			return cx::fnv1(String);
		}

		struct HashFunction
		{
			HashValue operator()(const char* Key)
			{
				return GetHashedString(Key);
			}
		};
	}
}