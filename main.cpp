#include <algorithm>
#include <fstream>
#include <iostream>

#include "crc32.h"
#include "main.h"

#if defined(_WIN32)
#define mkdir2(path) _mkdir(path)
#else
#include <sys/stat.h>
#define mkdir2(path) mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO)
#endif

bool readHeader(std::ifstream &stream, RAWHeader& header)
{
    stream.read(reinterpret_cast<char*>(&header), sizeof(RAWHeader));

    if (std::string(header.magic, 12) != "asus package") {
        std::cerr << "Wrong magic!\n";
        return false;
    }

    return true;
}

bool readBlocks(std::ifstream &stream, const RAWHeader& header, std::vector<BlockHeader>& blocks)
{
    for (uint32_t i = 0; i < header.count; i++) {
        BlockHeader blockHeader;
        stream.read(reinterpret_cast<char*>(&blockHeader), sizeof(BlockHeader));
        blocks.push_back(blockHeader);
    }

    return true;
}

void doList(const std::string& input)
{
    std::ifstream stream(input, std::ios::in | std::ios::binary);
    
    RAWHeader header;
    if (!readHeader(stream, header)) {
        return;
    }

    std::vector<BlockHeader> blocks;
    if (!readBlocks(stream, header, blocks)) {
        return;
    }

    for (const auto& b: blocks) {
        std::string filename;
        for (uint8_t i = 0; i < 32; i += 2) {
            char c = b.partition[i];
            if (c == '\0') {
                break;
            }

            filename += c;
        }

        std::cout << filename << "\n";
    }

    stream.close();
}

void doUnpack(const std::string& input, const std::string& output)
{
    std::cout << "Unpacking " << input << " to " << output << "\n";

    std::ifstream stream(input, std::ios::in | std::ios::binary);
    
    RAWHeader header;
    if (!readHeader(stream, header)) {
        return;
    }

    std::vector<BlockHeader> blocks;
    if (!readBlocks(stream, header, blocks)) {
        return;
    }

    // Data starts at 1024 * 10
    stream.seekg(1024 * 10, stream.beg);

    mkdir2(output.c_str());

    for (const auto& b: blocks) {
        std::vector<char> data;
        data.resize(b.size);

        std::string filename;
        for (uint8_t i = 0; i < 32; i += 2) {
            char c = b.partition[i];
            if (c == '\0') {
                break;
            }

            if (c == ':') {
                c = '_';
            }

            filename += c;
        }

        filename += ".img";

        std::cout << filename << "\n";
        stream.read(data.data(), b.size);

        std::ofstream outstream(output + "/" + filename, std::ios::out | std::ios::binary);
        outstream.write(data.data(), data.size());
        outstream.close();
    }

    stream.close();

    std::cout << "Done\n";
}

void showUsage()
{
    std::cerr << "Usage: asustool\n"
        << "-l <input>\n"
        << "    List files in image <input>\n"
        << "-u <input> [output dir]\n"
        << "    Extracts image <input> to directory\n"
        << "    [output dir]. Defaults to <input>_out";
}

int main(int argc, char** argv) {
    if (argc < 3) {
        showUsage();
        return -1;
    }

    std::string option = std::string(argv[1]);
    if (option == "-l") {
        std::string input = std::string(argv[2]);
        doList(input);
    } else if (option == "-u") {
        std::string input = std::string(argv[2]);
        std::string output;
        if (argc == 4) {
            output = std::string(argv[3]);
        } else {
            output = input + "_out";
        }

        doUnpack(input, output);
    } else {
        showUsage();
    }

    return 0;
}
