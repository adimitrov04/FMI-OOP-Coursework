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
    static Network& GetNetwork ()
    {
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
    
    const String& GetHomeDirectoryPath () const;

    void OpenThread() const;
    void OpenPost() const;

    void SetCurrentUser (User& setUser) const;
    void RegisterUser (const User& user);
    void AddThread (const Thread& thread);

    void LoadNetwork (String& dir_path);
    void CreateNewNetwork (String& dir_path);
    void SaveInCurrentPath () const;
    void ExportToNewPath (String& export_path) const;

public:
    static String home_directory_path;

private:
    Network() = default;
    Network(const Network& other) = delete;
    Network& operator=(const Network& other) = delete;

    static bool fix_dir_path (String& path) noexcept;

private:
    static uint64_t currentUserCount;

    static User* CURRENTLY_LOGGED_IN_USER;
    static Thread* CURRENTLY_OPENED_THREAD;
    static Post* CURRENTLY_OPENED_POST;

private:
    Vector<User> users;
    Vector<Thread> threads;

};

#endif // __NETWORK_H__