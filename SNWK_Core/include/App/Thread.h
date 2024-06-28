#pragma once

#ifndef __THREAD_H__
#define __THREAD_H__

#include <cstdint>

#include "../snwk_lib.h"

#include "../Utils/Vector.h"

#include "Interfaces/AppElement.h"
#include "Interfaces/ISerializeable.h"

#include "User.h"
#include "Comment.h"
#include "Post.h"

class Thread : public AppElement
{

public:
    Thread();
    Thread(const uint64_t id);
    Thread(const String& title);
    Thread(uint64_t setID, uint64_t setAuthorID, const String& setTitle, Vector<Post>&& loadPosts);

    ~Thread() = default;

public:
    Thread& operator= (const Thread& other);
    bool operator== (const Thread& other) const noexcept;

    bool operator< (const Thread& other) const noexcept;
    bool operator<= (const Thread& other) const noexcept;
    bool operator> (const Thread& other) const noexcept;
    bool operator>= (const Thread& other) const noexcept;

public:
    uint64_t GetThreadID () const noexcept;
    uint64_t GetAuthorID () const noexcept;
    const String& GetTitle () const noexcept;
    uint64_t GetPostCount () const noexcept;
    uint64_t GetTruePostCount () const noexcept;
    
    Post* GetPostByID (const uint64_t id) const;
    Thread GetDeletedVersion () const;

    void SetAuthorID (uint64_t setAuthorID);
    void SetTitle (const String& setTitle);

    void AddPost (const Post& post, snwk::SNWKFile<Post> &postDataFile);
    void AddPostNoWrite (const Post& post);

    virtual void Serialize (std::fstream& file) const override;
    virtual void Deserialize (std::fstream& file) override;

    using AppElement::DeleteObject;
    static Vector<Thread> getEmptyVector();

private:
    Thread (const Thread& other);
    void SetID (uint64_t setID);

public:
    static const snwk::FourCC TYPE_FCC;

private:
    uint64_t thread_id;
    uint64_t author_id;
    
    String title;
    Vector<Post> post_list;

};

#endif // __THREAD_H__