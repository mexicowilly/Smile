#include "credential_store.hpp"

#include <openssl/rand.h>

namespace
{

constexpr const std::size_t ADDER_SIZE = 9;
constexpr const std::size_t MASK_SIZE = 7;
constexpr const std::size_t ADDER_MASK_SIZE = ADDER_SIZE + MASK_SIZE;

}

namespace smile
{

credential_store::credential_store(const std::string& pass)
    : encoded_(pass.length() + ADDER_MASK_SIZE)
{
    RAND_bytes(&encoded_[0], ADDER_MASK_SIZE);
    for (auto i = 0; i < pass.length(); i++)
        encoded_[i + ADDER_MASK_SIZE] = static_cast<unsigned char>(pass[i]) + encoded_[i % ADDER_SIZE];
    for (auto i = 0; i < pass.length(); i++)
        encoded_[i + ADDER_MASK_SIZE] ^= encoded_[(i % MASK_SIZE) + ADDER_SIZE];
}

std::string credential_store::get()
{
    std::string result(encoded_.size() - ADDER_MASK_SIZE, 0);
    for (auto i = 0; i < result.length(); i++)
        result[i] = encoded_[(i % MASK_SIZE) + ADDER_SIZE] ^ encoded_[i + ADDER_MASK_SIZE];
    for (auto i = 0; i < result.length(); i++)
        result[i] -= encoded_[i % ADDER_SIZE];
    return result;
}

}