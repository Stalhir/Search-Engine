#pragma once
#include <array>

namespace SPECIAL_CHAR_STRING
{
    constexpr  std::array<char, 34> special_chars =
    {
        ',', '"', '\'', ':', ';', '(', ')', '[', ']', '{', '}',
        '/', '\\', '|', '.', '!', '?', '@', '#', '$', '%', '^',
        '&', '*', '-', '_', '+', '=', '<', '>', '~', '`', '\t',
        '\n'
    };


    constexpr  std::array<const char*, 28> badExtension = {
        ".pdf", ".txt", ".zip", ".gz", ".tar", ".rar",
        ".jpg", ".jpeg", ".png", ".gif", ".bmp", ".svg",
        ".mp3", ".mp4", ".avi", ".mov", ".wav",
        ".doc", ".docx", ".xls", ".xlsx", ".ppt", ".pptx",
        ".exe", ".dll", ".so", ".bin", ".msi"
    };
}