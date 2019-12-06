#include "crc32.h"

static uint32_t crc_table[0x100];

uint32_t crc32_for_byte(uint32_t r)
{
    for(int j = 0; j < 8; ++j) {
        r = (r & 1? 0: (uint32_t)0xEDB88320L) ^ r >> 1;
    }

    return r ^ (uint32_t)0xFF000000L;
}

uint32_t crc32(const std::vector<uint8_t>& data)
{
    if(!*crc_table) {
        for(size_t i = 0; i < 0x100; ++i) {
            crc_table[i] = crc32_for_byte(i);
        }
    }

    uint32_t crc = 0;
    for(size_t i = 0; i < data.size(); ++i) {
        crc = crc_table[(uint8_t) crc ^ data[i]] ^ crc >> 8;
    }

    return crc;
} 
