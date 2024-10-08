#include <stdexcept>

#include "../../include/App/Comment.h"
//#include "../../include/App/Network.h"

// ---- CONSTANTS ----

const snwk::FourCC Comment::TYPE_FCC = {'c', 'm', 'n', 't'};

//const String Comment::VOTE_TABLE_DIR = Network::home_directory_path + "comments/"; CHANGE WHEN NETWORK IS IMPLEMENTED
const String Comment::VOTE_TABLE_DIR = String("comments") + dir::DIVIDER;

Vector<Comment> Comment::getEmptyVector()
{
    Vector<Comment> empty;
    return empty;
}

// ---- LIFECYCLE ----

/**
 * @warning Creates invalid object which may break a Network if it is pushed back into
 * a comment base. To be used ONLY by Vector when reserving space or by internal Comment mehtods.
 */
Comment::Comment()
: parent_thread_id(0)
, parent_post_id(0)
, comment_id(0)
, author_id(0)
, content()
, replying_to_id(0)
, VoteableObject()
{}

/**
 * Search constructor for searching by ID.
 * 
 * @warning Creates invalid object which may break a Network if it is pushed back into
 * a comment base. To be used ONLY by internal Comment mehtods or for temporary objects when
 * searching for a Comment within a Vector.
 */
Comment::Comment(const uint64_t id)
: Comment()
{
    SetID(id);
}

Comment::Comment(uint64_t setThreadID, uint64_t setPostID, uint64_t setID, uint64_t setAuthorID, const String& setContent, uint64_t setReplyID)
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
        set_score(other.GetScore());

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

uint64_t Comment::GetAuthorID () const noexcept
{
    return author_id;
}

uint64_t Comment::GetID () const noexcept
{
    return comment_id;
}

uint64_t Comment::GetParentThreadID () const noexcept
{
    return parent_thread_id;
}

uint64_t Comment::GetParentPostID () const noexcept
{
    return parent_post_id;
}

uint64_t Comment::GetReplyID () const noexcept
{
    return replying_to_id;
}

// ---- SETTERS ----

void Comment::SetParentThreadID (const uint64_t id)
{
    parent_thread_id = id;
}

void Comment::SetParentPostID (const uint64_t id)
{
    parent_post_id = id;
}

void Comment::SetID (const uint64_t id)
{
    comment_id = id;
}

void Comment::SetReplyID (const uint64_t id)
{
    replying_to_id = id;
}

void Comment::SetAuthorID (const uint64_t id)
{
    author_id = id;
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

Comment Comment::GetDeletedVersion () const
{
    Comment deletedVer(*this);
    deletedVer.SetContent("[deleted]");

    return deletedVer;
}

AppElement* Comment::clone () const
{
    return new Comment(*this);
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

    VoteableObject::deserialize(file);
}

// ---- PRIVATE ----

Comment::Comment (const Comment& other)
: Comment()
{
    parent_thread_id = other.parent_thread_id;
    parent_post_id = other.parent_post_id;
    comment_id = other.comment_id;

    author_id = other.author_id;
    replying_to_id = other.replying_to_id;

    content = other.content;

    if (other.IsDeleted())
        DeleteObject();
}

String Comment::generate_vote_table_filename() const
{
    String thread_id = string::parseToString(parent_thread_id);
    String post_id = string::parseToString(parent_post_id);
    String comm_id = string::parseToString(comment_id);

    return VOTE_TABLE_DIR + (thread_id + "_" + post_id + "_" + comm_id + ".snwk");
}