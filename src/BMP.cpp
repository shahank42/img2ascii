#include "BMP.h"


BMP::BMP(std::string filename)
{
    std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary);

    if (file.fail())
    {
        std::cerr << "Unable to open \"" << filename << "\"\n";
    }
    else
    {
        // Read the BMP magic number
        BMPFileMagic magic {};
        file.read((char*) (&magic), sizeof(magic));

        if ((magic.magic[0] != 'B') || (magic.magic[1] != 'M'))
        {
            std::cerr << filename << " is not in proper BMP format.\n";
        }
        else
        {
            BMPFileHeader fileHeader {};
            file.read((char*) (&fileHeader), sizeof(fileHeader));

            BMPInfoHeader infoHeader {};
            file.read((char*) (&infoHeader), sizeof(infoHeader));

            bool toFlipImage { true };
            if (infoHeader.height < 0)
            {
                toFlipImage = false;
                infoHeader.height *= -1;
            }

            // Only supports 24-bit images
            if (infoHeader.bitsPerPixel != 24)
            {
                std::cerr << filename << " uses " << infoHeader.bitsPerPixel << " bpp.\nBitmap only supports 24bit.\n";
            }

            // Only supports compressed images
            if (infoHeader.compression != 0)
            {
                std::cerr << filename << " is compressed.\n";
            }



            m_width = infoHeader.width;
            m_height = infoHeader.height;

            for (int row { }; row < m_height; ++row)
            {
                std::vector<RGB> rowData {};

                for (int col { }; col < m_width; ++col)
                {
                    int blueInfo { file.get() };
                    int greenInfo { file.get() };
                    int redInfo { file.get() };

                    rowData.push_back(RGB(redInfo, greenInfo, blueInfo));
                }

                file.seekg((infoHeader.width % 4), std::ios::cur);

                if (toFlipImage)
                    m_RGBImageMatrix.insert(m_RGBImageMatrix.begin(), rowData);
                else
                    m_RGBImageMatrix.push_back(rowData);
            }

            file.close();
        }
    }
}

void BMP::saveToFile(std::string filename)
{
    std::ofstream file(filename.c_str(), std::ios::out | std::ios::binary);

    if (file.fail())
    {

    }
    else
    {
        BMPFileMagic magic {};
        magic.magic[0] = 'B';
        magic.magic[1] = 'M';
        file.write((char*) &magic, sizeof(magic));

        BMPFileHeader fileHeader {};
        fileHeader.offsetData = sizeof(BMPFileMagic) + sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
        fileHeader.fileSize = (fileHeader.offsetData + (m_RGBImageMatrix.size() * 3 + m_RGBImageMatrix[0].size() % 4) * m_RGBImageMatrix.size());
        file.write((char*) &fileHeader, sizeof(fileHeader));

        BMPInfoHeader infoHeader {};
        infoHeader.headerSize = sizeof(BMPInfoHeader);
        infoHeader.width = m_RGBImageMatrix[0].size();
        infoHeader.height = m_RGBImageMatrix.size();
        infoHeader.planes = 1;
        infoHeader.bitsPerPixel = 24;
        infoHeader.compression = 0;
        infoHeader.sizeImage = 0;
        infoHeader.horizontalResolution = 2835;
        infoHeader.verticalResolution = 2835;
        infoHeader.colorCount = 0;
        infoHeader.importantColorCount = 0;
        file.write((char*) &infoHeader, sizeof(infoHeader));

        for (int row { m_RGBImageMatrix.size() - 1 }; row > -1; --row)
        {
            const std::vector<RGB>& rowData { m_RGBImageMatrix[row] };

            for (int col { }; col < rowData.size(); ++col)
            {
                const RGB& pixelColors { rowData[col] };

                file.put(static_cast<unsigned char>(pixelColors.b));
                file.put(static_cast<unsigned char>(pixelColors.g));
                file.put(static_cast<unsigned char>(pixelColors.r));
            }

            // Pad rows till their lengths are multiples of 4
            for (int i { }; i < (rowData.size() % 4); ++i)
                file.put(0);
        }

        file.close();
    }
}




















