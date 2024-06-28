#pragma once

#ifndef __NETWORK_H__
#define __NETWORK_H__

#include "../project_utils.h"

#include "User.h"
#include "Comment.h"
#include "Post.h"
#include "Thread.h"

class Network
{

public:
    static void SetGlobalSavePath (String path);
    static Network& GetNetwork ()
    {
        if (!GLOBAL_SAVE_PATH)
            throw std::logic_error("Network.GetNetwork: Could not find network save directory.");

        GLOBAL_SAVE_PATH.cat(String("SavedNetworks") + dir::DIVIDER);
        if (dir::dir_exists(GLOBAL_SAVE_PATH) == false)
            dir::mkdir(GLOBAL_SAVE_PATH);

        static Network instance;
        return instance;
    }

    ~Network();

public:
    User* GetCurrentUser () const noexcept;
    User* GetUserByID (const uint32_t id) const noexcept;
    User* GetUserByName (const String& name) const noexcept;
    
    Thread* GetThreadByID (const uint32_t id) const noexcept;
    Thread* GetThreadByTitle (const String& title) const;
    
    const String& GetCurrentNetworkPath () const;
    const String& GetCurrentVoteTablePath () const;

    void OpenThread() const;
    void OpenPost() const;

    void SetCurrentUser (User& setUser) const;
    void RegisterUser (const User& user);
    void AddThread (const Thread& thread);

    void LoadNetwork (const String& networkName);
    void CreateNewNetwork (const String& networkName);
    void SaveInCurrentPath () const;
    void ExportToNewPath (const String& networkName) const;

public:
    // The directory containing all networks. Has to be set externally
    static String GLOBAL_SAVE_PATH;
    

private:
    Network() = default;
    Network(const Network& other) = delete;
    Network& operator=(const Network& other) = delete;

    bool load_users (snwk::SNWKFile<User> &inFile);
    bool load_threads (snwk::SNWKFile<Thread> &inThreadFile, snwk::SNWKFile<Post> &inPostFile, snwk::SNWKFile<Comment> &inCommentFile);

private:
    static uint64_t current_user_count;

    static User* CURRENTLY_LOGGED_IN_USER;
    static Thread* CURRENTLY_OPENED_THREAD;
    static Post* CURRENTLY_OPENED_POST;

private:
    // The path of the current network
    String network_directory_path;
    // The path of the "vote_tables" folder
    String voting_directory_path;

    Vector<User> users;
    Vector<Thread> threads;

};

#endif // __NETWORK_H__