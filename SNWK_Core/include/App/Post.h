#pragma once

#ifndef __POST_H__
#define __POST_H__

#include <cstdint>

#include "../project_utils.h"
#include "../SNWKFormat/snwk_format.h"

#include "Interfaces/AppElement.h"
#include "Interfaces/ISerializeable.h"

#include "User.h"
#include "Comment.h"

class Post : public AppElement
{

public:
    Post();
    Post(uint32_t setThreadID, uint32_t setPostID, uint32_t setAuthorID,
         const String& setTitle, const String& setContent, Vector<Comment>&& loadComments);

    ~Post() = default;

public:
    

public:
    uint32_t GetParentThreadID () const noexcept;
    uint32_t GetPostID () const noexcept;
    uint32_t GetAuthorID () const noexcept;
    const String& GetTitle () const noexcept;
    const String& GetContent () const noexcept;

    void SetTitle (const String& text);
    void SetContent (const String& text);

    virtual void Serialize (std::fstream& file) const override;
    virtual void Deserialize (std::fstream& file) override;

    using AppElement::DeleteObject;

public:
    static const snwk::FourCC TYPE_FCC;
    static const Post DELETED_POST;

private:
    static Vector<Comment> getEmptyVector();

private:
    uint32_t parent_thread_id;
    uint32_t post_id;
    uint32_t author_id;

    String title;
    String content;
    Vector<Comment> post_comments;

};

#endif // __POST_H__