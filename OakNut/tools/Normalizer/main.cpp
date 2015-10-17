#include "LodePNG.h"

#include <cinttypes>
#include <memory>
#include <string>

#define EDITOR

using namespace std;

struct sImgContext
{
    uint32_t* pData;
    int w;
    int h;
    struct sBakeState
    {
        int bevel = 0;
        int raise = 0;
        int specular = 0;
        int shininess = 0;
        int selfIllum = 0;
        bool invBevel = false;
    } bakeState;
};
sImgContext img;

void normalMap();

int main(int argc, char* argv[])
{
    if (argc < 3) return 1;

    string inputFilename = argv[1];
    string outputFilename = argv[2];

    std::vector<unsigned char> image;
    unsigned int w, h;
    auto ret = lodepng::decode(image, w, h, inputFilename);
    if (ret) return 1;

    img.w = w;
    img.h = h;
    img.pData = (uint32_t*)image.data();

    normalMap();

    std::vector<unsigned char> outPng;
    lodepng::encode(outPng, image, w, h);
    lodepng::save_file(outPng, outputFilename);

    return 0;
}

int clamp(int x, int min, int max)
{
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

#define NORMAL_STRENGTH 1
#define AMBIENT_SIZE 8

void normalMap()
{
    int s[9];
    int *pS;
    int avg;
#ifdef EDITOR
    auto pNewData = new uint32_t[img.w * img.h];
#else
    auto pNewData = (uint32_t*)mem_alloc(img.w * img.h * 4);
#endif
    for (int y = 0; y < img.h; ++y)
    {
        for (int x = 0; x < img.w; ++x)
        {
            pS = s;
            for (int j = y + img.h - 1; j <= y + img.h + 1; ++j)
            {
                for (int i = x + img.w - 1; i <= x + img.w + 1; ++i)
                {
                    *pS = img.pData[(j % img.h) * img.w + (i % img.w)];
                    *pS &= 0xff;
                    ++pS;
                }
            }

            int nx = NORMAL_STRENGTH * -(s[2] - s[0] + 2 * (s[5] - s[3]) + s[8] - s[6]);
            int ny = NORMAL_STRENGTH * -(s[6] - s[0] + 2 * (s[7] - s[1]) + s[8] - s[2]);
            int nz = 255;

            // Normalize
#ifdef EDITOR
            int len = (int)std::sqrt((double)(nx * nx + ny * ny + nz * nz));
#else
            int len = (int)sqrt14((double)(nx * nx + ny * ny + nz * nz));
#endif
            if (len > 0)
            {
                nx = nx * 255 / len;
                ny = ny * 255 / len;
                nz = nz * 255 / len;

                // Bring back in range
                nx = (nx + 255) / 2;
                ny = (ny + 255) / 2;
                nz = (nz + 255) / 2;
            }
            else
            {
                nx = 128;
                ny = 128;
                nz = 255;
            }

            // Ambient
            avg = 0;
            for (int j = y + img.h - AMBIENT_SIZE; j <= y + img.h + AMBIENT_SIZE; ++j)
            {
                for (int i = x + img.w - AMBIENT_SIZE; i <= x + img.w + AMBIENT_SIZE; ++i)
                {
                    avg += img.pData[(j % img.h) * img.w + (i % img.w)] & 0xff;
                }
            }
            avg /= (AMBIENT_SIZE * 2 + 1) * (AMBIENT_SIZE * 2 + 1);
            avg = avg - s[4];
            avg = avg * 1000 / 255;
            avg = 255 - clamp(avg, 0, 255);
            pNewData[y * img.w + x] =
                ((avg << 24) & 0xff000000) |
                ((nz << 16) & 0xff0000) |
                ((ny << 8) & 0xff00) |
                (nx & 0xff);
        }
    }
    memcpy(img.pData, pNewData, img.w * img.h * 4);
#ifdef EDITOR
    delete[] pNewData;
#endif
}
