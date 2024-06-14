#include <cstdlib>
#include <stdexcept>

#include "Directory.h"

// Check user environment in order to ensure proper list command is used.
const String LIST_COMM = getenv("windir") ? "dir " : "ls 2>/dev/null ";

const String MAKE_COMM = "mkdir 2>/dev/null ";

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