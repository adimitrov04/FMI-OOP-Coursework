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
    Vector(const uint64_t starting_capacity);
    Vector(Vector<T> &&other) noexcept;
    Vector(const Vector<T> &other);

    ~Vector() noexcept;

public:
    Vector<T> &operator=(const Vector& other);
    Vector<T> &operator=(Vector&& other) noexcept;
    
    T& operator[] (const uint64_t index);
    const T& operator[] (const uint64_t index) const;

public:
    const uint64_t size () const noexcept;
    const uint64_t capacity () const noexcept;
    T& at (const uint64_t index);
    const T& at (const uint64_t index) const;
    T* binary_search (T& search_arg) const noexcept;
    bool is_sorted () const noexcept;
    
    void push_back (const T& element);
    void push_sorted (const T& element);
    void pop_back ();
    void reserve (const uint64_t capacity);
    void append (const Vector<T> &other);

    template <typename SortAlgorithm = void(T*, uint64_t)>
    void sort (const SortAlgorithm& sorter = sort::quicksort<T>);

    void clear () noexcept;

private:
    void copy (const Vector<T> &other);
    void move (Vector<T> &&other) noexcept;

private:
    static const uint64_t DEFAULT_STARTING_CAPACITY = 4;
    
    uint64_t f_capacity;
    uint64_t f_size;

    T* arr;

};

#include "../../src/Utils/Vector.ipp"

#endif // __VECTOR_H__