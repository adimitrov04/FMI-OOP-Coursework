#pragma once

#ifndef __ISCOREABLE_H__
#define __ISCOREABLE_H__

#include <cstdint>

class IScoreable
{

public:
    IScoreable()
    : score(0)
    {}

    virtual ~IScoreable() = default;

public:
    int32_t GetScore () const;
    virtual void Upvote ();
    virtual void Downvote ();

protected:
    int32_t score;

};

#endif // __ISCOREABLE_H__