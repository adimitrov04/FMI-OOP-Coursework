#pragma once

#ifndef __APP_ELEMENT_BASE__
#define __APP_ELEMENT_BASE__

class AppElement
{

public:
    AppElement() : is_deleted_object(false) {}
    virtual ~AppElement() = default;

public:
    virtual void DeleteObject () = 0;
    bool IsDeleted () const { return is_deleted_object; }

private:
    bool is_deleted_object;

};

#endif // __APP_ELEMENT_BASE__