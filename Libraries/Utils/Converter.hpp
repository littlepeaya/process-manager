/// Created by TrungTQ on 2016-08-05 10:15:00

#ifndef LIBX_UTILS_CONVERTER_HPP_
#define LIBX_UTILS_CONVERTER_HPP_

#include <algorithm>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/String.hpp"
#include "Libraries/Values/ValueArray.hpp"

namespace libX {
namespace utils {

class Converter {
public:
    template<typename T>
    T* ptr(T& obj);

    template<typename T>
    T* ptr(T* obj);

    template<typename T>
    static String GetHex(T data, size_t hex_len = sizeof(T) << 1);

    static String PrintfHex(u8_p pbuffer, u32_t length);

    static u8_t ToByte(bool value);

    static u16_t Crc16(u8_p pdata, u32_t length);

    static u16_t Crc16(vu8_t data);

    static u16_t Crc16(lstu8_t data);

    static lstu8_t FromHexString(String hex);

    /// @fn     Crc32GenTab
    /// @brief  None
    /// @param  None
    /// @return LSTU8
    static lstu32_t Crc32GenTab();

    /// @fn     ReverseBitsInBuf
    /// @brief  None
    /// @param  buf
    /// @param  offset
    /// @param  len
    /// @return None
    static void ReverseBitsInBuf(lstu8_t& buf, u32_t offset, u32_t len);

    /// @fn     lexical_cast
    /// @brief  Link https://stackoverflow.com/questions/1070497/c-convert-hex-string-to-signed-integer
    /// @tparam T2
    /// @tparam T1
    /// @param  in
    /// @return T2
    template<typename T2, typename T1>
    static T2 lexical_cast(const T1& in) {
        T2 out;
        std::stringstream ss;
        ss << in;
        ss >> out;
        return out;
    };
};

/// @func   GetHex
/// @brief  Convert Data to Hex format String
/// @param  None
/// @retval None
template<typename T>
inline String
Converter::GetHex(
    T data,
    size_t hex_len
) {
    static const_char_p digits = "0123456789ABCDEF";
    String rc(hex_len, '0');
    for (size_t i = 0, j = (hex_len - 1) * 4; i < hex_len; ++i, j -= 4) {
        rc[i] = digits[(data >> j) & 0x0F];
    }
    return rc;
}

/// @func   Convert
/// @brief  None
/// @param  None
/// @retval None
inline String
Converter::PrintfHex(
    u8_p  pbuffer,
    u32_t length
){
    bool_t boLastData = FALSE;
    String strRet;
    if (length == 1) {
        strRet.append(Converter::GetHex(pbuffer[0]));
    } else {
        for (u32_t i = 0; i < length; i++) {
            strRet.append(Converter::GetHex(pbuffer[i]));
            if (!boLastData) { strRet.append(" "); }
            if (i == length - 2) { boLastData = TRUE; }
        }
    }
    return strRet;
}

/// @func   ptr
/// @brief  turn reference into pointer
/// @param  None
/// @retval None
template<typename T>
inline T*
Converter::ptr(
    T& obj
) {
    return &obj;
}

/// @func   ptr
/// @brief  obj is already pointer, return it
/// @param  None
/// @retval None
template<typename T>
inline T*
Converter::ptr(
    T* obj
) {
    return obj;
}

/// @fn     ToByte
/// @brief  None
/// @param  BOOL value
/// @return BYTE
inline u8_t
Converter::ToByte(
    bool value
) {
    if (value) return 0x01;
    return 0x00;
}


/// @func   CRC16
/// @brief  None
/// @param  None
/// @retval None
inline u16_t
Converter::Crc16(
    u8_p  pdata,
    u32_t length
) {
    u8_p pbyTemp = pdata;
    u8_t byX;
    u16_t wCrc16  = 0x1D0F;
    u32_t dwlen = length;
    while (dwlen--) {
        byX = (wCrc16 >> 8) ^ *pbyTemp++;
        byX ^= byX >> 4;
        wCrc16 = (wCrc16 << 8) ^ ((u16_t)(byX << 12)) ^ ((u16_t)(byX << 5)) ^ ((u16_t)byX);
    }
    return wCrc16;
}

inline u16_t
Converter::Crc16(
    vu8_t data
) {
    const u16_t POLY = 0x1021;
    u16_t wCrc16  = 0x1D0F;
    for (auto & it : data) {
        for (u8_t i = 0x80; i != 0; i >>= 1) {
            u8_t newBit = ToByte((it & i) != 0) ^ ToByte((wCrc16 & 0x8000) != 0);
            wCrc16 <<= 1;
            if (newBit != 0) {
                wCrc16 ^= POLY;
            }
        }
    }
    return wCrc16;
}

inline u16_t
Converter::Crc16(
    lstu8_t data
) {
    const u16_t POLY = 0x1021;
    u16_t wCrc16  = 0x1D0F;
    for (u8_t i = 0; i < data.Size(); i++) {
        for (u8_t j = 0x80; j != 0; j >>= 1) {
            u8_t newBit = ToByte((data[i] & j) != 0) ^ ToByte((wCrc16 & 0x8000) != 0);
            wCrc16 <<= 1;
            if (newBit != 0) {
                wCrc16 ^= POLY;
            }
        }
    }
    return wCrc16;
}

inline lstu8_t
Converter::FromHexString(
    String hex
) {
    Vector<u8_t> buff;
    u32_t i = 0;
    if (hex.length() % 2 != 0) {
        i = 1;
        #ifndef MT7688
        buff.push_back((u8_t) std::stoi(hex.substr(0, 1).c_str(), 0, 16));
        #else  /* MT7688 */
        buff.push_back(lexical_cast<u8_t>(hex.substr(0, 1)));
        #endif /* MT7688 */
    }
    for (; i < hex.length(); i += 2) {
        #ifndef MT7688
        buff.push_back((u8_t) std::stoi(hex.substr(i, 2).c_str(), 0, 16));
        #else  /* MT7688 */
        buff.push_back(lexical_cast<u8_t>(hex.substr(i, 2)));
        #endif /* MT7688 */
    }
    return lstu8_t(buff);
}

/// @fn     Crc32GenTab
/// @brief  None
/// @param  None
/// @return LSTU8
inline lstu32_t
Converter::Crc32GenTab(
) {
    lstu32_t crcTab(256);
    u32_t poly = 0xEDB88320;
    u32_t crc;
    for (u16_t i = 0; i < 256; i++) {
        crc = (u32_t) i;
        for (u8_t j = 8; j > 0; j--) {
            if ((crc & 1) != 0)
                crc = (crc >> 1) ^ poly;
            else
                crc >>= 1;
        }
        crcTab[i] = crc;
    }
    return crcTab;
}

/// @fn     ReverseBitsInBuf
/// @brief  None
/// @param  buf
/// @param  offset
/// @param  len
/// @return None
inline void
Converter::ReverseBitsInBuf(
    lstu8_t& buf,
    u32_t offset,
    u32_t len
) {
    for (u32_t i = offset; i < (len + offset); i++) {
        buf[i] = ((buf[i] << 4) & 0xF0) | ((buf[i] >> 4) & 0x0F);
        buf[i] = ((buf[i] << 2) & 0xCC) | ((buf[i] >> 2) & 0x33);
        buf[i] = ((buf[i] << 1) & 0xAA) | ((buf[i] >> 1) & 0x55);
    }
}

}
}

#endif /* !CONVERTER_HPP_ */
