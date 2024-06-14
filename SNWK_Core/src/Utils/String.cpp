#include <iostream>
#include <cstddef>
#include <stdexcept>

#include <cmath>
#include <fstream>
#include <cstring>

#include "../../include/Utils/String.h"

// -- UTILITIES --

using namespace string_utils;

const bool string_utils::isnewline (const char ch)
{
    if (ch == '\r' || ch == '\n')
        return true;

    return false;
}

/**
 * Safe strlen implementation - combines checks of strnlen and checks if str is nullptr.
 * 
 * @param LIMIT the length limit, won't check past this point.
 * 
 * @return The length of the cstring up to the null-termination character;
 * @return (LIMIT + 1) if the cstring is not null-terminated
 */
const size_t string_utils::strlen (const char* str, const size_t LEN_LIMIT)
{
    if (!str)
        return 0;

    size_t len(0);
    while (*str && len <= LEN_LIMIT)
    {
        len++;
        str++;    
    }

    return len;
}

/**
 * Safe strcmp implementation - checks if str1, str2 are nullptr and whether they are terminated.
 * 
 * @throws std::invalid_argument if either str1 or str2 is not a valid, null-terminated cstring
 * 
 * @return The length of the cstring up to the null-termination character;
 * @return (LIMIT + 1) if the cstring is not null-terminated
 */
const int string_utils::strcmp (const char* str1, const char* str2)
{
    if (!str1 || !str2)
        throw std::invalid_argument("strcmp: Cannot compare nullptrs.");

    if (strlen(str1) == String::MAX_BUFFER_LENGTH || strlen(str2) == String::MAX_BUFFER_LENGTH)
        throw std::invalid_argument("strcmp: Argument cstrings are not null-terminated.");

    int diff(0);
    while (*str1 && *str2)
    {
        if (*str1 != *str2)
        {
            diff = *str1 - *str2;
            break;
        }

        str1++;
        str2++;
    }

    return diff;
}

const int string_utils::strcasecmp (const char* str1, const char* str2)
{
    if (!str1 || !str2)
        throw std::invalid_argument("strcasecmp: Cannot compare nullptrs.");

    if (strlen(str1) == String::MAX_BUFFER_LENGTH || strlen(str2) == String::MAX_BUFFER_LENGTH)
        throw std::invalid_argument("strcasecmp: Argument cstrings are not null-terminated.");

    const int CASE_DIFF = 'a' - 'A';

    int diff(0);
    while (*str1 && *str2)
    {
        if (*str1 != *str2)
        {
            if (!isalpha(*str1) || !isalpha(*str2) || std::abs(*str1 - *str2) != CASE_DIFF)
            {
                diff = (*str1 - *str2);
                break;
            }
        }

        str1++;
        str2++;
    }

    return diff;
}

void string_utils::copyWord (char* dest, const char* &src)
{
    while (*src && !isspace(*src) && !isnewline(*src))
    {
        *dest = *src;

        src++;
        dest++;
    }

    *dest = '\0';
}

const size_t string_utils::getCurrentWordLength (const char* word)
{
    size_t wordSize = 0;

    while (*word && !isspace(*word) && !isnewline(*word))
    {
        wordSize++;
        word++;
    }

    return wordSize;
}

const char* string_utils::findNextWord (const char* currentPos)
{
    const char* iterator = currentPos;

    if (!iterator)
        return nullptr;

    while (*iterator && (isspace(*iterator) || isnewline(*iterator)))
        iterator++;

    if (*iterator == '\0')
        return nullptr;

    return iterator;
}

String string_utils::getCurrentWordInString (const char* &word)
{
    if (getCurrentWordLength(word) == 0)
        throw std::out_of_range("String.getCurrentWordInString: no words left to extract");

    char* buffer = new char[getCurrentWordLength(word) + 1];

    copyWord(buffer, word);

    try
    {
        String output(buffer);

        delete[] buffer;
        return output;
    }
    catch (...)
    {
        delete[] buffer;
        throw;
    }
}

// ---- LIFECYCLE ----

String::String() noexcept
: size(0), arr(nullptr)
{}

String::String (const char* str)
: String()
{
    size_t len = string_utils::strlen(str, MAX_BUFFER_LENGTH);
    if (!len || len > MAX_BUFFER_LENGTH)
        throw std::invalid_argument("String.ctor: Cstring argument is not null-terminated or is nullptr.");

    size = len + 1;
    arr = new char[size];

    strcpy(arr, str);
}

String::String (const String& other)
: String()
{
    if (other.size == 0)
        return;

    char* buffer = new char[other.size];
    strcpy(buffer, other.c_str());

    arr = buffer;
    size = other.size;
}

String::String (String&& other) noexcept
: String()
{
    move(std::move(other));
}

