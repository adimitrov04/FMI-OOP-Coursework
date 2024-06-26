#include <iostream>
#include <cstdint>
#include <stdexcept>
#include <limits>

#include <cmath>
#include <fstream>
#include <utility>
#include <cstring>

#include "../../include/Utils/String.h"

// -- UTILITIES --

using namespace string_utils;

const bool string_utils::isnewline (const char ch) noexcept
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
const uint64_t string_utils::strlen (const char* str, const uint64_t LEN_LIMIT)
{
    if (!str)
        return 0;

    uint64_t len(0);
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
 * @return The numerical difference between the first different character encountered in str2 relative to str1
 */
const int string_utils::strcmp (const char* str1, const char* str2)
{
    if (!str1 || !str2)
        throw std::invalid_argument("strcmp: Cannot compare nullptrs.");

    if (strlen(str1) == String::MAX_BUFFER_LENGTH || strlen(str2) == String::MAX_BUFFER_LENGTH)
        throw std::invalid_argument("strcmp: Argument cstring(s) not null-terminated.");

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
        throw std::invalid_argument("strcasecmp: Argument cstring(s) not null-terminated.");

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

/**
 * Finds the location of an instance of a substring in a bigger string.
 * 
 * @throws `std::invalid_argument` if either `bigStr` or `smallStr` is not a valid, null-terminated cstring
 * 
 * @return A pointer within `bigStr`, locating the start of the encountered instance of `smallStr`
 * @return `nullptr` if `smallStr` is not in `bigStr`
 */
const char* string_utils::strstr (const char* bigStr, const char* smallStr)
{
    if (bigStr == nullptr || smallStr == nullptr)
        throw std::invalid_argument("strstr: Argument cannot be nullptr.");

    if (string_utils::strlen(bigStr) == String::MAX_BUFFER_LENGTH || string_utils::strlen(smallStr) == String::MAX_BUFFER_LENGTH)
        throw std::invalid_argument("strcasecmp: Argument cstring(s) not null-terminated.");
    
    while (*bigStr)
    {
        if (string_utils::strcmp(bigStr, smallStr) == 0)
            return bigStr;

        bigStr++;
    }

    return nullptr;
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

const uint64_t string_utils::getCurrentWordLength (const char* word)
{
    uint64_t wordSize = 0;

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
        throw std::out_of_range("String.getCurrentWordInString: No words left to extract");

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

/**
 * Concatenate a single character to the end of a cstring.
 * 
 * @param dest The original cstring
 * 
 * @warning `dest` MUST be null-terminated. If it is not, function invokes undefined behaviour.
 * @warning `dest` MUST have enough space in it to fit the new characred.
 */
void string_utils::add_char (char* dest, const char ch)
{
    char* put = dest + (string_utils::strlen(dest));
    
    *put = ch;
    put++;
    *put = '\0';
}

/**
 * Reverses a null-terminated c-string
 * 
 * @warning `str` MUST be null-terminated of undefined behaviour is invoked
 * @warning If `str` is nullptr, undefined behaviour is invoked
 */
void string_utils::reverse_cstr (char* str)
{
    char* left = str;
    char* right = (str + string_utils::strlen(str)) - 1;
    
    while (left < right)
    {
        std::swap(*left, *right);
        
        left++;
        right--;
    }
}

// ---- LIFECYCLE ----

String::String() noexcept
: size(0)
, arr(nullptr)
{}

String::String (const char* str)
: String()
{
    uint64_t len = string_utils::strlen(str, MAX_BUFFER_LENGTH);
    if (!str || len == MAX_BUFFER_LENGTH + 1)
        throw std::invalid_argument("String.cstr_ctor: Cstring argument is not null-terminated or is nullptr.");

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

bool String::operator== (const char* str) const
{
    return (string_utils::strcmp(this->c_str(), str) == 0);
}

bool String::operator== (const String& other) const
{
    return *this == other.c_str();
}

bool String::operator!= (const char* str) const
{
    return !(*this == str);
}

bool String::operator!= (const String& other) const
{
    return *this != other.c_str();
}

bool String::operator> (const char* other) const
{
    return (string_utils::strcasecmp(this->c_str(), other) > 0);
}

bool String::operator> (const String& other) const
{
    return *this > other.c_str();
}

bool String::operator< (const char* other) const
{
    return (string_utils::strcasecmp(this->c_str(), other) < 0);
}

bool String::operator< (const String& other) const
{
    return *this < other.c_str();
}

bool String::operator>= (const char* other) const
{
    return (*this > other) || (*this == other);
}

bool String::operator<= (const char* other) const
{
    return (*this < other) || (*this == other);
}

bool String::operator>= (const String& other) const
{
    return (*this > other) || (*this == other);
}

bool String::operator<= (const String& other) const
{
    return (*this < other) || (*this == other);
}

String::operator bool() const noexcept
{
    if (arr == nullptr || length() == 0)
        return false;

    return true;
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

const uint64_t String::length () const noexcept
{
    return string_utils::strlen(arr);
}

const char* String::c_str () const noexcept
{
    return arr;
}

char String::last () const
{
    if (arr == nullptr || size == 0)
        throw std::invalid_argument("String.last: Current String is empty.");
    
    return arr[size - 1];
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
    uint64_t len = string_utils::strlen(str, MAX_BUFFER_LENGTH);
    if (!str || len > MAX_BUFFER_LENGTH)
        throw std::invalid_argument("String.cat: Cstring argument is not null-terminated or is nullptr.");

    uint64_t newSize = this->size + len;
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
        buffer = nullptr;
        throw;
    }

    delete[] buffer;
    buffer = nullptr;
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
        buffer = nullptr;

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
    uint64_t len = string_utils::strlen(str, MAX_BUFFER_LENGTH);
    if (!str || len > MAX_BUFFER_LENGTH)
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
    arr = nullptr;
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

// ---- EXTERNAL ----

String string::parseToString (const uint32_t num)
{
    const char ZERO_CHAR = '0';
    
    // Get the number of digits; if num is zero log10 breaks, so in that case just make countDigits 1 manually
    int countDigits = (num > 0) ? static_cast<int>(std::floorf(std::log10(num) + 1)) : 1;
    char* buffer = new char[countDigits + 1];

    for (int r_cut = 1, l_cut = 10; r_cut <= num || num < 10; r_cut *= 10)
    {
        if (num < 10)
        {
            string_utils::add_char(buffer, (ZERO_CHAR + num));
            break;
        }
        
        int currentDigit = (num / r_cut) % l_cut;
        string_utils::add_char(buffer, (ZERO_CHAR + currentDigit));
    }

    string_utils::reverse_cstr(buffer);
    
    String output(buffer);
    delete[] buffer;
    buffer = nullptr;

    return output;
}