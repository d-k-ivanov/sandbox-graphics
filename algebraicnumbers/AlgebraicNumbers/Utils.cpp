#include "Utils.h"

#include <cstddef>
#include <SDL_image.h>
#include <SDL_surface.h>


SDL_Surface* load_image(const char* filename, const Uint8 r, const Uint8 g, const Uint8 b)
{
    SDL_Surface* loadedImage = nullptr;
    SDL_Surface* optimizedImage = nullptr;
    loadedImage = IMG_Load(filename);
    if (loadedImage != nullptr)
    {
        optimizedImage = SDL_ConvertSurfaceFormat(loadedImage, SDL_PIXELFORMAT_RGBA8888, 0);
        const Uint32 colorkey = SDL_MapRGB(loadedImage->format, r, g, b);

        SDL_SetColorKey(optimizedImage, SDL_TRUE, colorkey);
        SDL_FreeSurface(loadedImage);
    }
    return optimizedImage;
}

SDL_Surface* load_image(const char* filename)
{
    SDL_Surface* loadedImage = nullptr;
    SDL_Surface* optimizedImage = nullptr;
    loadedImage = IMG_Load(filename);

    if (loadedImage != nullptr)
    {
        optimizedImage = SDL_ConvertSurfaceFormat(loadedImage, SDL_PIXELFORMAT_RGBA8888, 0);
        SDL_FreeSurface(loadedImage);
    }
    return optimizedImage;
}

void apply_surface(int x, int y, const SDL_Surface* source, SDL_Surface* destination)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(const_cast<SDL_Surface*>(source), nullptr, destination, &offset);
}

void apply_surface(const int x, const int y, const SDL_Surface* source, SDL_Surface* destination,
                   const SDL_Rect* cliprect)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(const_cast<SDL_Surface*>(source), const_cast<SDL_Rect*>(cliprect), destination, &offset);
}

Uint32 get_dot(const SDL_Surface* surface, const int x, const int y)
{
    if (x >= 0 && y >= 0 && x < surface->w && y < surface->h)
    {
        const Uint32* pixels = static_cast<Uint32*>(surface->pixels);
        return pixels[(y * surface->w) + x];
    }
    else { return 0; }
}

void dot(const SDL_Surface* surface, const int x, const int y, const Uint32 pixel)
{
    if (x >= 0 && y >= 0 && x < surface->w && y < surface->h)
    {
        auto* pixels = static_cast<Uint32*>(surface->pixels);
        pixels[(y * surface->w) + x] = pixel;
    }
}

Uint8 rgbb(Uint32 pixel)
{
    return static_cast<Uint8>(pixel & 0x000000FF);
}

Uint8 rgbg(Uint32 pixel)
{
    return static_cast<Uint8>((pixel & 0x0000FF00) >> 8);
}

Uint8 rgbr(Uint32 pixel)
{
    return static_cast<Uint8>((pixel & 0x00FF0000) >> 16);
}

Uint8 rgba(const Uint32 pixel)
{
    return static_cast<Uint8>(pixel >> 24);
}

Uint8 lerp(const Uint8 a, const Uint8 b, double t)
{
    const auto ret = static_cast<Uint8>((b - a) * t + a);
    return ret;
}

void rasterCircle(const SDL_Surface* surface, const int x0, const int y0, const int radius, const Uint32 color)
{
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    dot(surface, x0, y0 + radius, color);
    dot(surface, x0, y0 - radius, color);
    dot(surface, x0 + radius, y0, color);
    dot(surface, x0 - radius, y0, color);

    while (x < y)
    {
        // ddF_x == 2 * x + 1;
        // ddF_y == -2 * y;
        // f == x * x + y * y - radius * radius + 2 * x - y + 1;
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        dot(surface, x0 + x, y0 + y, color);
        dot(surface, x0 - x, y0 + y, color);
        dot(surface, x0 + x, y0 - y, color);
        dot(surface, x0 - x, y0 - y, color);
        dot(surface, x0 + y, y0 + x, color);
        dot(surface, x0 - y, y0 + x, color);
        dot(surface, x0 + y, y0 - x, color);
        dot(surface, x0 - y, y0 - x, color);
    }
}

