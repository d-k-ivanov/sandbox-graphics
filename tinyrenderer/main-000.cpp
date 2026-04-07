#include "tgaimage.h"

#include <cmath>
#include <iostream>

constexpr TGAColor white  = {255, 255, 255, 255};    // attention, BGRA order
constexpr TGAColor green  = {0, 255, 0, 255};
constexpr TGAColor red    = {0, 0, 255, 255};
constexpr TGAColor blue   = {255, 128, 64, 255};
constexpr TGAColor yellow = {0, 200, 255, 255};

int main(int argc, char** argv)
{
    constexpr int width  = 64;
    constexpr int height = 64;
    TGAImage      framebuffer(width, height, TGAImage::RGB);

    int ax = 31, ay = 59;
    int bx = 3, by = 3;
    int cx = 59, cy = 3;

    framebuffer.set(ax, ay, red);
    framebuffer.set(bx, by, green);
    framebuffer.set(cx, cy, blue);

    framebuffer.write_tga_file("framebuffer.tga");

    return 0;
}
