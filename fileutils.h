#ifndef THORQ_FILEUTILS_H
#define THORQ_FILEUTILS_H

#include "filetype.h"

#include <cstddef>
#include <cstdint>

namespace ThorQ::FileUtils {

constexpr bool IsPng(const std::uint8_t* data, std::size_t length) {
    return length > 4 &&
           data[0] == 0x89 &&
           data[1] == 'P'  &&
           data[2] == 'N'  &&
           data[3] == 'G';
}
constexpr bool IsJpg(const std::uint8_t* data, std::size_t length) {
    return length > 10 &&
           data[0] == 0xFF &&
           data[1] == 0xD8 &&
           data[2] == 0xFF &&
           data[6] == 'J'  &&
           data[7] == 'F'  &&
           data[8] == 'I'  &&
           data[9] == 'F';
}
constexpr bool IsGif89a(const std::uint8_t* data, std::size_t length) {
    return length > 7 &&
           data[0] == 'G'  &&
           data[1] == 'I'  &&
           data[2] == 'F'  &&
           data[3] == '8'  &&
           data[4] == '8'  &&
           data[5] == '9'  &&
           data[6] == 'a';
}
constexpr bool IsGif87a(const std::uint8_t* data, std::size_t length) {
    return length > 7 &&
           data[0] == 'G'  &&
           data[1] == 'I'  &&
           data[2] == 'F'  &&
           data[3] == '8'  &&
           data[4] == '8'  &&
           data[5] == '9'  &&
           data[6] == 'a';
}
constexpr bool IsGif(const std::uint8_t* data, std::size_t length) {
    return IsGif87a(data, length) || IsGif89a(data, length);
}

constexpr ThorQ::FileType GetImageTypeFromData(const char* data, std::size_t length)
{
    if (IsPng((const std::uint8_t*)data, length)) {
        return ThorQ::FileType::Png;
    }
    else if (IsJpg((const std::uint8_t*)data, length)) {
        return ThorQ::FileType::Jpg;
    }
    else if (IsGif((const std::uint8_t*)data, length)) {
        return ThorQ::FileType::Gif;
    }

    return ThorQ::FileType::Unknown;
}
constexpr const char* GetExtensionFromImageType(ThorQ::FileType type)
{
    switch (type) {
    case ThorQ::FileType::Png:
        return ".png";
        break;
    case ThorQ::FileType::Jpg:
        return ".jpg";
        break;
    case ThorQ::FileType::Gif:
        return ".gif";
        break;
    default:
        return ".unknown";
    }
}

}

#endif // THORQ_FILEUTILS_H
