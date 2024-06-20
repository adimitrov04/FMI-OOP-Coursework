#include <iostream>
#include <stdexcept>

#include "../../include/App/User.h"

// ---- CONSTANTS ----

const snwk::FourCC User::TYPE_FCC = {'u', 's', 'e', 'r'};

const User User::NULL_USER("/null", "0", 0, 0);

const User User::DELETED_USER("[deleted]", "0", 0, 0);

// ---- LIFECYCLE ----

User::User ()
: user_id(0)
, username()
, password()
, user_score(0)
, is_admin(false)
{}

User::User(const String& setName, const String& setPass, const uint32_t setID, const int32_t setScore)
: User()
{
    SetID(setID);
    SetName(setName);
    SetPass(setPass);
    SetScore(setScore);

    if (user_id == 1)
        is_admin = true;
}

// ---- OPERATORS ----

User& User::operator= (const User& other)
{
    if (this != &other)
    {
        user_id = other.user_id;
        username = other.username;
        password = other.password;
        user_score = other.user_score;
        is_admin = other.is_admin;

        if (other.IsDeleted())
            DeleteObject();
    }

    return *this;
}

bool User::operator== (const User& other) const noexcept
{
    return username == other.GetName();
}

// ---- GETTERS ----

const String& User::GetName () const noexcept
{
    return username;
}

uint32_t User::GetID () const noexcept
{
    return user_id;
}

const String& User::GetPass () const noexcept
{
    return password;
}

int32_t User::GetScore () const noexcept
{
    return user_score;
}

User User::GetDeletedVersion () const
{
    User deletedVer(DELETED_USER);
    deletedVer.SetID(user_id);

    return deletedVer;
}

// ---- SETTERS ----

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

void User::SetID (uint32_t setID)
{
    user_id = setID;
}

void User::SetScore (int32_t setScore)
{
    user_score = setScore;
}

void User::AddScore (const int8_t value) noexcept
{
    user_score += value;
}

// ---- SERIALIZATION ----

void User::Serialize (std::fstream& file) const
{
    if (!file)
        throw std::runtime_error("User.serialize: File is not in good state.");
    
    AppElement::Serialize(file);

    file.write(reinterpret_cast<const char*>(&user_id), sizeof(user_id));
    check_file_state(file);

    uint8_t adminbyte = (is_admin) ? 1 : 0;
    file.write(reinterpret_cast<const char*>(&adminbyte), sizeof(adminbyte));
    check_file_state(file);
    
    //std::clog << username << '\n';
    username.serialize(file);
    check_file_state(file);

    //std::clog << password << '\n';
    password.serialize(file);
    check_file_state(file);

    file.write(reinterpret_cast<const char*>(&user_score), sizeof(user_score));
    check_file_state(file);
}

void User::Deserialize (std::fstream& file)
{
    if (!file)
        throw std::runtime_error("User.serialize: File is not in good state.");
    
    AppElement::Deserialize(file);

    file.read(reinterpret_cast<char*>(&user_id), sizeof(user_id));
    check_file_state(file);

    uint8_t adminbyte;
    file.read(reinterpret_cast<char*>(&adminbyte), sizeof(adminbyte));
    check_file_state(file);
    is_admin = adminbyte ? true : false;

    username.deserialize(file);
    check_file_state(file);

    password.deserialize(file);
    check_file_state(file);

    file.read(reinterpret_cast<char*>(&user_score), sizeof(user_score));
    check_file_state(file);
}

void User::printinfo (std::ostream& out) const
{
    if (IsDeleted())
    {
        GetDeletedVersion().printinfo(out);
        return;
    }    

    out << "User: " << username << '\n';
    out << "ID: " << user_id << '\n';
    out << "Password: " << password << '\n';
    out << "Score: " << user_score << '\n';
    out << "Admin: " << is_admin << '\n';
}

// ---- PRIVATE ----


User::User(const User& other)
: User(other.GetName(), other.GetPass(), other.GetID(), other.GetScore())
{
    if (other.IsDeleted())
        DeleteObject();
}
