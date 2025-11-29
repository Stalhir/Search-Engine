#pragma once
#include <array>

namespace spec_symbols
{
    constexpr  std::array<char, 34> special_chars =
    {
        ',', '"', '\'', ':', ';', '(', ')', '[', ']', '{', '}',
        '/', '\\', '|', '.', '!', '?', '@', '#', '$', '%', '^',
        '&', '*', '-', '_', '+', '=', '<', '>', '~', '`', '\t',
        '\n'
    };
}