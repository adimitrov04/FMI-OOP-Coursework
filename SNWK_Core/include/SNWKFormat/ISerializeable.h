#pragma once

#ifndef __ISERIALIZEABLE_INTERFACE__
#define __ISERIALIZEABLE_INTERFACE__

#include <fstream>
#include <stdexcept>

struct ISerializeable
{
    virtual void Serialize (std::fstream& out_file) const = 0;
    virtual void Deserialize (std::fstream& in_file) = 0;
    
    ISerializeable() = default;
    virtual ~ISerializeable() = default;

protected:
    /**
     * Check current file state before continuing serialization in order to determine whether
     * previous operation was successful.
     * 
     * @throw std::range_error if reading operation failed (failbit was set)
     * @throw std::runtime_error if writing operation failed (badbit was set)
     */
    void check_file_state (std::fstream& file) const
    {
        if (file.good())
            return;
        
        if (file.fail())
            throw std::range_error("FATAL: File ended before object reading was complete. Data likely corrupted.");

        if (file.bad())
            throw std::runtime_error("Writing to file failed, record is not signed");
    }
};

#endif // __ISERIALIZEABLE_INTERFACE__