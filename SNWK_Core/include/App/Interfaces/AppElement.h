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
    AppElement() : is_deleted_object(false) {}
    virtual ~AppElement() = default;

public:
    virtual void DeleteObject () = 0;
    bool IsDeleted () const { return is_deleted_object; }

protected:    
    virtual void Serialize (std::fstream& file) const override = 0;
    virtual void Deserialize (std::fstream& file) override = 0;
    
    using ISerializeable::check_file_state;

private:
    bool is_deleted_object;

};

void AppElement::Serialize (std::fstream& file) const
{
    uint8_t deletedByte = (is_deleted_object) ? 1 : 0;
    file.write(reinterpret_cast<const char*>(&deletedByte), sizeof(deletedByte));
    check_file_state(file);
}

void AppElement::Deserialize (std::fstream& file)
{
    uint8_t deletedByte = 0;
    file.read(reinterpret_cast<char*>(&deletedByte), sizeof(deletedByte));
    check_file_state(file);
    is_deleted_object = deletedByte ? true : false;
}

#endif // __APP_ELEMENT_BASE__