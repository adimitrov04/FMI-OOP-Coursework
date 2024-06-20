#pragma once

#ifndef __SNWK_FILE_H__
#define __SNWK_FILE_H__

#include <cstdint>
#include <fstream>
#include <stdexcept>

#include "snwk_format.h"

namespace snwk
{

enum write_mode
{
    // Moves the writing pointer to the end of the file before writing
    append,
    // Does not move the write pointer from its current position, overwriting any data the pointer is currently at (UNSAFE)
    overwrite,
    // Signs the data count as 0 and moves the write pointer to the end of the file header. Used to rewrtire files from the start.
    rewrite
};

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
    void write_object (const ObjectType& inObj, write_mode mode = append);

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