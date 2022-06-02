#ifndef Texture_H
#define Texture_H

#include <array>
#include <string>
#include <fstream>
#include <vector>

std::vector<std::string> split(const std::string &s, char delim)
{
    // from https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
    std::vector<std::string> result;

    auto start = 0U;
    auto end = s.find(delim);
    while (end != std::string::npos)
    {
        result.emplace_back(s.substr(start, end - start));
        start = end + sizeof(delim);
        end = s.find(delim, start);
    }
    result.emplace_back(s.substr(start, end - start));

    return result;
}

class Texture
{
public:
    uint8_t *image;
    int width;
    int height;

    // reads .ppm file (with P3 encoding + values between 0 and 255) with given path and stores it in an array (height, width , 3)
    // for performance reasons the array is flattened
    Texture() {}
    Texture(const std::string &path)
    {
        std::ifstream file(path);
        std::string line;
        std::getline(file, line);
        std::getline(file, line);
        std::vector<std::string> s = split(line, ' ');
        width = std::stoi(s[0]);
        height = std::stoi(s[1]);
        image = new uint8_t[width * height * 3];
        std::getline(file, line);

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                std::getline(file, line);
                int index = x * 3 + (height - 1 - y) * width * 3;
                s = split(line, ' ');
                image[index] = std::stoi(s[0]);
                image[index + 1] = std::stoi(s[1]);
                image[index + 2] = std::stoi(s[2]);
            }
        }
    }

    // for some reason this gets called even though it shouldn't, resulting in fragments in the image
    // ~Texture()
    // {
    //     if (image)
    //     {
    //         delete[] image;
    //     }
    //     std::cout << "texture destructor called\n";
    // }

    // access elements like it was a 3-dimensional array
    std::array<uint8_t, 3> accessElement(int y, int x) const
    {
        int index = y * width * 3 + x * 3;
        return {image[index], image[index + 1], image[index + 2]};
    }
};

#endif