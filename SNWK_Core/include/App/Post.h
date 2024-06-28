#pragma once

#ifndef __POST_H__
#define __POST_H__

#include <cstdint>

#include "../project_utils.h"
#include "../snwk_lib.h"

#include "Interfaces/AppElement.h"
#include "Interfaces/ISerializeable.h"

#include "User.h"
#include "Comment.h"

class Post : public AppElement
{

public:
    Post();
    Post(const uint64_t id);
    Post(uint64_t setThreadID, uint64_t setPostID, uint64_t setAuthorID,
         const String& setTitle, const String& setContent, Vector<Comment>&& loadComments);

    ~Post() = default;

public:
    Post& operator= (const Post& other);
    bool operator== (const Post& other) const;
    
    bool operator> (const Post& other) const;
    bool operator<= (const Post& other) const;
    bool operator< (const Post& other) const;
    bool operator>= (const Post& other) const;

public:
    uint64_t GetParentThreadID () const noexcept;
    uint64_t GetPostID () const noexcept;
    uint64_t GetAuthorID () const noexcept;
    const String& GetTitle () const noexcept;
    const String& GetContent () const noexcept;

    uint64_t GetCommentCount () const noexcept;
    uint64_t GetTrueCommentCount () const noexcept;
    Comment* GetCommentByID (uint64_t id) const;

    Post GetDeletedVersion () const;

    void SetTitle (const String& text);
    void SetContent (const String& text);

    void AddComment (const Comment& comment, snwk::SNWKFile<Comment> &commentDataFile);
    void AddCommentNoWrite (const Comment& comment);

    virtual void Serialize (std::fstream& file) const override;
    virtual void Deserialize (std::fstream& file) override;

    using AppElement::DeleteObject;
    virtual AppElement* clone () const override;
    static Vector<Post> getEmptyVector();

public:
    static const snwk::FourCC TYPE_FCC;

private:
    Post(const Post& other);
    void SetID (const uint64_t id);

private:
    uint64_t parent_thread_id;
    uint64_t post_id;
    uint64_t author_id;

    String title;
    String content;
    Vector<Comment> post_comments;

};

#endif // __POST_H__