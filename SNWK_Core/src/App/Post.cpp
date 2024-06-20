#include "../../include/App/Post.h"

// ---- CONSTANTS ----

const snwk::FourCC Post::TYPE_FCC = {'p', 'o', 's', 't'};

const Post Post::DELETED_POST(0, 0, 0, "[deleted]", "/null", Comment::getEmptyVector());

Vector<Post> Post::getEmptyVector()
{
    Vector<Post> empty;
    return empty;
}

// ---- LIFECYCLE ----

Post::Post()
: parent_thread_id(0)
, post_id(0)
, author_id(0)
{}

Post::Post(uint32_t setThreadID, uint32_t setPostID, uint32_t setAuthorID, const String& setTitle, const String& setContent,
Vector<Comment>&& loadComments)
: Post()
{
    parent_thread_id = setThreadID;
    post_id = setPostID;
    author_id = setAuthorID;

    SetTitle(setTitle);
    SetContent(setContent);

    post_comments = loadComments;
}

// ---- OPERATORS ----

Post& Post::operator= (const Post& other)
{
    if (this != &other)
    {
        parent_thread_id = other.parent_thread_id;
        post_id = other.post_id;
        author_id = other.author_id;

        title = other.title;
        content = other.content;
        post_comments = other.post_comments;

        if (other.IsDeleted())
            DeleteObject();
    }

    return *this;
}

bool Post::operator== (const Post& other)
{
    return post_id == other.post_id;
}

bool Post::operator< (const Post& other)
{
    return post_id < other.post_id;
}

bool Post::operator<= (const Post& other)
{
    return post_id <= other.post_id;
}

bool Post::operator> (const Post& other)
{
    return post_id > other.post_id;
}

bool Post::operator>= (const Post& other)
{
    return post_id >= other.post_id;
}

// ---- GETTERS ----

uint32_t Post::GetParentThreadID () const noexcept
{
    return parent_thread_id;
}

uint32_t Post::GetPostID () const noexcept
{
    return post_id;
}

uint32_t Post::GetAuthorID () const noexcept
{
    return author_id;
}

const String& Post::GetTitle () const noexcept
{
    return title;
}

const String& Post::GetContent () const noexcept
{
    return content;
}

// ---- SETTERS ----

void Post::SetTitle (const String& text)
{
    title = text;
}

void Post::SetContent (const String& text)
{
    content = text;
}

// ---- SERIALIZATION ----

void Post::Serialize (std::fstream& file) const
{
    if (!file)
        throw std::invalid_argument("Comment.Serialize: File is not in good state.");

    AppElement::Serialize(file);

    file.write(reinterpret_cast<const char*>(&parent_thread_id), sizeof(parent_thread_id));
    check_file_state(file);

    file.write(reinterpret_cast<const char*>(&post_id), sizeof(post_id));
    check_file_state(file);

    file.write(reinterpret_cast<const char*>(&author_id), sizeof(author_id));
    check_file_state(file);

    title.serialize(file);
    check_file_state(file);
    
    content.serialize(file);
    check_file_state(file);
}

void Post::Deserialize (std::fstream& file)
{
    if (!file)
        throw std::invalid_argument("Comment.Deserialize: File is not in good state.");

    AppElement::Deserialize(file);

    file.read(reinterpret_cast<char*>(&parent_thread_id), sizeof(parent_thread_id));
    check_file_state(file);

    file.read(reinterpret_cast<char*>(&post_id), sizeof(post_id));
    check_file_state(file);

    file.read(reinterpret_cast<char*>(&author_id), sizeof(author_id));
    check_file_state(file);

    title.deserialize(file);
    check_file_state(file);
    
    content.deserialize(file);
    check_file_state(file);
}