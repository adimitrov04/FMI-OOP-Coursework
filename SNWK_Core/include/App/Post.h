#pragma once

#ifndef __POST_H__
#define __POST_H__

#include <cstdint>

#include "../Utils/Vector.h"

#include "Interfaces/ISerializeable.h"

#include "User.h"
#include "Comment.h"

class Post : public ISerializeable
{

public:
    Post();
    ~Post();

public:
    virtual void Serialize (std::fstream& file) const override;
    virtual void Deserialize (std::fstream& file) override;

private:
    uint16_t post_id;
    User& author;

    String title;
    String content;
    Vector<Comment> post_comments;

};

#endif // __POST_H__