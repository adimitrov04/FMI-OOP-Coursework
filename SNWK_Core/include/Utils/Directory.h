#pragma once

#ifndef __DIRECTORY_LIB__
#define __DIRECTORY_LIB__

#include <cstdlib>
#include <stdexcept>

#include "String.h"

namespace dir
{

bool dir_exists (const char* path) noexcept;
int mkdir (const char* path);

}

#endif // __DIRECTORY_LIB__