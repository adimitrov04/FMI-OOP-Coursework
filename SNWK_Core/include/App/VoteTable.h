#pragma once

#ifndef __VOTETABLE_H__
#define __VOTETABLE_H__

#include <cstdint>

#include "../project_utils.h"
#include "../snwk_lib.h"

#include "../SNWKFormat/ISerializeable.h"

enum vote_values
{
    downvote = -1,
    upvote = 1,
    none = 0
};

class VoteEntry : public ISerializeable
{

public:
    VoteEntry(const uint32_t setID, const vote_values setVal);

public:
    vote_values GetValue () const;
    void SetValue (const vote_values val);

    virtual void Serialize (std::fstream& file) const override;
    virtual void Deserialize (std::fstream& file) override;

public:
    //VoteEntry& operator= (const VoteEntry& other);
    bool operator== (const VoteEntry& other) const;

public:
    static const snwk::FourCC TYPE_FCC;

private:
    uint32_t user_id = 0;
    int8_t vote = 0;

};

class VoteTable
{

public:
    VoteTable() = default;
    ~VoteTable() = default;

public:
    void LoadTable (const char* path);
    void SaveTable (const char* path);

    VoteEntry* FindEntry (const uint32_t id) const;
    void AddEntry (const VoteEntry& inEntry);
    void RemoveEntry (const uint32_t entryID);

private:
    Vector<VoteEntry> entries;

};

#endif // __VOTETABLE_H__