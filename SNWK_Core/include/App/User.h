#pragma once

#ifndef __USER_H__
#define __USER_H__

#include <cstdint>

#include "../Utils/String.h"

#include "../SNWKFormat/ISerializeable.h"

class User : public ISerializeable
{

public:
    User();
    ~User() = default;

public:
    const bool operator== (const User& other) const;

public:
    const String& GetName () const;
    const uint32_t GetID () const;
    const String& GetPass () const;
    const int32_t GetScore () const;

    void SetName (const String& newName);
    void SetPass (const String& newPass);

    virtual void Serialize (std::fstream& file) const override;
    virtual void Deserialize (std::fstream& file) override;

private:
    void addScore (const int32_t value);

private:
    String username;
    String password;
    const uint32_t user_id;

    int32_t user_score;
    bool is_admin;

};

#endif // __USER_H__