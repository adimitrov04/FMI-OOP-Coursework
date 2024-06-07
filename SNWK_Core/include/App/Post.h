#pragma once

#ifndef __POST_H__
#define __POST_H__

#include "../Utils/Vector.h"

#include "../SNWKFormat/ISerializeable.h"

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
    const User& author;
    String title;
    String content;
    Vector<Comment> comments;

};

#endif // __POST_H__