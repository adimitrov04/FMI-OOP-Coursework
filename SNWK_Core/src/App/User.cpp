#include <iostream>
#include <stdexcept>

#include "../../include/Utils/String.h"
#include "../../include/Utils/Vector.h"

#include "../../include/App/User.h"

// ---- CONSTANTS ----

const snwk::FourCC User::TYPE_FCC = {'u', 's', 'e', 'r'};

// ---- LIFECYCLE ----

User::User(const String& setName, const String& setPass, const uint32_t setID)
: user_id(setID)
, user_score(0)
, is_admin(false)
{   
    SetName(setName);
    SetPass(setPass);

    if (user_id == 1)
        is_admin = true;
}

// ---- OPERATORS ----

bool User::operator== (const User& other) const noexcept
{
    return username == other.GetName();
}

// ---- GETTERS ----

const String& User::GetName () const noexcept
{
    return username;
}

const uint32_t User::GetID () const noexcept
{
    return user_id;
}

const String& User::GetPass () const noexcept
{
    return password;
}

const int32_t User::GetScore () const noexcept
{
    return user_score;
}

// ---- GETTERS ----

void User::SetName (const String& newName)
{
    if (newName.length() == 0)
        throw std::invalid_argument("User.SetName: Name cannot be empty.");

    username = newName;
}

void User::SetPass (const String& newPass)
{
    if (newPass.length() == 0)
        throw std::invalid_argument("User.SetPass: Password cannot be empty.");

    password = newPass;
}

// ---- SERIALIZATION ----

void User::Serialize (std::fstream& file) const
{
    if (!file)
        throw std::runtime_error("User.serialize: File is not in good state.");
    
    file.write(reinterpret_cast<const char*>(&user_id), sizeof(user_id));
    if (!file)
        throw std::runtime_error("User.serialize: Writing failed at user_id.");

    uint8_t adminbyte = (is_admin) ? 1 : 0;
    file.write(reinterpret_cast<const char*>(&adminbyte), sizeof(adminbyte));
    if (!file)
        throw std::runtime_error("User.serialize: Writing failed at is_admin.");
    
    username.serialize(file);
    password.serialize(file);

    file.write(reinterpret_cast<const char*>(&user_score), sizeof(user_score));
    if (!file)
        throw std::runtime_error("User.serialize: Writing failed at user_score.");
}

void User::Deserialize (std::fstream& file)
{
    if (!file)
        throw std::runtime_error("User.serialize: File is not in good state.");
    
    file.read(reinterpret_cast<char*>(&user_id), sizeof(user_id));
    if (!file)
        throw std::runtime_error("User.serialize: Reading failed at user_id.");

    uint8_t adminbyte;
    file.read(reinterpret_cast<char*>(&adminbyte), sizeof(adminbyte));
    if (!file)
        throw std::runtime_error("User.serialize: Reading failed at is_admin.");

    is_admin = adminbyte ? true : false;
    
    username.deserialize(file);
    password.deserialize(file);

    file.read(reinterpret_cast<char*>(&user_score), sizeof(user_score));
    if (!file)
        throw std::runtime_error("User.serialize: Writing failed at user_score.");
}

void User::AddScore (const int32_t value) noexcept
{
    user_score += value;
}

void User::printinfo (std::ostream& out) const
{
    out << "User: " << username << '\n';
    out << "ID: " << user_id << '\n';
    out << "Password: " << password << '\n';
    out << "Score: " << user_score << '\n';
    out << "Admin: " << is_admin << '\n';
}