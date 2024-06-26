#include <stdexcept>

#include "../../include/snwk_lib.h"

#include "../../include/App/Network.h"

// ---- STATIC VARIABLES ----

uint64_t currentUserCount = 0;

User* CURRENTLY_LOGGED_IN_USER = nullptr;

Thread* CURRENTLY_OPENED_THREAD = nullptr;
Post* CURRENTLY_OPENED_POST = nullptr;

// ---- STATIC METHODS ----



// ---- LIFECYCLE ----

Network::~Network()
{
    currentUserCount = 0;

    CURRENTLY_LOGGED_IN_USER = nullptr;

    CURRENTLY_OPENED_THREAD = nullptr;
    CURRENTLY_OPENED_POST = nullptr;
}

// ---- GETTERS ----

User* Network::GetCurrentUser () const noexcept
{
    return Network::CURRENTLY_LOGGED_IN_USER;
}

User* Network::GetUserByID (const uint32_t id) const noexcept
{
    return users.auto_search(User(id));
}

User* Network::GetUserByName (const String& name) const noexcept
{
    return users.basic_search(User(name));
}

Thread* Network::GetThreadByID (const uint32_t id) const noexcept
{
    return threads.auto_search(id);
}

Thread* Network::GetThreadByTitle (const String& title) const
{
    if (threads.count_repetitions(title) > 1)
        throw std::runtime_error("Network.GetThreadByTitle: Multiple threads have given title, use GetThreadByID.");

    return threads.basic_search(title);
}

const String& Network::GetHomeDirectoryPath () const
{
    return home_directory_path;
}

// ---- SETTERS ----



// ---- LOAD & SAVE METHODS ----

