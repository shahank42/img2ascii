#include <iostream>
#include <string>
#include <algorithm>

#include <Image.h>
#include <RGB.h>


/* Constant Values */

const std::string ASCII_CHAR_MAP { "    .,:;i1tfLCG08B@" };
int ASCII_IMAGE_WIDTH { };
int CONTRAST_FACTOR { };



/* ---------------- */




/* Custom Functions */

void pixelToGrayscale(RGB& pixel)
{
    pixel = RGB((0.299 * pixel.r) + (0.587 * pixel.g) + (0.114 * pixel.b));
}

void changeContrast(RGB& pixel)
{
    double contrastFactor { CONTRAST_FACTOR };
    double contrastCorrectionFactor { (259.0 * (contrastFactor + 255)) / (255.0 * (259 - contrastFactor)) };

    double newRed    { std::clamp((contrastCorrectionFactor * (pixel.r - 128) + 128), 0.0, 255.0) };
    double newGreen  { std::clamp((contrastCorrectionFactor * (pixel.g - 128) + 128), 0.0, 255.0) };
    double newBlue   { std::clamp((contrastCorrectionFactor * (pixel.b - 128) + 128), 0.0, 255.0) };

    pixel = RGB(newRed, newGreen, newBlue);
}

void reduceImageRes(Image& img, int redWidth)
{
    auto pixelMatrix { img.getPixelMatrix() };
    std::vector<std::vector<RGB>> reducedImageMap {};

    int horClumpingFactor { img.width / redWidth };
    int verClumpingFactor { horClumpingFactor * img.height / img.width };

    for (int i { }; i < pixelMatrix.size(); i += horClumpingFactor)
    {
        std::vector<RGB> row {};

        for (int j { }; j < pixelMatrix[i].size(); j += verClumpingFactor)
        {

            RGB avgRGB(0);

            for (int ni { i }; ni < i + horClumpingFactor; ++ni)
            {
                for (int nj { j }; nj < j + verClumpingFactor; ++nj)
                {
                    if (ni < pixelMatrix.size() && nj < pixelMatrix[ni].size())
                    {
                        avgRGB.r += pixelMatrix[ni][nj].r;
                        avgRGB.g += pixelMatrix[ni][nj].g;
                        avgRGB.b += pixelMatrix[ni][nj].b;
                    }
                    else
                    {
                        avgRGB.r += 128;
                        avgRGB.g += 128;
                        avgRGB.b += 128;
                    }
                }
            }

            avgRGB.r /= (horClumpingFactor * verClumpingFactor);
            avgRGB.g /= (horClumpingFactor * verClumpingFactor);
            avgRGB.b /= (horClumpingFactor * verClumpingFactor);

            row.push_back(avgRGB);
        }

        reducedImageMap.push_back(row);
    }

    img.setPixelMatrix(reducedImageMap);
}

std::vector<std::vector<double>> getGrayscaleMap(Image& img)
{
    img.transformPixels(pixelToGrayscale);


    auto pixelMatrix { img.getPixelMatrix() };
    std::vector<std::vector<double>> grayMap {};

    for (int i { }; i < pixelMatrix.size(); ++i)
    {
        std::vector<double> row {};

        for (int j { }; j < pixelMatrix[i].size(); ++j)
        {
            row.push_back(pixelMatrix[i][j].b);
        }

        grayMap.push_back(row);
    }

    return grayMap;
}

char grayScaleToASCIIChar(double grayScale)
{
    std::string ASCIIChars { ASCII_CHAR_MAP };
    std::reverse(ASCIIChars.begin(), ASCIIChars.end());
    const unsigned int stringLength { ASCIIChars.size() };

    return ASCIIChars[static_cast<int>((((stringLength - 1) * grayScale) / 255))];
}

std::vector<std::vector<char>> getASCIIMap(std::vector<std::vector<double>> grayMap)
{
    std::vector<std::vector<char>> ASCIIMap {};

    for (int i { }; i < grayMap.size(); ++i)
    {
        std::vector<char> row {};

        for (int j { }; j < grayMap[i].size(); ++j)
        {
            row.push_back(grayScaleToASCIIChar(grayMap[i][j]));
        }

        ASCIIMap.push_back(row);
    }

    return ASCIIMap;
}

void writeASCIIToOutputFile(std::string filename, std::vector<std::vector<char>> ASCIIMap)
{
    std::ofstream file(filename.c_str(), std::ios::out);

    if (file.is_open())
    {
        for (const auto& row : ASCIIMap)
        {
            for (char ASCIIVal : row)
                file << ASCIIVal;

            file << "\n";
        }

        file.close();
    }
}

/* ---------------- */


int main(int argc, char* argv[])
{
    const std::string IMG_FILENAME { argv[1] };
    const std::string OUT_FILENAME { IMG_FILENAME + "-ascii.txt" };


    std::cout << "[*] bmp2ascii Image-To-Text converter by shahank\n";

    std::cout << "[*] Opening image: \"" << IMG_FILENAME << "\"\n";
    Image img { IMG_FILENAME };

    std::cout << "[>] Adjust image contrast by a factor of (default = 128) >>> ";
    std::cin >> CONTRAST_FACTOR;
    if (CONTRAST_FACTOR == '\n') // gotta handle this
        CONTRAST_FACTOR = 128;
    std::cout << "[*] Adjusting image contrast by a factor of " << CONTRAST_FACTOR << "...\n";
    img.transformPixels(changeContrast);

    std::cout << "[>] (Proportionately) resize image to a width of (default = 100) >>> ";
    std::cin >> ASCII_IMAGE_WIDTH;
    if (ASCII_IMAGE_WIDTH == '\n') // gotta handle this too
        ASCII_IMAGE_WIDTH = 100;
    std::cout << "[*] (Proportionately) resizing image to a width of " << ASCII_IMAGE_WIDTH << "\n";
    reduceImageRes(img, ASCII_IMAGE_WIDTH);

    std::vector<std::vector<double>> grayScaleMap { getGrayscaleMap(img) };
    std::vector<std::vector<char>> ASCIIMap { getASCIIMap(grayScaleMap) };

    std::cout << "[*] ASCII conversion successful!\n";

    writeASCIIToOutputFile(OUT_FILENAME, ASCIIMap);

    std::cout << "[*] Find your ASCII art at: \"" << OUT_FILENAME << "\"\n";
    std::cout << "[*} TIP: If things appear distorted and wobbly, try switching to a monospace font.\n";

    std::cout << "[*] Opening output file...\n";
    std::string openingCommand { "start notepad \"" + OUT_FILENAME + "\" &" };
    system(openingCommand.c_str());
    std::cout << "[*] Opened the output file in notepad.\n";

    std::cout << "[*] All tasks completed! :-)\n";
    system("pause");

    return 0;
}
