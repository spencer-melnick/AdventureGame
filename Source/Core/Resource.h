#pragma once

#include <string>
#include <vector>
#include <map>

#include "../Utility/Hash.h"
#include "ResourceFactory.h"
#include "ResourceDictionary.h"
#include "ResourceManager.h"

// A "Resource" is actually just a ResourceFactory class and any datatype with a
// static GetResourceType() member that returns the appropriate ResourceType
// (ideally a hashed string of its class name using Utility::GetHashedString() )