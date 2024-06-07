/*
#pragma once

#ifndef __SNWK_EXCEPTIONS_H__
#define __SNWK_EXCEPTIONS_H__

#include <stdexcept>

namespace snwkexcept
{

class bad_file : public std::runtime_error
{

public:
    bad_file() = delete;
    bad_file(const char* what_arg) : std::runtime_error(what_arg) {}

};

class invalid_format : public snwkexcept::bad_file
{

public:
    invalid_format() = delete;
    invalid_format(const char* what_arg) : snwkexcept::bad_file(what_arg) {}

};

class type_mismatch : public snwkexcept::bad_file
{

public:
    type_mismatch() = delete;
    type_mismatch(const char* what_arg) : snwkexcept::bad_file(what_arg) {}

};

class corrupt_data : public snwkexcept::bad_file
{

public:
    corrupt_data() = delete;
    corrupt_data(const char* what_arg) : snwkexcept::bad_file(what_arg) {}

};

}

#endif // __SNWKEXCEPTIONS_H__
*/