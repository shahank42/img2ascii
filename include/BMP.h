#ifndef BMP_H
#define BMP_H

#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <cstdint>

#include <RGB.h>

const int BMP_MAGIC_ID { 2 };


struct BMPFileMagic
{
    unsigned char magic[BMP_MAGIC_ID];
};

struct BMPFileHeader
{
    uint32_t fileSize { 0 };
    uint16_t reserved1 { 0 };
    uint16_t reserved2 { 0 };
    uint32_t offsetData { 0 };
};

struct BMPInfoHeader
{
    uint32_t headerSize { 0 };
    int32_t width { 0 };
    int32_t height { 0 };
    uint16_t planes { 1 };
    uint16_t bitsPerPixel { 0 };
    uint32_t compression { 0 };
    uint32_t sizeImage { 0 };
    int32_t horizontalResolution { 0 };
    int32_t verticalResolution { 0 };
    uint32_t colorCount { 0 };
    uint32_t importantColorCount { 0 };
};


class BMP
{
public:
    BMP(std::string filename);

    std::vector<std::vector<RGB>> getRGBImageMatrix() { return m_RGBImageMatrix; }
    int getWidth() { return m_width; }
    int getHeight() { return m_height; }

    void saveToFile(std::string filename);
    void setRGBImageMatrix(std::vector<std::vector<RGB>>& _RGBImageMatrx) { m_RGBImageMatrix = _RGBImageMatrx; }

private:
    int m_width { 0 };
    int m_height { 0 };
    std::vector<std::vector<RGB>> m_RGBImageMatrix {};
};

#endif // BMP_H
