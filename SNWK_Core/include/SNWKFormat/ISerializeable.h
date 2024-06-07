#pragma once

#ifndef __ISERIALIZEABLE_INTERFACE__
#define __ISERIALIZEABLE_INTERFACE__

#include <fstream>

struct ISerializeable
{
    virtual void Serialize (std::fstream& out_file) const = 0;
    virtual void Deserialize (std::fstream& in_file) = 0;
    
    ISerializeable() = default;
    virtual ~ISerializeable() = default;
};

#endif // __ISERIALIZEABLE_INTERFACE__