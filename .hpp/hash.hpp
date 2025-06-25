
#pragma once

#include <cstddef>
#include <cstdint>

typedef uint64_t hash_t;

class hash
{
private:
    constexpr hash_t _FNV1a(const char* key) const
    {
        hash_t v = 0xcbf29ce484222325; // FNV-1a offset basis
        hash_t prime = 0x100000001b3; // FNV-1a prime

        for (size_t i = 0; key[i] != '\0'; ++i)
        {
            hash_t byte = static_cast<hash_t>(key[i]);
            v ^= byte;
            v *= prime;
        }

        return v;
    }

public:
    const hash_t value;

    constexpr hash() : value(0) {};
    constexpr hash(hash_t v) : value(v) {}

    constexpr hash(const char* key) : value(_FNV1a(key)) {}

    constexpr bool operator==(const hash& other) const { return value == other.value; }
    constexpr bool operator!=(const hash& other) const { return !(*this == other); }
};

#include <functional>

// Why I had to name the class hash? I dont know, but it is very annoying to specialize the struct hash to it.

// Uses this hashing system instead of std::hash
template <>
struct std::hash<::hash>
{
    constexpr std::size_t operator()(const ::hash& h) const noexcept {
        return h.value;
    }
};