String::~String() noexcept
{
    clear();
}

// ---- OPERATORS ----

String& String::operator= (const String& other)
{
    if (this != &other)
        copy(other);

    return *this;
}

String& String::operator= (String&& other) noexcept
{
    if (this != &other)
    {
        clear();
        move(std::move(other));
    }

    return *this;
}

String& String::operator= (const char* str)
{
    copy(str);

    return *this;
}

String& String::operator+ (const String& other)
{
    this->cat(other);

    return *this;
}

String String::operator+ (const String& str) const
{
    String result(*this);
    result.cat(str);

    return result;
}


String& String::operator+ (const char* str)
{
    this->cat(str);

    return *this;
}

String String::operator+ (const char* str) const
{
    String result(*this);
    result.cat(str);

    return result;
}

const bool String::operator== (const String& other) const
{
    return (string_utils::strcmp(this->c_str(), other.c_str()) == 0);
}

const bool String::operator== (const char* str) const
{
    return (string_utils::strcmp(this->c_str(), str) == 0);
}

std::ostream& operator<< (std::ostream& out, const String& str) noexcept
{
    out << str.c_str();

    return out;
}

std::istream& operator>> (std::istream& in, String& str)
{
    str.read(in);

    return in;
}

// ---- GETTERS ----

const size_t String::length () const noexcept
{
    return string_utils::strlen(arr);
}

const char* String::c_str () const noexcept
{
    return arr;
}

Vector<String> String::extract_words () const
{
    Vector<String> outputVector;

    const char* iterator = findNextWord(arr);
    
    while (iterator)
    {
        String currentWord = getCurrentWordInString(iterator);
        outputVector.push_back(currentWord);

        iterator = findNextWord(iterator);
    }

    return outputVector;
}


void String::print (std::ostream& out) const noexcept
{
    if (arr == nullptr)
        out << "null";

    out << arr;
}

// ---- SETTERS ----

void String::cat (const char* str)
{
    size_t len = string_utils::strlen(str, MAX_BUFFER_LENGTH);
    if (!len || len > MAX_BUFFER_LENGTH)
        throw std::invalid_argument("String.cat: Cstring argument is not null-terminated or is nullptr.");

    size_t newSize = this->size + len;
    char* buffer = new char[newSize];

    strcpy(buffer, this->c_str());
    strcat(buffer, str);

    try
    {
        copy(buffer);
    }
    catch (...)
    {
        delete[] buffer;
        throw;
    }

    delete[] buffer;
}

void String::cat (const String& other)
{
    cat(other.c_str());
}

void String::read (std::istream& in)
{
    char* buffer = new char[MAX_BUFFER_LENGTH];
    
    in.getline(buffer, MAX_BUFFER_LENGTH);
    if (in.good() == false)
    {
        if (in.eof())
            return;

        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        in.clear();

        delete[] buffer;

        throw std::length_error("String.read: Exceeded maximum buffer length.");
    }

    try
    {
        copy(buffer);
    }
    catch (...)
    {
        delete[] buffer;
        throw;
    }

    delete[] buffer;
}

void String::serialize (std::fstream& file) const
{
    if (size == 0 || arr == nullptr)
        throw std::invalid_argument("String.serialize: Cannot write empty cstring.");
    
    file.write(reinterpret_cast<const char*>(&size), sizeof(size));
    file.write(arr, size);
}

void String::deserialize (std::fstream& file)
{
    if (!file)
        std::runtime_error("String.deserialize: Input file is not in good state");

    if (arr)
        clear();

    file.read(reinterpret_cast<char*>(&size), sizeof(size));

    if (file)
    {
        try
        {
            arr = new char[size];
            file.read(arr, size);
        }
        catch (std::bad_alloc&)
        {
            size = 0;
            throw std::runtime_error("String.deserialize: Could not allocate memory (possible file corruption).");
        }
    }

    if (!file)
    {
        clear();
        throw std::runtime_error("String.deserialize: Reading from file failed.");
    }
}

void String::copy (const char* str)
{
    size_t len = string_utils::strlen(str, MAX_BUFFER_LENGTH);
    if (!len || len > MAX_BUFFER_LENGTH)
        throw std::invalid_argument("String.copy: Cstring argument is not null-terminated or is nullptr.");

    char* buffer = new char[len + 1];
    strcpy(buffer, str);
    
    if (arr)
        clear();

    arr = buffer;
    size = len + 1;
}

void String::copy (const String& other)
{
    copy(other.c_str());
}

void String::clear () noexcept
{
    delete[] arr;
    size = 0;
}

// ---- PRIVATE ----

void String::move (String&& other) noexcept
{
    size = other.size;
    other.size = 0;

    arr = other.arr;
    other.arr = nullptr;
}