#pragma once

#ifndef __THREAD_H__
#define __THREAD_H__

#include "../Utils/Vector.h"

#include "../SNWKFormat/ISerializeable.h"

#include "User.h"
#include "Post.h"

class Thread : public ISerializeable
{

public:
    Thread();
    ~Thread();

public:
    virtual void Serialize (std::fstream& file) const override;
    virtual void Deserialize (std::fstream& file) override;

private:
    Vector<Post> post_list;

};

#endif // __THREAD_H__