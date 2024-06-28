#pragma once

#ifndef __ICLONEABLE_H__
#define __ICLONEABLE_H__

template <class BaseName>
/**
 * Interface for base classes which support polymorphic cloning
 */
class ICLoneable
{

public:
    virtual ~ICLoneable() = default;
    virtual BaseName* clone () const = 0;

};

#endif // __ICLONEABLE_H__