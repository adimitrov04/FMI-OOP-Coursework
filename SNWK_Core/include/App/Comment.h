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
    Comment(const uint64_t id);
    Comment(uint64_t setThreadID, uint64_t setPostID, uint64_t setID,
            uint64_t setAuthorID, const String& setContent, uint64_t setReplyID);
    
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
    uint64_t GetAuthorID () const noexcept;
    uint64_t GetID () const noexcept;
    uint64_t GetParentThreadID () const noexcept;
    uint64_t GetParentPostID () const noexcept;
    uint64_t GetReplyID () const noexcept;
    Comment GetDeletedVersion () const;
    
    void SetParentThreadID (const uint64_t id);
    void SetParentPostID (const uint64_t id);
    void SetID (const uint64_t id);
    void SetReplyID (const uint64_t id);
    void SetAuthorID (const uint64_t id);
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
    uint64_t parent_thread_id;
    uint64_t parent_post_id;
    uint64_t comment_id;

    uint64_t author_id;
    uint64_t replying_to_id;

    String content;

};

#endif // __COMMENT_H__