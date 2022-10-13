#ifndef IMAGE_H
#define IMAGE_H

#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <functional>

#include <RGB.h>
#include <BMP.h>

class Image
{
public:
    Image(const unsigned int& _width, const unsigned int& _height, const RGB& solidCol);
    Image(std::string filename);

    void transformPixels(std::function<void(RGB&)> transformationFunction);
    void saveAs(std::string filename);
    void setPixelMatrix(std::vector<std::vector<RGB>> _pixelMatrix);
    std::vector<std::vector<RGB>> getPixelMatrix() { return m_pixelMatrix; }


    unsigned int width {};
    unsigned int height {};

private:
    std::string m_filename {};
    std::vector<std::vector<RGB>> m_pixelMatrix {};
    BMP m_BMPImageObj { m_filename };
};

#endif // IMAGE_H