void wuCircle(const SDL_Surface* surface, const int cx, const int cy, const int r, Uint32 color)
{
    Uint32 alpha1, alpha2;
    const Uint32 color2 = color & 0x00FFFFFF;
    int x = r;
    int y = -1;
    float t = 0;
    while (x > y)
    {
        y++;

        const float curDist = 1.0f - fmodf(sqrtf(static_cast<float>(r * r - y * y)), 1.0);
        if (curDist < t)
            x--;
        alpha1 = color2 | static_cast<Uint8>(127 * curDist) << 24;
        alpha2 = color2 | static_cast<Uint8>(127 - 127 * curDist) << 24;


        dot(surface, cx - y, cy - x, color);
        dot(surface, cx - y, cy - x - 1, alpha2);
        dot(surface, cx - y, cy - x + 1, alpha1);

        dot(surface, cx - x, cy - y, color);
        dot(surface, cx - x - 1, cy - y, alpha2);
        dot(surface, cx - x + 1, cy - y, alpha1);

        dot(surface, cx - x, cy + y, color);
        dot(surface, cx - x - 1, cy + y, alpha2);
        dot(surface, cx - x + 1, cy + y, alpha1);

        dot(surface, cx - y, cy + x, color);
        dot(surface, cx - y, cy + x + 1, alpha2);
        dot(surface, cx - y, cy + x - 1, alpha1);

        dot(surface, cx + y, cy + x, color);
        dot(surface, cx + y, cy + x - 1, alpha1);
        dot(surface, cx + y, cy + x + 1, alpha2);

        dot(surface, cx + x, cy + y, color);
        dot(surface, cx + x - 1, cy + y, alpha1);
        dot(surface, cx + x + 1, cy + y, alpha2);

        dot(surface, cx + x, cy - y, color);
        dot(surface, cx + x - 1, cy - y, alpha1);
        dot(surface, cx + x + 1, cy - y, alpha2);

        dot(surface, cx + y, cy - x, color);
        dot(surface, cx + y, cy - x - 1, alpha2);
        dot(surface, cx + y, cy - x + 1, alpha1);

        t = curDist;
    }
}

