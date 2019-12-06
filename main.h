#pragma once

struct RAWHeader {
    char magic[12];
    uint32_t unk1;
    uint32_t emp1, emp2;
    uint32_t count;
    uint32_t emp3, emp4, emp5, emp6, emp7;
};

struct BlockHeader {
    char partition[32];
    char filename[32];
    uint32_t size;
    uint32_t emp1, emp2, emp3;
    uint32_t unk1;
    uint32_t emp4;
    uint32_t crc;
    uint32_t emp5;
};
