#include <stdexcept>

#include "../../include/snwk_lib.h"
#include "../../include/Utils/Directory.h"

#include "../../include/App/Network.h"

// ---- STATIC VARIABLES ----

User* Network::CURRENTLY_LOGGED_IN_USER = nullptr;

Thread* Network::CURRENTLY_OPENED_THREAD = nullptr;
Post* Network::CURRENTLY_OPENED_POST = nullptr;

String Network::GLOBAL_SAVE_PATH;

// ---- LIFECYCLE ----

void Network::SetGlobalSavePath (String path)
{
    if (!path)
        throw std::invalid_argument("Network.SetGlobalSavePath: Invalid GLOBAL_SAVE_PATH.");

    if (path.last() != dir::DIVIDER.first())
        path.cat(dir::DIVIDER);

    if (dir::dir_exists(path) == false)
        throw std::invalid_argument("Network.SetGlobalSavePath: Invalid GLOBAL_SAVE_PATH.");

    GLOBAL_SAVE_PATH = path;
}

Network::~Network()
{
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
    return users.binary_search(User(id));
}

User* Network::GetUserByName (const String& name) const noexcept
{
    return users.basic_search(User(name));
}

Thread* Network::GetThreadByID (const uint32_t id) const noexcept
{
    return threads.binary_search(id);
}

Thread* Network::GetThreadByTitle (const String& title) const
{
    if (threads.count_repetitions(title) > 1)
        throw std::runtime_error("Network.GetThreadByTitle: Multiple threads have given title, use GetThreadByID.");

    return threads.basic_search(title);
}

const String& Network::GetCurrentNetworkPath () const
{
    return network_directory_path;
}

const String& Network::GetCurrentVoteTablePath () const
{
    return voting_directory_path;
}

// ---- SETTERS ----

void Network::CreateNewNetwork (const String& networkName)
{
    String networkDirectory = GLOBAL_SAVE_PATH + networkName + dir::DIVIDER;
    //std::clog << networkDirectory << '\n';

    try
    {
        if (dir::mkdir(networkDirectory) != 0)
            throw std::runtime_error("Could not create network directory.");
        
        network_directory_path = networkDirectory;
        voting_directory_path = networkDirectory + "vote_tables" + dir::DIVIDER;
        
        dir::mkdir(voting_directory_path);

        snwk::SNWKFile<User> userFile;
        userFile.create((networkDirectory + "users.snwk").c_str());
        userFile.close();

        snwk::SNWKFile<Comment> commentFile;
        commentFile.create((networkDirectory + "comments.snwk").c_str());
        commentFile.close();
        
        snwk::SNWKFile<Post> postFile;
        postFile.create((networkDirectory + "posts.snwk").c_str());
        postFile.close();

        snwk::SNWKFile<Thread> threadFile;
        threadFile.create((networkDirectory + "threads.snwk").c_str());
        threadFile.close();
    }
    catch (...)
    {
        network_directory_path.clear();
        voting_directory_path.clear();
        throw;
    }
}

// ---- LOAD & SAVE METHODS ----

