#pragma once

#ifndef __DIRECTORY_LIB__
#define __DIRECTORY_LIB__

#include "String.h"

enum FilesystemID { UNIX, WINDOWS };

namespace dir
{

extern const String DIVIDER;

bool dir_exists (const String& path) noexcept;
int mkdir (const String& path);

}

#endif // __DIRECTORY_LIB__