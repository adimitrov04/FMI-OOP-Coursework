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
    Thread(const uint32_t id);
    Thread(const String& title);
    Thread(uint32_t setID, uint32_t setAuthorID, const String& setTitle, Vector<Post>&& loadPosts);

    ~Thread() = default;

public:
    Thread& operator= (const Thread& other);
    bool operator== (const Thread& other) const noexcept;

    bool operator< (const Thread& other) const noexcept;
    bool operator<= (const Thread& other) const noexcept;
    bool operator> (const Thread& other) const noexcept;
    bool operator>= (const Thread& other) const noexcept;

public:
    uint32_t GetThreadID () const noexcept;
    uint32_t GetAuthorID () const noexcept;
    const String& GetTitle () const noexcept;
    
    Post* GetPostByID (const uint32_t id) const;
    Thread GetDeletedVersion () const;

    void SetAuthorID (uint32_t setAuthorID);
    void SetTitle (const String& setTitle);

    void AddPost (const Post& post, snwk::SNWKFile<Post> &postDataFile);

    virtual void Serialize (std::fstream& file) const override;
    virtual void Deserialize (std::fstream& file) override;

    using AppElement::DeleteObject;
    static Vector<Thread> getEmptyVector();

private:
    Thread (const Thread& other);
    void SetID (uint32_t setID);

public:
    static const snwk::FourCC TYPE_FCC;
    static const Thread DELETED_THREAD;

private:
    uint32_t thread_id;
    uint32_t author_id;
    
    String title;
    Vector<Post> post_list;

};

#endif // __THREAD_H__