void wuline(SDL_Surface* surface, int x1, int y1, int x2, int y2, Uint32 pixel)
{
    double dx, dy, xend, yend, xgap, ygap, xpxl1, ypxl1, xpxl2, ypxl2, intery, interx, gradient, f;
    Uint8 alph;
    dx = x2 - x1;
    dy = y2 - y1;
    int ax, ay, a, b;
    Uint32 pixnoalph = (pixel & 0x00FFFFFF);
    if (abs(static_cast<int>(dx)) > abs(static_cast<int>(dy)))
    {
        if (x2 < x1)
        {
            ax = x1;
            x1 = x2;
            x2 = ax;
            ay = y1;
            y1 = y2;
            y2 = ay;
        }
        gradient = dy / dx;
        xend = ceil(x1);
        yend = y1 + gradient * (xend - x1);
        xgap = 1.0 - fract(x1 + 0.5);
        xpxl1 = xend;
        ypxl1 = floor(yend);
        f = 1.0 - fract(yend) * xgap;
        alph = lerp(rgba(get_dot(surface, static_cast<int>(xpxl1), static_cast<int>(ypxl1))), rgba(pixel), f);
        dot(surface, static_cast<int>(xpxl1), static_cast<int>(ypxl1), pixnoalph | (alph << 24));
        f = fract(yend) * xgap;
        alph = lerp(rgba(get_dot(surface, static_cast<int>(xpxl1), static_cast<int>(ypxl1) + 1)), rgba(pixel), f);
        dot(surface, static_cast<int>(xpxl1), static_cast<int>(ypxl1) + 1, pixnoalph | (alph << 24));
        intery = yend + gradient;
        xend = ceil(x2);
        yend = y2 + gradient * (xend - x2);
        xgap = 1.0 - fract(x2 + 0.5);
        xpxl2 = xend;
        ypxl2 = (int)(yend);
        f = 1.0 - fract(yend) * xgap;
        alph = lerp(rgba(get_dot(surface, static_cast<int>(xpxl2), static_cast<int>(ypxl2))), rgba(pixel), f);
        dot(surface, static_cast<int>(xpxl2), static_cast<int>(ypxl2), pixnoalph | (alph << 24));
        f = fract(yend) * xgap;
        alph = lerp(rgba(get_dot(surface, static_cast<int>(xpxl2), static_cast<int>(ypxl2) + 1)), rgba(pixel), f);
        dot(surface, static_cast<int>(xpxl2), static_cast<int>(ypxl2) + 1, pixnoalph | (alph << 24));


        a = static_cast<int>(xpxl1) + 1;
        b = static_cast<int>(xpxl2) - 1;
        for (int x = a; x <= b; x++)
        {
            f = 1.0 - fract(intery);
            alph = lerp(rgba(get_dot(surface, x, static_cast<int>(intery))), rgba(pixel), f);
            dot(surface, x, static_cast<int>(intery), pixnoalph | (alph << 24));


            f = fract(intery);
            alph = lerp(rgba(get_dot(surface, x, static_cast<int>(intery) + 1)), rgba(pixel), f);
            dot(surface, x, static_cast<int>(intery) + 1, pixnoalph | (alph << 24));

            intery = intery + gradient;
        }
    }
    else
    {
        if (y2 < y1)
        {
            ax = x1;
            x1 = x2;
            x2 = ax;
            ay = y1;
            y1 = y2;
            y2 = ay;
        }

        gradient = dx / dy;

        yend = ceil(y1);
        xend = x1 + gradient * (yend - y1);
        ygap = 1.0 - fract(y1 + 0.5);
        xpxl1 = static_cast<int>(xend);
        ypxl1 = yend;
        f = 1.0 - fract(xend) * ygap;
        alph = lerp(rgba(get_dot(surface, static_cast<int>(xpxl1), static_cast<int>(ypxl1))), rgba(pixel), f);
        dot(surface, static_cast<int>(xpxl1), static_cast<int>(ypxl1), pixnoalph | (alph << 24));

        f = fract(xend) * ygap;
        alph = lerp(rgba(get_dot(surface, static_cast<int>(xpxl1), static_cast<int>(ypxl1) + 1)), rgba(pixel), f);
        dot(surface, static_cast<int>(xpxl1), static_cast<int>(ypxl1) + 1, pixnoalph | (alph << 24));

        interx = xend + gradient;

        yend = ceil(y2);
        xend = x2 + gradient * (yend - y2);
        ygap = fract(y2 + 0.5);
        xpxl2 = static_cast<int>(xend);
        ypxl2 = yend;
        f = 1.0 - fract(xend) * ygap;
        alph = lerp(rgba(get_dot(surface, static_cast<int>(xpxl2), static_cast<int>(ypxl2))), rgba(pixel), f);
        dot(surface, static_cast<int>(xpxl2), static_cast<int>(ypxl2), pixnoalph | (alph << 24));

        f = fract(xend) * ygap;
        alph = lerp(rgba(get_dot(surface, static_cast<int>(xpxl2), static_cast<int>(ypxl2) + 1)), rgba(pixel), f);
        dot(surface, static_cast<int>(xpxl2), static_cast<int>(ypxl2) + 1, pixnoalph | (alph << 24));


        a = static_cast<int>(ypxl1) + 1;
        b = static_cast<int>(ypxl2) - 1;
        for (int y = a; y <= b; y++)
        {
            f = 1.0 - fract(interx);
            alph = lerp(rgba(get_dot(surface, static_cast<int>(interx), y)), rgba(pixel), f);
            dot(surface, static_cast<int>(interx), y, pixnoalph | (alph << 24));

            f = fract(interx);
            alph = lerp(rgba(get_dot(surface, static_cast<int>(interx) + 1, y)), rgba(pixel), f);
            dot(surface, static_cast<int>(interx) + 1, y, pixnoalph | (alph << 24));
            interx = interx + gradient;
        }
    }
}


double fract(double x)
{
    return fmod(x, 1.0);
}

void bline(const SDL_Surface* surface, int x, int y, const int x2, const int y2, const Uint32 color)
{
    const int w = x2 - x;
    const int h = y2 - y;
    int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;

    if (w < 0)
        dx1 = -1;
    else if (w > 0)
        dx1 = 1;

    if (h < 0)
        dy1 = -1;
    else if (h > 0)
        dy1 = 1;

    if (w < 0)
        dx2 = -1;
    else if (w > 0)
        dx2 = 1;

    int longest = abs(w);
    int shortest = abs(h);
    if (!(longest > shortest))
    {
        longest = abs(h);
        shortest = abs(w);
        if (h < 0)
            dy2 = -1;
        else if (h > 0)
            dy2 = 1;
        dx2 = 0;
    }
    int numerator = longest >> 1;
    for (int i = 0; i <= longest; i++)
    {
        dot(surface, x, y, color);
        numerator += shortest;
        if (!(numerator < longest))
        {
            numerator -= longest;
            x += dx1;
            y += dy1;
        }
        else
        {
            x += dx2;
            y += dy2;
        }
    }
}
