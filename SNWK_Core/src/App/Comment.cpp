#include <stdexcept>

#include "../../include/App/Comment.h"
//#include "../../include/App/Network.h"

// ---- CONSTANTS ----

const snwk::FourCC Comment::TYPE_FCC = {'c', 'm', 'n', 't'};

//const String Comment::VOTE_TABLE_DIR = Network::home_directory_path + "comments/"; CHANGE WHEN NETWORK IS IMPLEMENTED
const String Comment::VOTE_TABLE_DIR = "comments/";

// ---- LIFECYCLE ----

Comment::Comment()
: parent_thread_id(0)
, parent_post_id(0)
, comment_id(0)
, author_id(0)
, content()
, replying_to_id(0)
, VoteableObject()
{}

Comment::Comment(uint32_t setThreadID, uint32_t setPostID, uint32_t setID, uint32_t setAuthorID, const String& setContent, uint32_t setReplyID)
: Comment()
{
    parent_thread_id = setThreadID;
    parent_post_id = setPostID;
    SetID(setID);

    author_id = setAuthorID;
    SetContent(setContent);
    replying_to_id = setReplyID;
    
    set_vote_table_name(generate_vote_table_filename());
}

// ---- OPERATORS ----

Comment& Comment::operator= (const Comment& other)
{
    if (this != &other)
    {
        parent_post_id = other.parent_post_id;
        comment_id = other.comment_id;

        author_id = other.author_id;
        content = other.content;
        replying_to_id = other.replying_to_id;
     
        set_vote_table_name(other.get_vote_table_name());

        if (other.IsDeleted())
            DeleteObject();
    }

    return *this;
}

bool Comment::operator== (const Comment& other) const noexcept
{
    return comment_id == other.comment_id;
}
    
bool Comment::operator< (const Comment& other) const noexcept
{
    return comment_id < other.comment_id;
}

bool Comment::operator<= (const Comment& other) const noexcept
{
    return comment_id <= other.comment_id;
}

bool Comment::operator> (const Comment& other) const noexcept
{
    return comment_id > other.comment_id;
}

bool Comment::operator>= (const Comment& other) const noexcept
{
    return comment_id >= other.comment_id;
}

// ---- GETTERS ----

const String& Comment::GetContent () const noexcept
{
    return content;
}

uint32_t Comment::GetAuthorID () const noexcept
{
    return author_id;
}

uint32_t Comment::GetID () const noexcept
{
    return comment_id;
}

uint32_t Comment::GetParentThreadID () const noexcept
{
    return parent_thread_id;
}

uint32_t Comment::GetParentPostID () const noexcept
{
    return parent_post_id;
}

uint32_t Comment::GetReplyID () const noexcept
{
    return replying_to_id;
}

// ---- SETTERS ----

void Comment::SetID (const uint32_t id)
{
    comment_id = id;
}

void Comment::SetContent (const String& text)
{
    content = text;
}

// ---- VOTING ----

void Comment::Upvote (const User& voter, User& author)
{
    if (IsDeleted())
        throw std::invalid_argument("Comment.vote: Cannot vote on a deleted comment.");

    VoteableObject::Upvote(voter, author);
}

void Comment::Downvote (const User& voter, User& author)
{
    if (IsDeleted())
        throw std::invalid_argument("Comment.vote: Cannot vote on a deleted comment.");

    VoteableObject::Downvote(voter, author);
}

void Comment::Unvote (const User& voter, User& author)
{
    if (IsDeleted())
        throw std::invalid_argument("Comment.vote: Cannot vote on a deleted comment.");

    VoteableObject::Unvote(voter, author);
}

// ---- SERIALIZATION ----

void Comment::Serialize (std::fstream& file) const
{
    if (!file)
        throw std::invalid_argument("Comment.Serialize: File is not in good state.");

    AppElement::Serialize(file);

    file.write(reinterpret_cast<const char*>(&parent_thread_id), sizeof(parent_thread_id));
    check_file_state(file);

    file.write(reinterpret_cast<const char*>(&parent_post_id), sizeof(parent_post_id));
    check_file_state(file);

    file.write(reinterpret_cast<const char*>(&comment_id), sizeof(comment_id));
    check_file_state(file);

    file.write(reinterpret_cast<const char*>(&author_id), sizeof(author_id));
    check_file_state(file);
    
    file.write(reinterpret_cast<const char*>(&replying_to_id), sizeof(replying_to_id));
    check_file_state(file);
    
    content.serialize(file);
    check_file_state(file);

    VoteableObject::serialize(file);
}

void Comment::Deserialize (std::fstream& file)
{
    if (!file)
        throw std::invalid_argument("Comment.Deserialize: File is not in good state.");

    AppElement::Deserialize(file);

    file.read(reinterpret_cast<char*>(&parent_thread_id), sizeof(parent_thread_id));
    check_file_state(file);

    file.read(reinterpret_cast<char*>(&parent_post_id), sizeof(parent_post_id));
    check_file_state(file);

    file.read(reinterpret_cast<char*>(&comment_id), sizeof(comment_id));
    check_file_state(file);

    file.read(reinterpret_cast<char*>(&author_id), sizeof(author_id));
    check_file_state(file);
    
    file.read(reinterpret_cast<char*>(&replying_to_id), sizeof(replying_to_id));
    check_file_state(file);
    
    content.deserialize(file);
    check_file_state(file);

    VoteableObject::serialize(file);
}

// ---- PRIVATE ----

String Comment::generate_vote_table_filename() const
{
    String thread_id = string::parseToString(parent_thread_id);
    String post_id = string::parseToString(parent_post_id);
    String comm_id = string::parseToString(comment_id);

    return VOTE_TABLE_DIR + (thread_id + "_" + post_id + "_" + comm_id + ".snwk");
}