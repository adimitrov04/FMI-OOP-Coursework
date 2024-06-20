#pragma once

#ifndef __VOTEABLE_OBJECT_H__
#define __VOTEABLE_OBJECT_H__

#include <cstdint>
#include <fstream>

#include "../project_utils.h"

#include "VoteTable.h"
#include "User.h"

class VoteableObject
{

public:
    VoteableObject(String setVoteTableFileName = "null");
    virtual ~VoteableObject() = default;

public:
    int32_t GetScore () const;

    void Upvote (const User& voter);
    void Downvote (const User& voter);
    void Unvote (const User& voter);

    void serialize (std::fstream& file) const;
    void deserialize (std::fstream& file);

protected:
    void set_vote_table_name (const String& fileName);
    const String& get_vote_table_name () const;
    void set_score (const int32_t setScore);

protected:
    VoteTable vote_table;

private:
    void Vote (vote_values value, const User& voter);

private:
    String vote_table_path;
    int32_t score;

};

#endif // __VOTEABLE_OBJECT_H__