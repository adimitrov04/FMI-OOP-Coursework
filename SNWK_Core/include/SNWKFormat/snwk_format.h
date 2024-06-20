#pragma once

#ifndef __SNWK_FORMAT_H__
#define __SNWK_FORMAT_H__

#include <cstdint>
#include <fstream>

namespace snwk
{

// ---- FOURCC DEFINITION ----

class FourCC
{

public:
    FourCC (char set_c1, char set_c2, char set_c3, char set_c4);
    FourCC (std::fstream& reading_file);

    bool operator== (const FourCC& other) const;
    bool operator!= (const FourCC& other) const;

public:
    void serialize (std::fstream& file) const;

public:
    char c1, c2, c3, c4;

};

// ---- FILE_HEADER DEFINITION ----

enum state
{
    good,
    closed,
    bad,
    empty
};

state getStateFromChar(const char);
char getCharFromState(const state);

class FILE_HEADER
{

public:
    FILE_HEADER (const FourCC& setRecordType);
    ~FILE_HEADER () = default;

public:
    int8_t get_size () const noexcept;

    void validate (std::fstream& file);
    void serialize (std::fstream& file) const;
    void sign_new_record (std::fstream& file);
    void clear () noexcept;

    state get_state () const noexcept;
    void set_state (state setState) noexcept;
    uint64_t get_count () const noexcept;
    void write_state (std::fstream& file, state setState);
    void reset_data_count (std::fstream& file);


private:
    static const FourCC FORMAT_HANDLE;
    const FourCC& RECORD_TYPE_SPECIFIER;

    uint64_t data_count;
    state file_state;

private:
    static constexpr int HEADER_SIZE = (sizeof(FourCC) * 2) + sizeof(uint64_t) + sizeof(char);
    static constexpr int state_pos = HEADER_SIZE - sizeof(char);
    static constexpr int count_pos = state_pos - sizeof(uint64_t);

private:
    void validate_format (std::fstream& file) const;
    void validate_type (std::fstream& file) const;

};

}

#endif // __SNWK_FORMAT_H__