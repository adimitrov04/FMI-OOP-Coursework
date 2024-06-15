#pragma once

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <iostream>
#include <cstdint>
#include <utility>
#include <stdexcept>

#include "Sort.h"

template <typename T>
class Vector
{

public:
    Vector() noexcept;
    Vector(const int64_t starting_capacity);
    Vector(Vector<T> &&other) noexcept;
    Vector(const Vector<T> &other);

    ~Vector() noexcept;

public:
    Vector<T> &operator=(const Vector& other);
    Vector<T> &operator=(Vector&& other) noexcept;
    
    T& operator[] (const int64_t index);
    const T& operator[] (const int64_t index) const;

public:
    const int64_t size () const noexcept;
    const int64_t capacity () const noexcept;
    T& at (const int64_t index);
    const T& at (const int64_t index) const;
    int64_t binary_search (const T& search_arg) const noexcept;
    bool is_sorted () const noexcept;
    
    void push_back (const T& element);
    void pop_back ();
    void reserve (const int64_t capacity);
    void append (const Vector<T> &other);

    template <typename SortAlgorithm = void(T*, uint64_t)>
    void sort (const SortAlgorithm& sorter = sort::quicksort<T>);

    void clear () noexcept;

private:
    void copy (const Vector<T> &other);
    void move (Vector<T> &&other) noexcept;

private:
    static const int64_t DEFAULT_STARTING_CAPACITY = 4;
    
    int64_t f_capacity;
    int64_t f_size;

    T* arr;

};

#include "../../src/Utils/Vector.ipp"

#endif // __VECTOR_H__