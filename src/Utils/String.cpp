#include <iostream>
#include <cstddef>
#include <stdexcept>
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

String::String()
: size(0),
  arr(nullptr)
{}

String::String (const char* str)
: String()
{
    size_t len = strnlen(str, MAX_BUFFER_LENGTH);
    if (!len || len == MAX_BUFFER_LENGTH)
        throw std::invalid_argument("String.ctor: Cstring argument is not null-terminated or is nullptr.");

    size = len + 1;
    arr = new char[size];

    strcpy(arr, str);
}

String::String (const String& other)
: String(other.c_str())
{}

String::String (String&& other)
: String()
{
    move(std::move(other));
}

String::~String()
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

String& String::operator= (String&& other)
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

String& String::operator+ (const char* str)
{
    this->cat(str);

    return *this;
}

const bool String::operator== (const String& other) const
{
    return strcmp(this->c_str(), other.c_str()) == 0;
}

const bool String::operator== (const char* str) const
{
    return strcmp(this->c_str(), str) == 0;
}

std::ostream& operator<< (std::ostream& out, const String& str)
{
    out << str.c_str();

    return out;
}

std::istream& operator>> (std::istream& in, String& str)
{
    str.read(in);

    return in;
}

// ---- CASTS ----

String::operator char*() const
{
    char* output = new char[this->size];
    strcpy(output, this->c_str());

    return output;
}

// ---- GETTERS ----

const size_t String::get_length () const
{
    return strlen(arr);
}

const char* String::c_str () const
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


void String::print (std::ostream& out) const
{
    if (arr == nullptr)
        out << "null";

    out << arr;
}

// ---- SETTERS ----

void String::cat (const char* str)
{
    size_t len = strnlen(str, MAX_BUFFER_LENGTH);
    if (!len || len == MAX_BUFFER_LENGTH)
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

void String::copy (const char* str)
{
    size_t len = strnlen(str, MAX_BUFFER_LENGTH);
    if (!len || len == MAX_BUFFER_LENGTH)
        throw std::invalid_argument("String.copy: Cstring argument is not null-terminated or is nullptr.");

    char* buffer = new char[len + 1];
    strcpy(buffer, str);
    
    if (arr)
        clear();

    arr = buffer;
    size = strlen(str) + 1;
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

void String::move (String&& other)
{
    size = other.size;

    arr = other.arr;
    other.arr = nullptr;
}