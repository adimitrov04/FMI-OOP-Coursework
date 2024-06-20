#include "../../include/Utils/Directory.h"

#include <cstdlib>
#include <stdexcept>

// Check whether windir evironment variable is defined in order to ensure
// that proper commantds are used.
static const String LIST_COMM = getenv("windir") ? "dir 1> NUL 2>&1 " : "ls >/dev/null 2>&1 ";
static const String MAKE_COMM = getenv("windir") ? "mkdir 1> NUL 2>&1 " : "mkdir >/dev/null 2>&1 ";

bool dir::dir_exists (const char* path) noexcept
{
    String command = LIST_COMM + path;
    return std::system(command.c_str()) == 0;
}

int dir::mkdir (const char* path)
{
    if (dir_exists(path))
        throw std::invalid_argument("mkdir: Directory already exists.");

    String command = MAKE_COMM + path;
    return std::system(command.c_str());
}