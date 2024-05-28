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
    Vector();
    Vector(const size_t starting_capacity);
    Vector(Vector<T> &&other);
    Vector(const Vector<T> &other);

    ~Vector();

public:
    Vector<T> &operator=(const Vector& other);
    Vector<T> &operator=(Vector&& other);
    
    T& operator[] (const size_t index);
    const T& operator[] (const size_t index) const;

    /* TODO: implement
    friend std::ostream& operator<< (std::ostream& out, const Vector<T> &vec);
    friend std::istream& operator>> (std::istream& in, Vector<T> &vec);
    */

public:
    const size_t size () const;
    const size_t capacity () const;
    T& at (const size_t index);
    const T& at (const size_t index) const;
    void print (std::ostream& out = std::cout) const;
    
    void push_back (const T& element);
    void pop_back ();
    void reserve (const size_t capacity);
    void append (const Vector<T> &other);

    void clear () noexcept;

private:
    void copy (const Vector<T> &other);
    void move (Vector<T> &&other);

private:
    static const size_t DEFAULT_STARTING_CAPACITY = 4;
    
    size_t f_capacity;
    size_t f_size;

    T* arr;

};

#include "../../src/Utils/Vector.ipp"

#endif // __VECTOR_H__