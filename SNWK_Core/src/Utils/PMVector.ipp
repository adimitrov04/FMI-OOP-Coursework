#include "../../include/Utils/PMVector.h"

// ---- LIFECYCLE ----

template <typename Base>
PMVector<Base>::PMVector(const PMVector<Base> &other)
: PMVector<Base>()
{
    copy(other);
}

template <typename Base>
PMVector<Base>::~PMVector()
{
    clear();
}

// ---- OPERATORS ----

template <typename Base>
Base* PMVector<Base>::operator[] (const uint64_t index)
{
    return vec[index];
}

template <typename Base>
const Base* PMVector<Base>::operator[] (const uint64_t index) const
{
    return vec[index];
}

// ---- METHODS ----

template <typename Base>
const uint64_t PMVector<Base>::size () const noexcept
{
    return vec.size();
}

template <typename Base>
const uint64_t PMVector<Base>::capacity () const noexcept
{
    return vec.capacity();
}

template <typename Base>
Base* PMVector<Base>::at (const uint64_t index)
{
    return vec.at(index);
}

template <typename Base>
const Base* PMVector<Base>::at (const uint64_t index) const
{
    return vec.at(index);
}

template <typename Base>
void PMVector<Base>::push_back (const Base* element)
{
    vec.push_back(element->clone());
}

template <typename Base>
void PMVector<Base>::pop_back ()
{
    vec.pop_back();
}

template <typename Base>
void PMVector<Base>::copy (const PMVector<Base> &other)
{
    PMVector<Base> buffer;

    try
    {
        for (uint64_t i = 0; i < other.size(); i++)
            buffer.push_back(other[i]);
    }
    catch(...)
    {
        buffer.clear()
        throw;
    }

    this->clear();
    vec = std::move(buffer.vec);
}

template <typename Base>
void PMVector<Base>::clear () noexcept
{
    for (uint64_t i = 0; i < vec.size(); i++)
        delete vec[i];

    vec.clear();
}