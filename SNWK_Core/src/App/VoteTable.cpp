#include <stdexcept>

#include "../../include/App/VoteTable.h"

// ---- VOTEENTRY ----

const snwk::FourCC VoteEntry::TYPE_FCC = {'v', 'o', 't', 'e'};

VoteEntry::VoteEntry(const uint64_t setID, const vote_values setVal) noexcept
: user_id(setID)
, vote(setVal)
{}

vote_values VoteEntry::GetValue () const
{
    return (vote_values)vote;
}

void VoteEntry::SetValue (const vote_values val)
{
    if (val == none)
        throw std::invalid_argument("VoteEntry.SetValue: Cannot set value to 'none'.");

    vote = val;
}

void VoteEntry::Serialize (std::fstream& file) const
{
    if (!file)
        throw std::invalid_argument("VoteEntry.serialize: File is not in good state.");

    file.write(reinterpret_cast<const char*>(&user_id), sizeof(user_id));
    check_file_state(file);

    file.write(reinterpret_cast<const char*>(&vote), sizeof(vote));
    check_file_state(file);
}

void VoteEntry::Deserialize (std::fstream& file)
{
    if (!file)
        throw std::invalid_argument("VoteEntry.deserialize: File is not in good state.");

    file.read(reinterpret_cast<char*>(&user_id), sizeof(user_id));
    check_file_state(file);

    file.read(reinterpret_cast<char*>(&vote), sizeof(vote));
    check_file_state(file);
}

// ---- OPERATORS ----

bool VoteEntry::operator== (const VoteEntry& other) const
{
    return user_id == other.user_id;
}

bool VoteEntry::operator< (const VoteEntry& other) const
{
    return user_id < other.user_id;
}

bool VoteEntry::operator<= (const VoteEntry& other) const
{
    return user_id <= other.user_id;
}

bool VoteEntry::operator> (const VoteEntry& other) const
{
    return user_id > other.user_id;
}

bool VoteEntry::operator>= (const VoteEntry& other) const
{
    return user_id >= other.user_id;
}

// ---- VOTETABLE ----

void VoteTable::LoadTable (const char* path)
{
    snwk::SNWKFile<VoteEntry> inputFile;
    
    // TODO: Fix this cause it doesn't check anything properly. Will work but is unsafe.
    try
    {
        inputFile.open(path);
    }
    catch (/*snwk::file_not_found*/...) // TODO: SNWK exceptions and file exists check
    {
        inputFile.create(path);
    }

    for (uint64_t i = 0; i < inputFile.get_object_count(); i++)
    {
        VoteEntry outObject;
        inputFile.read_object(outObject);

        entries.push_back(outObject);
    }

    inputFile.close();
}

void VoteTable::SaveTable (const char* path)
{
    snwk::SNWKFile<VoteEntry> outFile;
    outFile.create(path);

    for (uint64_t i = 0; i < entries.size(); i++)
        outFile.write_object(entries[i]);

    outFile.close();
}

uint64_t VoteTable::GetEntryCount () const noexcept
{
    return entries.size();
}

void VoteTable::Clear () noexcept
{
    entries.clear();
}

VoteEntry* VoteTable::FindEntry (const uint64_t id) const noexcept
{
    return entries.binary_search(VoteEntry(id));
}

void VoteTable::AddEntry (const VoteEntry& inEntry)
{
    if (inEntry.GetValue() == none)
        throw std::invalid_argument("VoteTable.AddEntry: Cannot add entry with a value of 'none'.");

    entries.push_sorted(inEntry);
}

void VoteTable::RemoveEntry (const uint64_t entryID)
{
    if (FindEntry(entryID) == nullptr)
        throw std::invalid_argument("VoteTable.RemoveEntry: Given entry is not in table.");

    uint64_t removeIndex = (FindEntry(entryID) - entries.front());
    entries.pop(removeIndex);
}