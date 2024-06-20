#pragma once

#ifndef __USER_H__
#define __USER_H__

#include <ostream>
#include <cstdint>

#include "../Utils/String.h"
#include "../../include/SNWKFormat/snwk_format.h"

#include "Interfaces/AppElement.h"
#include "Interfaces/ISerializeable.h"

class User : public AppElement, public ISerializeable
{

public:
    User(const String& setName, const String& setPass, const uint32_t setID, const int32_t setScore);
    ~User() = default;

public:
    bool operator== (const User& other) const noexcept;

public:
    const String& GetName () const noexcept;
    const uint32_t GetID () const noexcept;
    const String& GetPass () const noexcept;
    const int32_t GetScore () const noexcept;

    void SetName (const String& newName);
    void SetPass (const String& newPass);
    void AddScore (const int8_t value) noexcept;

    virtual void Serialize (std::fstream& file) const override;
    virtual void Deserialize (std::fstream& file) override;

    virtual void DeleteObject () override;

    // For testing, remove later
    void printinfo (std::ostream& out) const;

public:
    static const snwk::FourCC TYPE_FCC;

private:
    uint32_t user_id;
    String username;
    String password;

    int32_t user_score;
    bool is_admin;

};

#endif // __USER_H__