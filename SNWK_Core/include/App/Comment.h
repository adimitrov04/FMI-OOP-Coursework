#pragma once

#ifndef __COMMENT_H__
#define __COMMENT_H__

#include <cstdint>

#include "../Utils/String.h"
#include "../SNWKFormat/snwk_format.h"

#include "Interfaces/AppElement.h"
#include "Interfaces/ISerializeable.h"
#include "VoteableObject.h"

#include "User.h"
//#include "Post.h"

class Comment : public AppElement, public virtual VoteableObject
{

public:
    Comment();
    Comment(uint32_t setThreadID, uint32_t setPostID, uint32_t setID,
            uint32_t setAuthorID, const String& setContent, uint32_t setReplyID);
    
    ~Comment() = default;

public:
    Comment& operator= (const Comment& other);
    bool operator== (const Comment& other) const noexcept;
    
    bool operator< (const Comment& other) const noexcept;
    bool operator<= (const Comment& other) const noexcept;
    bool operator> (const Comment& other) const noexcept;
    bool operator>= (const Comment& other) const noexcept;

public:
    const String& GetContent () const noexcept;
    uint32_t GetAuthorID () const noexcept;
    uint32_t GetID () const noexcept;
    uint32_t GetParentThreadID () const noexcept;
    uint32_t GetParentPostID () const noexcept;
    uint32_t GetReplyID () const noexcept;
    Comment GetDeletedVersion () const;
    
    void SetParentThreadID (const uint32_t id);
    void SetParentPostID (const uint32_t id);
    void SetID (const uint32_t id);
    void SetReplyID (const uint32_t id);
    void SetAuthorID (const uint32_t id);
    void SetContent (const String& text);

    virtual void Upvote (const User& voter, User& author) override;
    virtual void Downvote (const User& voter, User& author) override;
    virtual void Unvote (const User& voter, User& author) override;

    virtual void Serialize (std::fstream& file) const override;
    virtual void Deserialize (std::fstream& file) override;

    using AppElement::DeleteObject;
    static Vector<Comment> getEmptyVector();

public:
    static const snwk::FourCC TYPE_FCC;
    static const String VOTE_TABLE_DIR;

private:
    Comment(const Comment& other);
    String generate_vote_table_filename() const;

private:
    uint32_t parent_thread_id;
    uint32_t parent_post_id;
    uint32_t comment_id;

    uint32_t author_id;
    uint32_t replying_to_id;

    String content;

};

#endif // __COMMENT_H__