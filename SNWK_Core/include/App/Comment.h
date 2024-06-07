#pragma once

#ifndef __COMMENT_H__
#define __COMMENT_H__

#include "../SNWKFormat/ISerializeable.h"
#include "Interfaces/IScoreable.h"

#include "User.h"

class Comment : public IScoreable
{

public:
    Comment();
    ~Comment();

public:


private:
    const User& author;
    String content;
    Comment& replying_to;

};

#endif // __COMMENT_H__