#pragma once

#ifndef __USER_H__
#define __USER_H__

#include <ostream>
#include <cstdint>

#include "../Utils/String.h"
#include "../Utils/Vector.h"
#include "../SNWKFormat/snwk_format.h"

#include "Interfaces/AppElement.h"
#include "Interfaces/ISerializeable.h"

class User : public AppElement
{

public:
    User();
    User (const uint64_t searchID);
    User (const String& searchName);
    User(const String& setName, const String& setPass, const uint64_t setID, const int32_t setScore);
    ~User() = default;

public:
    User& operator= (const User& other);
    bool operator== (const User& other) const noexcept;

    bool operator< (const User& other) const noexcept;
    bool operator<= (const User& other) const noexcept;
    bool operator> (const User& other) const noexcept;
    bool operator>= (const User& other) const noexcept;

public:
    const String& GetName () const noexcept;
    uint64_t GetID () const noexcept;
    const String& GetPass () const noexcept;
    int32_t GetScore () const noexcept;

    void SetName (const String& newName);
    void SetPass (const String& newPass);
    void AddScore (const int8_t value) noexcept;
    void MakeAdmin (User& other) noexcept;

    virtual void Serialize (std::fstream& file) const override;
    virtual void Deserialize (std::fstream& file) override;

    using AppElement::DeleteObject;
    virtual AppElement* clone () const override;
    User GetDeletedVersion () const;

    // For testing, remove later
    void printinfo (std::ostream& out) const;

public:
    static const snwk::FourCC TYPE_FCC;

private:
    User(const User& other);
    void SetID (uint64_t setID);
    void SetScore (int32_t setScore);

private:
    uint64_t user_id;
    String username;
    String password;

    int32_t user_score;
    bool is_admin;

};

#endif // __USER_H__