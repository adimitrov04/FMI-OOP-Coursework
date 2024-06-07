#include "../../include/SNWKFormat/snwk_format.h"
#include "../../include/SNWKFormat/snwk_exceptions.h"

using namespace snwk;

const FourCC FILE_HEADER::FORMAT_HANDLE = {'s', 'n', 'w', 'k'};

// -------- UTILS --------

state snwk::getStateFromChar (const char ch)
{
    switch (ch)
    {
        case 'o':
            return state::good;
        case 's':
            return state::closed;
        case 'b':
            return state::bad;
        default:
            throw std::invalid_argument("State symbol is not recognized.");
    }
}

char snwk::getCharFromState(const state st)
{
    switch (st)
    {
        case good:
            return 'o';
        case closed:
            return 's';
        case bad:
            return 'b';
        default:
            throw std::invalid_argument("State is unrecognized or state::empty.");
    }
}

// -------- TYPE: FOURCC --------

FourCC::FourCC (char set_c1, char set_c2, char set_c3, char set_c4)
: c1(set_c1)
, c2(set_c2)
, c3(set_c3)
, c4(set_c4)
{}

FourCC::FourCC (std::fstream& file)
{
    file.read(&c1, sizeof(c1));
    file.read(&c2, sizeof(c2));
    file.read(&c3, sizeof(c3));
    file.read(&c4, sizeof(c4));
}

void FourCC::serialize (std::fstream& file) const
{
    file.write(&c1, sizeof(c1));
    file.write(&c2, sizeof(c2));
    file.write(&c3, sizeof(c3));
    file.write(&c4, sizeof(c4));
}

bool FourCC::operator== (const FourCC& other) const
{
    if (c1 == other.c1 && c2 == other.c2 && c3 == other.c3 && c4 == other.c4)
        return true;

    return false;
}

bool FourCC::operator!= (const FourCC& other) const
{
    return !(*this == other);
}

// -------- TYPE: FILE_HEADER --------

// ---- LIFECYCLE ----

FILE_HEADER::FILE_HEADER(const FourCC& setRecordType)
: RECORD_TYPE_SPECIFIER(setRecordType)
, data_count(0)
, file_state(state::empty)
{}

// ---- METHODS ----

int8_t FILE_HEADER::get_size () const noexcept
{
    return HEADER_SIZE;
}

void FILE_HEADER::validate (std::fstream& file)
{
    validate_format(file);
    validate_type(file);

    file.read(reinterpret_cast<char*>(&data_count), sizeof(data_count));
    
    char stateChar;
    file.read(&stateChar, sizeof(stateChar));

    if (getStateFromChar(stateChar) == state::bad)
        throw std::runtime_error("SNWKFile.open: Specified file has been corrupted.");

    if (getStateFromChar(stateChar) == state::good)
        throw std::runtime_error("SNWKFile.open: Specified file is already in use.");

    write_state(file, state::good);
    set_state(state::good);
}

void FILE_HEADER::serialize (std::fstream& file) const
{
    FORMAT_HANDLE.serialize(file);
    RECORD_TYPE_SPECIFIER.serialize(file);

    file.write(reinterpret_cast<const char*>(&data_count), sizeof(data_count));
    
    char writeState = getCharFromState(file_state);
    file.write(&writeState, sizeof(char));
}

void FILE_HEADER::sign_new_record (std::fstream& file)
{
    data_count++;
    file.seekp(count_pos);
    file.write(reinterpret_cast<char*>(&data_count), sizeof(data_count));

    file.seekp(0, std::ios_base::end);
}

void FILE_HEADER::clear () noexcept
{
    data_count = 0;
    file_state = state::empty;
}

state FILE_HEADER::get_state () const noexcept
{
    return file_state;
}

void FILE_HEADER::set_state (state setState) noexcept
{
    file_state = setState;
}

uint64_t FILE_HEADER::get_count () const noexcept
{
    return data_count;
}

void FILE_HEADER::write_state (std::fstream& file, state setState)
{
    char stateChar = getCharFromState(setState);

    file.seekp(state_pos, std::ios_base::beg);
    file.write(&stateChar, sizeof(char));
    
    file.seekp(0, std::ios_base::end);
}

// ---- PRIVATE ----

void FILE_HEADER::validate_format (std::fstream& file) const
{
    FourCC arg(file);
    
    if (arg != FORMAT_HANDLE)
        throw std::runtime_error("SNWK.open: Given file is not a SNWK file.");
}

void FILE_HEADER::validate_type (std::fstream& file) const
{
    FourCC arg(file);

    if (arg != RECORD_TYPE_SPECIFIER)
        throw /*snwkexcept::type_mismatch*/std::runtime_error("SNWK.open: Objects in given file are not of the specified type.");
}