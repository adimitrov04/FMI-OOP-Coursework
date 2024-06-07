#pragma once

#ifndef __SNWK_FILE_H__
#define __SNWK_FILE_H__

#include <cstdint>
#include <fstream>
#include <stdexcept>

#include "snwk_format.h"

namespace snwk
{

template <class ObjectType>
class SNWKFile
{

public:
    SNWKFile() noexcept;
    SNWKFile(const char* file_path);

    ~SNWKFile() noexcept;

public:
    bool operator!() noexcept;

public:
    void create (const char* file_path);
    void open (const char* file_path);
    void close () noexcept;

    void read_object (ObjectType& outObj);
    void write_object (const ObjectType& inObj);

    inline bool is_good () noexcept;
    snwk::state get_state () const noexcept;
    uint64_t get_object_count () const noexcept;

private:
    SNWKFile(const SNWKFile& other) = delete;
    SNWKFile& operator= (const SNWKFile& other) = delete;

private:
    std::fstream file_stream;
    FILE_HEADER metadata;

};

}

#include "../../src/SNWKFormat/snwk_file.ipp"


#endif // __SNWK_FILE_H__