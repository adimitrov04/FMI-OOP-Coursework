#include <cstdint>
#include <iostream>

#include "../../include/App/VoteableObject.h"

// ---- LIFECYCLE ----

VoteableObject::VoteableObject(String setVoteTableFileName)
: score(0)
, vote_table_path(setVoteTableFileName)
{}

// ---- METHODS ----

int32_t VoteableObject::GetScore () const
{
    return score;
}

void VoteableObject::Upvote (const User& voter, User& author)
{
    vote(vote_values::upvote, voter, author);
}

void VoteableObject::Downvote (const User& voter, User& author)
{
    vote(vote_values::downvote, voter, author);
}

void VoteableObject::Unvote (const User& voter, User& author)
{
    vote(vote_values::none, voter, author);
}

void VoteableObject::serialize (std::fstream& file) const
{
    //std::clog << "write score: " << score << '\n';
    file.write(reinterpret_cast<const char*>(&score), sizeof(score));
    if (!file)
        throw std::runtime_error("VoteableObject.serialize: Writing score failed.");
}

void VoteableObject::deserialize (std::fstream& file)
{
    int32_t setScore = 0;
    file.read(reinterpret_cast<char*>(&setScore), sizeof(setScore));
    if (!file)
        throw std::runtime_error("VoteableObject.deserialize: Reading score failed.");
    
    //std::clog << "read score: " << setScore << '\n';
    score = setScore;
}

// ---- PROTECTED ----

void VoteableObject::set_vote_table_name (const String& fileName)
{
    vote_table_path = fileName;
}

const String& VoteableObject::get_vote_table_name () const
{
    return vote_table_path;
}

void VoteableObject::set_score (const int32_t setScore)
{
    score = setScore;
}

// ---- PRIVATE ----

void VoteableObject::vote (vote_values val, const User& voter, User& author)
{
    vote_table.LoadTable(vote_table_path.c_str());
    VoteEntry* currentUserEntry = nullptr;
    
    if (vote_table.GetEntryCount() > 0)
        currentUserEntry = vote_table.FindEntry(voter.GetID());

    // Safeguard just in case
    if (voter.IsDeleted())
        throw std::invalid_argument("Vote: Given user cannot vote since they are deleted.");

    if (currentUserEntry == nullptr)
    {
        if (val == none)
            throw std::logic_error("VoteableObject.Vote: User has not voted on this object yet and therefore cannot unvote.");
            
        VoteEntry makeEntry(voter.GetID(), val);  
        vote_table.AddEntry(makeEntry);

        try
        {
            vote_table.SaveTable(vote_table_path.c_str());
        }
        catch (std::exception& e)
        {
            vote_table.Clear();
            throw;
        }
        
        vote_table.Clear();

        score += val;
        author.AddScore(val);
        return;
    }

    if (val == none)
    {
        int8_t oldValue = currentUserEntry->GetValue();
        
        try
        {
            vote_table.RemoveEntry(voter.GetID());
            vote_table.SaveTable(vote_table_path.c_str());
        }
        catch (...)
        {
            vote_table.Clear();
            throw std::runtime_error("VoteableObject.Vote: Vote table modification failed, object is unchanged.");
        }

        vote_table.Clear();

        score -= oldValue;
        author.AddScore(-oldValue);
        return;
    }

    if (val == currentUserEntry->GetValue())
    {
        vote_table.Clear();
        throw std::logic_error("VoteableObject.Vote: User cannot vote twice on the same object.");
    }

    vote_values oldValue = currentUserEntry->GetValue();
    currentUserEntry->SetValue(val);

    try
    {
        vote_table.SaveTable(vote_table_path.c_str());
    }
    catch (std::exception&)
    {
        vote_table.Clear();
        throw;
    }
    
    vote_table.Clear();
    
    score = (score - oldValue) + val;
    author.AddScore(val - oldValue);
}