#ifndef SMILE_ENUM_TYPES_HPP
#define SMILE_ENUM_TYPES_HPP

namespace smile
{

enum class auth_type
{
    PASSWORD
};

enum class password_encryption_type
{
    UNKNOWN,
    DES,
    SHA1
};

inline int operator+ (auth_type at)
{
    return static_cast<int>(at);
}

inline int operator+ (password_encryption_type pet)
{
    return static_cast<int>(pet);
}

}

#endif
