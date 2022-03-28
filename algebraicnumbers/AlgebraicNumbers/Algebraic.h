#pragma once

#include <vector>

using Point = struct
{
    double x, y;
    int h, o;
};

std::vector<Point> precalc(int maxh);
