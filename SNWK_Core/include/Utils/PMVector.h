#pragma once

#ifndef __PMVECTOR_H__
#define __PMVECTOR_H__

#include "Vector.h"

/**
 * Class template for a Polymorphic Vector.
 */
template <typename Base>
class PMVector
{

public:
    PMVector() = default;
    PMVector(const PMVector<Base> &other);

    ~PMVector();

public:
    Base* operator[] (const uint64_t index);
    const Base* operator[] (const uint64_t index) const;

public:
    const uint64_t size () const noexcept;
    const uint64_t capacity () const noexcept;
    Base* at (const uint64_t index);
    const Base* at (const uint64_t index) const;

    void push_back (const Base* element);
    void pop_back ();

    void copy (const PMVector<Base> &other);
    void clear () noexcept;

private:
    PMVector<Base> &operator= (const PMVector<Base> &other) = delete;

private:
    Vector<Base*> vec;

};

#include "../../src/Utils/PMVector.ipp"

#endif // __PMVECTOR_H__