#pragma once

#include <array>
#include <optional>
#include <string>

namespace utils
{

template <size_t outSize>
std::optional<std::array<unsigned char, outSize>> hexToUchars(std::string_view hex)
{
    if (hex.size() != outSize * 2)
        return std::nullopt;

    auto hexValue = [](char c) -> int
    {
        if (c >= '0' && c <= '9')
            return c - '0';
        if (c >= 'a' && c <= 'f')
            return c - 'a' + 10;
        if (c >= 'A' && c <= 'F')
            return c - 'A' + 10;

        return -1;
    };

    std::array<unsigned char, outSize> result{};

    for (size_t i = 0; i < outSize; ++i)
    {
        const int hi = hexValue(hex[i * 2]);
        const int lo = hexValue(hex[i * 2 + 1]);

        if (hi < 0 || lo < 0)
            return std::nullopt;

        result[i] = static_cast<unsigned char>((hi << 4) | lo);
    }

    return result;
}

template <size_t outSize> std::string ucharsToHex(const std::array<unsigned char, outSize>& bytes)
{
    constexpr char hex[] = "0123456789abcdef";

    std::string result;
    result.resize(outSize * 2);

    for (size_t i = 0; i < outSize; ++i)
    {
        result[i * 2]     = hex[bytes[i] >> 4];
        result[i * 2 + 1] = hex[bytes[i] & 0x0F];
    }

    return result;
}

} // namespace utils
