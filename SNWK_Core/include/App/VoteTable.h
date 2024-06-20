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
    none = 0,
    upvote = 1
};

class VoteEntry : public ISerializeable
{

public:
    VoteEntry(const uint32_t setID = 0, const vote_values setVal = none) noexcept;

public:
    vote_values GetValue () const;
    void SetValue (const vote_values val);

    virtual void Serialize (std::fstream& file) const override;
    virtual void Deserialize (std::fstream& file) override;

public:
    bool operator== (const VoteEntry& other) const;
    
    bool operator< (const VoteEntry& other) const;
    bool operator<= (const VoteEntry& other) const;

    bool operator> (const VoteEntry& other) const;
    bool operator>= (const VoteEntry& other) const;

public:
    static const snwk::FourCC TYPE_FCC;

private:
    uint32_t user_id;
    int8_t vote;

};

class VoteTable
{

public:
    VoteTable() = default;
    ~VoteTable() = default;

public:
    void LoadTable (const char* path);
    void SaveTable (const char* path);
    uint64_t GetEntryCount () const noexcept;
    void Clear () noexcept;

    VoteEntry* FindEntry (const uint32_t id) const noexcept;
    void AddEntry (const VoteEntry& inEntry);
    void RemoveEntry (const uint32_t entryID);

private:
    Vector<VoteEntry> entries;

};

#endif // __VOTETABLE_H__