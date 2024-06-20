#include "../../include/App/Interfaces/AppElement.h"

AppElement::AppElement() : is_deleted_object(false) {}

// ---- PUBLIC ----

void AppElement::DeleteObject ()
{
    is_deleted_object = true;
}

bool AppElement::IsDeleted () const
{
    return is_deleted_object;
}

// ---- PROTECTED ----

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