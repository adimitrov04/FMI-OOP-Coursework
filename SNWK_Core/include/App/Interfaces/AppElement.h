#pragma once

#ifndef __APP_ELEMENT_BASE__
#define __APP_ELEMENT_BASE__

#include "ISerializeable.h"

/**
 * An abstraction to be implemented by all app elements in the network. Enforces
 * implementation of proper deletion and serialization behaviour.
 * 
 * @attention Every class implementing AppElement should have a static `DELETED`
 * object defined to work properly, since deleting an object simply sets
 * `is_deleted_object` to `true`.
 */
class AppElement : public ISerializeable
{

public:
    AppElement();
    virtual ~AppElement() = default;

public:
    virtual void DeleteObject ();
    bool IsDeleted () const;

protected:    
    virtual void Serialize (std::fstream& file) const override = 0;
    virtual void Deserialize (std::fstream& file) override = 0;
    
    using ISerializeable::check_file_state;

private:
    bool is_deleted_object;

};

#endif // __APP_ELEMENT_BASE__