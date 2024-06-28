#include <stdexcept>

#include "../../include/snwk_lib.h"
#include "../../include/Utils/Directory.h"

#include "../../include/App/Network.h"

// ---- STATIC VARIABLES ----

uint64_t current_user_count = 0;

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



// ---- LOAD & SAVE METHODS ----

void Network::LoadNetwork (const String& networkName)
{
    String networkDirectory = Network::GLOBAL_SAVE_PATH + networkName + dir::DIVIDER;
    if (dir::dir_exists(networkDirectory) == false)
        throw std::invalid_argument("Network.LoadNetwork: Specified network not found");

    snwk::SNWKFile<User> userFile;
    snwk::SNWKFile<Comment> commentFile;
    snwk::SNWKFile<Post> postFile;
    snwk::SNWKFile<Thread> threadFile;
    
    try
    {
        network_directory_path = networkDirectory;
        voting_directory_path = networkDirectory + "vote_tables" + dir::DIVIDER;
    }
    catch (...)
    {
        network_directory_path.clear();
        throw std::runtime_error("FATAL: Loading network failed.");
    }
    
    try
    {

        if (dir::dir_exists(voting_directory_path) == false)
            throw std::invalid_argument("Network.LoadNetwork: Could not find vote_tables folder.");

        userFile.open((networkDirectory + "users.snwk").c_str());
        commentFile.open((networkDirectory + "comments.snwk").c_str());
        postFile.open((networkDirectory + "posts.snwk").c_str());
        threadFile.open((networkDirectory + "threads.snwk").c_str());
    }
    catch (...)
    {
        userFile.close();
        commentFile.close();
        userFile.close();
        userFile.close();

        network_directory_path.clear();
        voting_directory_path.clear();

        throw std::invalid_argument("Network.LoadNetwork: Specified network is corrupted or incomplete.");
    }

    if (load_users(userFile) == false)
    {
        userFile.close();
        commentFile.close();
        userFile.close();
        userFile.close();

        network_directory_path.clear();
        voting_directory_path.clear();
        
        throw std::runtime_error("FATAL: Reading users file failed. Network could not be lodaded.");
    }

    if (load_threads(threadFile, postFile, commentFile) == false)
    {
        users.clear();
        
        userFile.close();
        commentFile.close();
        userFile.close();
        userFile.close();
        
        network_directory_path.clear();
        voting_directory_path.clear();

        throw std::runtime_error("FATAL: Reading threads file failed. Network could not be lodaded.");
    }

    userFile.close();
    commentFile.close();
    postFile.close();
    threadFile.close();
}

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

bool Network::load_users (snwk::SNWKFile<User> &inFile)
{
    try
    {
        for (uint64_t i = 0; i < inFile.get_object_count(); i++)
        {
            User temp;
            inFile.read_object(temp);

            users.push_back(temp);
        }
    }
    catch (...)
    {
        users.clear();
        
        return false;
    }

    return true;
}

bool Network::load_threads (snwk::SNWKFile<Thread> &inThreadFile,
                            snwk::SNWKFile<Post> &inPostFile,
                            snwk::SNWKFile<Comment> &inCommentFile)
{
    Vector<Comment> loadComments;
    Vector<Post> loadPosts;

    // LOAD COMMENTS
    try
    {
        for (uint64_t i = 0; i < inCommentFile.get_object_count(); i++)
        {
            Comment temp;
            inCommentFile.read_object(temp);

            loadComments.push_back(temp);
        }
    }
    catch (...)
    {
        loadComments.clear();
        
        return false;
    }

    // LOAD POSTS
    try
    {
        for (uint64_t i = 0; i < inPostFile.get_object_count(); i++)
        {
            Post temp;
            inPostFile.read_object(temp);

            loadPosts.push_back(temp);
        }
    }
    catch (...)
    {
        loadComments.clear();
        loadPosts.clear();

        return false;
    }

    // LOAD THREADS
    try
    {
        for (uint64_t i = 0; i < inThreadFile.get_object_count(); i++)
        {
            Thread temp;
            inThreadFile.read_object(temp);

            threads.push_back(temp);
        }
    }
    catch (...)
    {
        loadComments.clear();
        loadPosts.clear();
        threads.clear();

        return false;
    }

    // LINK ELEMENTS
    try
    {
        for (uint64_t i = 0; i < loadPosts.size(); i++)
        {
            Thread* parentThread = GetThreadByID(loadPosts[i].GetParentThreadID());
            if (parentThread == nullptr)
                throw std::runtime_error("Network.load_threads: Error linking posts with parent threads");

            parentThread->AddPostNoWrite(loadPosts[i]);
        }
    }
    catch (...)
    {
        loadComments.clear();
        loadPosts.clear();
        threads.clear();

        return false;
    }

    try
    {
        for (uint64_t i = 0; i < loadComments.size(); i++)
        {
            Thread* parentThread = GetThreadByID(loadComments[i].GetParentThreadID());
            if (parentThread == nullptr)
                throw std::runtime_error("Network.load_threads: Error linking comments with parent threads");

            Post* parentPost = parentThread->GetPostByID(loadComments[i].GetParentPostID());
            if (parentPost == nullptr)
                throw std::runtime_error("Network.load_threads: Error linking comments with parent posts");

            parentPost->AddCommentNoWrite(loadComments[i]);
        }
    }
    catch (...)
    {
        loadComments.clear();
        loadPosts.clear();
        threads.clear();

        return false;
    }

    loadComments.clear();
    loadPosts.clear();

    return true;
}
