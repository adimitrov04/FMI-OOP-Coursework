#pragma once

#ifndef __NETWORK_H__
#define __NETWORK_H__

#include "../Utils/Vector.h"
#include "../Utils/String.h"

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

public:
    void Load (const String& dir_path);
    void SaveInCurrentPath () const;
    void ExportToNewPath (const String& export_path) const;

private:


private:
    Network() = default;
    Network(const Network& other) = delete;
    Network& operator=(const Network& other) = delete;
    ~Network() = default;

private:
    String directory_path;
    
    Vector<User> users;
    Vector<Thread> threads;

};

#endif // __NETWORK_H__