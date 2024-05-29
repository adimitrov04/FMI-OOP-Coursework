#pragma once

#ifndef __STRING_H__
#define __STRING_H__

#include <iostream>

#include "Vector.h"

class String
{

public:
    String () noexcept;
    String (const String& other);
    String (String&& other) noexcept;
    String (const char* other);

    ~String() noexcept;

public:
    String& operator= (const String& other);
    String& operator= (String&& other) noexcept;
    String& operator= (const char* other);

    String& operator+ (const String& other);
    String& operator+ (const char* str);

    const bool operator== (const String& other) const noexcept;
    const bool operator== (const char* str) const noexcept;

    friend std::ostream& operator<< (std::ostream& out, const String& str) noexcept;
    friend std::istream& operator>> (std::istream& in, String& str);

public:
    const size_t get_length () const noexcept;
    const char* c_str () const noexcept;
    Vector<String> extract_words () const;
    void print (std::ostream& out = std::cout) const noexcept;

    void cat (const char* other);
    void cat (const String& other);

    void copy (const char* str);
    void copy (const String& other);
    void clear () noexcept;

    void read (std::istream& in = std::cin);

private:
    void move (String&& other) noexcept;

private:
    static const size_t MAX_BUFFER_LENGTH = 100000;

    size_t size;
    char* arr;

};

namespace string_utils
{
    const bool isnewline (const char ch);
    String getCurrentWordInString (const char* &word);
    const char* findNextWord (const char* currentPosition);
    const size_t getCurrentWordLength (const char* word);
    void copyWord (char* dest, const char* &src);
}

#endif // __STRING_H__