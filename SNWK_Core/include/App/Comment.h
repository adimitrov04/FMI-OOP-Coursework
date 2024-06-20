#pragma once

#ifndef __COMMENT_H__
#define __COMMENT_H__

#include <cstdint>

#include "Interfaces/ISerializeable.h"
#include "VoteableObject.h"

#include "User.h"
#include "Post.h"

class Comment : public ISerializeable, public virtual VoteableObject
{

public:
    Comment();
    Comment(const User& setAuthor, const String& setContent, const Post& setParent, const uint32_t setReply);
    
    ~Comment() = default;

public:
    Comment& operator= (const Comment& other);

public:
    virtual void Serialize (std::fstream& file) const override;
    virtual void Deserialize (std::fstream& file) override;

private:
    uint32_t comment_id;
    uint32_t parent_post_id;

    User& author;
    String content;
    uint32_t replying_to_id;

};

#endif // __COMMENT_H__