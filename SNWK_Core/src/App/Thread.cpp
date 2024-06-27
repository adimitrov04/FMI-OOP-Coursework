#include <stdexcept>

#include "../../include/App/Thread.h"

// ---- CONSTANTS ----

const snwk::FourCC Thread::TYPE_FCC = {'t', 'h', 'r', 'd'};

Vector<Thread> Thread::getEmptyVector ()
{
    Vector<Thread> empty;
    return empty;
}

// ---- LIFECYCLE ----

/**
 * @warning Creates invalid object which may break a Network if it is pushed back into
 * a thread base. To be used ONLY by Vector when reserving space or by internal Thread mehtods.
 */
Thread::Thread()
: thread_id(0)
, author_id(0)
{}

/**
 * Search constructor for searching by ID.
 * 
 * @warning Creates invalid object which may break a Network if it is pushed back into
 * a thread base. To be used ONLY by internal Thread mehtods or for temporary objects when
 * searching for a Thread within a Vector.
 */
Thread::Thread(const uint32_t id)
: Thread()
{
    SetID(id);
}

/**
 * Search constructor for searching by title.
 * 
 * @warning Creates invalid object which may break a Network if it is pushed back into
 * a thread base. To be used ONLY by internal Thread mehtods or for temporary objects when
 * searching for a Thread within a Vector.
 */
Thread::Thread(const String& title)
{
    SetTitle(title);
}

Thread::Thread(uint32_t setID, uint32_t setAuthorID, const String& setTitle, Vector<Post>&& loadPosts)
: Thread()
{
    SetID(setID);
    SetAuthorID(setAuthorID);
    
    SetTitle(setTitle);
    post_list = std::move(loadPosts);
}

// ---- OPERATORS ----

Thread& Thread::operator= (const Thread& other)
{
    if (this != &other)
    {
        thread_id = other.thread_id;
        author_id = other.author_id;

        title = other.title;
        post_list = other.post_list;

        if (other.IsDeleted())
            DeleteObject();
    }

    return *this;
}

bool Thread::operator== (const Thread& other) const noexcept
{
    return thread_id == other.thread_id;
}

bool Thread::operator< (const Thread& other) const noexcept
{
    return thread_id < other.thread_id;
}

bool Thread::operator<= (const Thread& other) const noexcept
{
    return thread_id <= other.thread_id;
}

bool Thread::operator> (const Thread& other) const noexcept
{
    return thread_id > other.thread_id;
}

bool Thread::operator>= (const Thread& other) const noexcept
{
    return thread_id >= other.thread_id;
}

// ---- GETTERS ----

uint32_t Thread::GetThreadID () const noexcept
{
    return thread_id;
}

uint32_t Thread::GetAuthorID () const noexcept
{
    return author_id;
}

const String& Thread::GetTitle () const noexcept
{
    return title;
}

Post* Thread::GetPostByID (const uint32_t id) const
{
    Post search_arg(thread_id, id, 0, "", "", Comment::getEmptyVector());
    
    Post* result = post_list.binary_search(search_arg);   
    return result;
}

Thread Thread::GetDeletedVersion () const
{
    Thread deletedVer(*this);
    deletedVer.SetTitle("[deleted]");

    return deletedVer;
}

// ---- SETTERS ----

void Thread::SetID (uint32_t setID)
{
    thread_id = setID;
}

void Thread::SetAuthorID (uint32_t setAuthorID)
{
    author_id = setAuthorID;
}

void Thread::SetTitle (const String& setTitle)
{
    title = setTitle;
}

void Thread::AddPost (const Post& post, snwk::SNWKFile<Post> &postDataFile)
{
    post_list.push_back(post);

    try
    {
        postDataFile.write_object(post);
    }
    catch (...)
    {
        post_list.pop_back();
        throw;
    }
}

// ---- SERIALIZATION ----

void Thread::Serialize (std::fstream& file) const
{
    if (!file)
        throw std::invalid_argument("Thread.Serialize: File is not in good state.");

    AppElement::Serialize(file);

    file.write(reinterpret_cast<const char*>(&thread_id), sizeof(thread_id));
    check_file_state(file);

    file.write(reinterpret_cast<const char*>(&author_id), sizeof(author_id));
    check_file_state(file);

    title.serialize(file);
    check_file_state(file);
}

void Thread::Deserialize (std::fstream& file)
{
    if (!file)
        throw std::invalid_argument("Thread.Deserialize: File is not in good state.");

    AppElement::Deserialize(file);

    file.read(reinterpret_cast<char*>(&thread_id), sizeof(thread_id));
    check_file_state(file);

    file.read(reinterpret_cast<char*>(&author_id), sizeof(author_id));
    check_file_state(file);

    title.deserialize(file);
    check_file_state(file);
}

// ---- PRIVATE ----

/**
 * @warning This constructor does not copy the source's Post list
 */
Thread::Thread (const Thread& other)
: Thread()
{
    thread_id = other.thread_id;
    author_id = other.author_id;
    title = other.title;
}