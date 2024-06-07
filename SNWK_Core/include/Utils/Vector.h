#pragma once

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <iostream>
#include <utility>
#include <stdexcept>

template <typename T>
class Vector
{

public:
    Vector() noexcept;
    Vector(const size_t starting_capacity);
    Vector(Vector<T> &&other) noexcept;
    Vector(const Vector<T> &other);

    ~Vector() noexcept;

public:
    Vector<T> &operator=(const Vector& other);
    Vector<T> &operator=(Vector&& other) noexcept;
    
    T& operator[] (const size_t index);
    const T& operator[] (const size_t index) const;

public:
    const size_t size () const noexcept;
    const size_t capacity () const noexcept;
    T& at (const size_t index);
    const T& at (const size_t index) const;
    
    void push_back (const T& element);
    void pop_back ();
    void reserve (const size_t capacity);
    void append (const Vector<T> &other);

    void clear () noexcept;

private:
    void copy (const Vector<T> &other);
    void move (Vector<T> &&other) noexcept;

private:
    static const size_t DEFAULT_STARTING_CAPACITY = 4;
    
    // RESOLVE POTENTIAL SERIALIZATION ISSUES
    size_t f_capacity;
    size_t f_size;

    T* arr;

};

#include "../../src/Utils/Vector.ipp"

#endif // __VECTOR_H__