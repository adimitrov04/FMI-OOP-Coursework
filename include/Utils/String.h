#pragma once

#ifndef __STRING_H__
#define __STRING_H__

#include <iostream>

#include "Vector.h"

class String
{

public:
    String ();
    String (const String& other);
    String (String&& other);
    String (const char* other);

    ~String();

public:
    String& operator= (const String& other);
    String& operator= (String&& other);
    String& operator= (const char* other);

    String& operator+ (const String& other);
    String& operator+ (const char* str);

    const bool operator== (const String& other) const;
    const bool operator== (const char* str) const;

    friend std::ostream& operator<< (std::ostream& out, const String& str);
    friend std::istream& operator>> (std::istream& in, String& str);

    explicit operator char*() const;

public:
    const size_t get_length () const;
    const char* c_str () const;
    Vector<String> extract_words () const;
    void print (std::ostream& out = std::cout) const;

    void cat (const char* other);
    void cat (const String& other);

    void copy (const char* str);
    void copy (const String& other);
    void clear () noexcept;

    void read (std::istream& in = std::cin);

private:
    void move (String&& other);

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