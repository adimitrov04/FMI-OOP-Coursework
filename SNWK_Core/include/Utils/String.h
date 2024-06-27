#pragma once

#ifndef __STRING_H__
#define __STRING_H__

#include <iostream>
#include <fstream>
#include <cstdint>

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
    String operator+ (const String& other) const;
    String& operator+ (const char* str);
    String operator+ (const char* str) const;

    bool operator== (const char* str) const;
    bool operator== (const String& other) const;

    bool operator!= (const char* str) const;
    bool operator!= (const String& other) const;

    bool operator> (const char* other) const;
    bool operator> (const String& other) const;
    
    bool operator< (const char* other) const;
    bool operator< (const String& other) const;

    bool operator>= (const char* other) const;
    bool operator>= (const String& other) const;
    
    bool operator<= (const char* other) const;
    bool operator<= (const String& other) const;

    operator bool() const noexcept;

    friend std::ostream& operator<< (std::ostream& out, const String& str) noexcept;
    friend std::istream& operator>> (std::istream& in, String& str);

public:

    const uint64_t length () const noexcept;
    const char* c_str () const noexcept;

    char last () const;

    Vector<String> extract_words () const;
    void print (std::ostream& out = std::cout) const noexcept;

    void cat (const char* other);
    void cat (const String& other);

    void copy (const char* str);
    void copy (const String& other);
    void clear () noexcept;

    void read (std::istream& in = std::cin);
    void serialize (std::fstream& file) const;
    void deserialize (std::fstream& file);

public:
    static const uint64_t MAX_BUFFER_LENGTH = 100000;

private:
    void move (String&& other) noexcept;

private:
    uint64_t size;
    char* arr;

};

namespace string_utils
{
    const bool isnewline (const char ch) noexcept;
    const uint64_t strlen (const char* str, const uint64_t LIMIT = String::MAX_BUFFER_LENGTH);
    const int strcmp (const char* str1, const char* str2);
    const int strcasecmp (const char* str1, const char* str2);
    const char* strstr (const char* bigStr, const char* smallStr);

    String getCurrentWordInString (const char* &word);
    const char* findNextWord (const char* currentPosition);
    const uint64_t getCurrentWordLength (const char* word);
    void copyWord (char* dest, const char* &src);

    void add_char (char* dest, const char ch);
    void reverse_cstr (char* str);
}

namespace string
{

String parseToString (const uint32_t num);

}


#endif // __STRING_H__