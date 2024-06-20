#pragma once

#ifndef __THREAD_H__
#define __THREAD_H__

#include <cstdint>

#include "../Utils/Vector.h"

#include "Interfaces/AppElement.h"
#include "Interfaces/ISerializeable.h"

#include "User.h"
#include "Post.h"

class Thread : public AppElement, public ISerializeable
{

public:
    Thread();
    ~Thread();

public:
    uint32_t GetThreadID () const noexcept;
    String& GetTitle () const noexcept;
    User& GetAuthor () const noexcept;
    Post& GetPostByID (const uint16_t id) const;

    virtual void Serialize (std::fstream& file) const override;
    virtual void Deserialize (std::fstream& file) override;

    virtual void DeleteObject () override;

private:
    uint32_t thread_id;
    User& opened_by;
    String title;

    Vector<Post> post_list;

};

#endif // __THREAD_H__