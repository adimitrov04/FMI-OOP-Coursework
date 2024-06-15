#pragma once

#ifndef __SORT_LIB__
#define __SORT_LIB__

#include <cstdint>
#include <iostream>

// ---- UTILITIES ----

template <typename T>
static T& max (T& obj1, T& obj2)
{
    return (obj1 > obj2) ? obj1 : obj2;
}

template <typename T>
static T& min (T& obj1, T& obj2)
{
    return (obj1 < obj2) ? obj1 : obj2;
}

template <typename T>
static void swap (T& obj1, T& obj2)
{
    T temp = obj1;
    obj1 = obj2;
    obj2 = temp;
}

template <typename T>
static void move (T* arr, uint64_t start, uint64_t dest)
{
    if (start == dest)
        return;

    T moveVal = arr[start];
    
    if (start < dest)
    {
        for (uint64_t i = start; i < dest; i++)
        {
            arr[i] = arr[i + 1];
        }
    }

    if (start > dest)
    {
        for (uint64_t i = start; i > dest; i--)
        {
            arr[i] = arr[i - 1];
        }
    }

    arr[dest] = moveVal;

}

// ---- ALGORITHMS ----

namespace sort
{

template <typename T>
void quicksort (T* arr, uint64_t size)
{
    if (size <= 1)
        return;

    // Pivot is chosen as median-of-three
    uint64_t pivot = 0;
    if (arr[0] <= arr[size - 1])
    {
        // arr[first] <= arr[mid]
        if ( max( arr[0], arr[size / 2] ) == arr[size / 2] )
        {
            // arr[first] <= arr[last] <= arr[mid]
            if ( max( arr[size - 1], arr[size / 2] ) == arr[size / 2] )
            {
                pivot = (size - 1);
            }
            // arr[first] <= arr[mid] <= arr[last]
            else
            {
                pivot = (size / 2);
            }
        }
    }
    else // arr[last] < arr[first]
    {
        // arr[mid] <= arr[last] < arr[first]
        if ( max( arr[size / 2], arr[size - 1] ) == arr[size - 1] )
        {
            pivot = (size - 1);
        }
        // arr[last] < arr[mid] <= arr[first]
        else if ( max( arr[size / 2], arr[0] ) == arr[0] )
        {
            pivot = (size / 2);
        }
    }

    swap(arr[pivot], arr[0]);
    pivot = 0;

    for (uint64_t i = pivot + 1; i < size; i++)
    {
        if (arr[i] < arr[pivot])
        {
            move(arr, i, pivot);
            pivot++;
        }
    }

    quicksort(arr, pivot);
    quicksort(arr + (pivot + 1), size - (pivot + 1));
}

template <typename T>
bool is_sorted (const T* arr, uint16_t size)
{
    for (uint64_t i = 1; i < size; i++)
    {
        if ( arr[i] < arr[i - 1] )
            return false;
    }

    return true;
}

}

#endif // __SORT_LIB__