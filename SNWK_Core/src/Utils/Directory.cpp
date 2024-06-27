#include "../../include/Utils/Directory.h"

#include <cstdlib>
#include <stdexcept>


// ---- ENVIRONMENT DEPENDENT VARIABLES ----

// Get the environment of the app
static const FilesystemID ENVIRONMENT = getenv("windir") ? WINDOWS : UNIX;

// Check whether windir evironment variable is defined in order to ensure
// that proper commands and path dividers are used.
static const String LIST_COMMAND = (ENVIRONMENT == WINDOWS) ? "dir 1> NUL 2>&1 " : "ls >/dev/null 2>&1 ";
static const String MAKE_COMMAND = (ENVIRONMENT == WINDOWS) ? "mkdir 1> NUL 2>&1 " : "mkdir >/dev/null 2>&1 ";

const String dir::DIVIDER = (ENVIRONMENT == WINDOWS) ? "\\" : "/";

// ---- FUNCTIONS ----

using namespace dir;

bool dir::dir_exists (const String& path) noexcept
{
    String command = LIST_COMMAND + path;
    return std::system(command.c_str()) == 0;
}

int dir::mkdir (const String& path)
{
    if (dir_exists(path))
        throw std::invalid_argument("mkdir: Directory already exists.");

    String command = MAKE_COMMAND + path;
    return std::system(command.c_str());
}