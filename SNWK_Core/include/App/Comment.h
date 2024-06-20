#pragma once

#ifndef __COMMENT_H__
#define __COMMENT_H__

#include <cstdint>

#include "../Utils/String.h"

#include "Interfaces/AppElement.h"
#include "Interfaces/ISerializeable.h"
#include "VoteableObject.h"

#include "User.h"
#include "Post.h"

class Comment : public AppElement, public ISerializeable, public virtual VoteableObject
{

public:
    Comment();
    Comment(const uint32_t setID, const User& setAuthor, const String& setContent,
            const Post& setParent, const uint32_t setReply);
    
    ~Comment() = default;

public:
    Comment& operator= (const Comment& other);

public:
    const String& GetContent () const;
    User& GetAuthor () const;
    uint32_t GetID () const;
    uint32_t GetParentPostID () const;
    uint32_t GetReplyID () const;

    virtual void Serialize (std::fstream& file) const override;
    virtual void Deserialize (std::fstream& file) override;

    virtual void DeleteObject () override;

private:
    uint32_t comment_id;
    uint32_t parent_post_id;

    User& author;
    String content;
    uint32_t replying_to_id;

};

#endif // __COMMENT_H__