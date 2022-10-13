#include "Image.h"


Image::Image(std::string filename)
    : m_filename { filename }
{
    width = m_BMPImageObj.getWidth();
    height = m_BMPImageObj.getHeight();

    std::vector<std::vector<RGB>> RGBImageMatrix { m_BMPImageObj.getRGBImageMatrix() };

    for (const auto& rowData : RGBImageMatrix)
        m_pixelMatrix.push_back(rowData);
}

void Image::setPixelMatrix(std::vector<std::vector<RGB>> _pixelMatrix)
{
    m_pixelMatrix = _pixelMatrix;
    width = m_pixelMatrix[0].size();
    height = m_pixelMatrix.size();
}

void Image::transformPixels(std::function<void(RGB&)> transformationFunction)
{
    for (unsigned int j { }; j < width; ++j)
        for (unsigned int i { }; i < height; ++i)
            transformationFunction(m_pixelMatrix[i][j]);
}

void Image::saveAs(std::string filename)
{
    BMP newImage { m_filename };
    newImage.setRGBImageMatrix(m_pixelMatrix);
    newImage.saveToFile(filename);
}




