#pragma once

#ifndef __VOTEABLE_OBJECT_H__
#define __VOTEABLE_OBJECT_H__

#include <cstdint>
#include <fstream>

#include "../project_utils.h"

#include "VoteTable.h"
#include "User.h"

/**
 * Abstract class for objects which support a scoring system. Each class implementing
 * this abstraction will have a `score` member and an associated `VoteTable` with a
 * specified path.
 * 
 * @attention Every class implementing this abstraction is required to implement
 * a copy assignment operator which manually assigns the `vote_table_path` and `score`
 * values from the copied object
 * @attention If implementation class also implements the `ISerializable` interface,
 * the `VoteableObject` serialization MUST also be called during object serialization
 */
class VoteableObject
{

public:
    VoteableObject(String setVoteTableFileName = "");
    virtual ~VoteableObject() = default;

public:
    int32_t GetScore () const;

    virtual void Upvote (const User& voter, User& author);
    virtual void Downvote (const User& voter, User& author);
    virtual void Unvote (const User& voter, User& author);

    void serialize (std::fstream& file) const;
    void deserialize (std::fstream& file);

protected:
    void set_vote_table_name (const String& fileName);
    const String& get_vote_table_name () const;
    void set_score (const int32_t setScore);

protected:
    VoteTable vote_table;

private:
    void vote (vote_values value, const User& voter, User& author);

private:
    String vote_table_path;
    int32_t score;

};

#endif // __VOTEABLE_OBJECT_H__