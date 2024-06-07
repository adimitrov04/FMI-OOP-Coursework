#pragma once

#ifndef __COMMENT_H__
#define __COMMENT_H__

#include "../SNWKFormat/ISerializeable.h"
#include "Interfaces/IScoreable.h"

#include "User.h"

class Comment : public ISerializeable, public IScoreable
{

public:
    Comment();
    ~Comment();

public:
    virtual void Serialize (std::fstream& file) const override;
    virtual void Deserialize (std::fstream& file) override;

private:
    const User& author;
    String content;
    Comment& replying_to;

};

#endif // __COMMENT_H__