#include "DrawSurface.h"
#include "Utils.h"

#include <cstddef>

#include <SDL.h>
#include <SDL_stdinc.h>
#include <SDL_surface.h>

DrawSurface::DrawSurface() : m_Surface(nullptr), m_Dealloc(false)
{
}

DrawSurface::DrawSurface(const char* file) : m_Surface(load_image(file)), m_Dealloc(true)
{
}

DrawSurface::DrawSurface(const int xsize, const int ysize) : m_Surface(nullptr), m_Dealloc(true)
{
    Uint32 rmask, gmask, bmask, amask;
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
    #else
    rmask = 0x00ff0000;
    gmask = 0x0000ff00;
    bmask = 0x000000ff;
    amask = 0xff000000;
    #endif
    m_Surface = SDL_CreateRGBSurface(SDL_SWSURFACE, xsize, ysize, 32, rmask, gmask, bmask, amask);
}

DrawSurface::DrawSurface(SDL_Surface* surfaceptr, const bool dealloc) : m_Surface(surfaceptr), m_Dealloc(dealloc)
{
}

DrawSurface::DrawSurface(const DrawSurface& r) : m_Surface(nullptr), m_Dealloc(true)
{
    assign(r);
}

DrawSurface& DrawSurface::operator=(const DrawSurface& r)
{
    assign(r);
    return *this;
}

DrawSurface& DrawSurface::assign(const DrawSurface& r)
{
    // if(r.dealloc)
    assignAsCopyOf(r);
    // else
    // assignAsReferenceTo(r);
    return *this;
}

DrawSurface& DrawSurface::assignAsReferenceTo(const DrawSurface& r)
{
    if (this != (&r))
    {
        if (m_Dealloc && (m_Surface != nullptr))
            SDL_FreeSurface(m_Surface);

        m_Surface = r.m_Surface;
        m_Dealloc = false;
    }
    return *this;
}

DrawSurface& DrawSurface::assignAsCopyOf(const DrawSurface& r)
{
    if (this != (&r))
    {
        if (m_Dealloc && (m_Surface != nullptr))
            SDL_FreeSurface(m_Surface);

        m_Surface = r.copyOf();

        if (m_Surface == nullptr)
            m_Dealloc = false;
        else
            m_Dealloc = true;
    }

    return *this;
}

DrawSurface::~DrawSurface()
{
    if (m_Dealloc && (m_Surface != nullptr))
    {
        SDL_FreeSurface(m_Surface);
    }
}

Uint32 DrawSurface::getFlags() const
{
    return m_Surface->flags;
}

const SDL_PixelFormat* DrawSurface::getFormat() const
{
    return m_Surface->format;
}

int DrawSurface::getWidth() const
{
    return m_Surface->w;
}

int DrawSurface::getHeight() const
{
    return m_Surface->h;
}

Uint16 DrawSurface::getPitch() const
{
    return static_cast<Uint16>(m_Surface->pitch);
}

const Uint32* DrawSurface::getPixels() const
{
    return static_cast<Uint32*>(m_Surface->pixels);
}

const SDL_Surface* DrawSurface::constGetSurface() const
{
    return m_Surface;
}

// SDL_Surface* DrawSurface::getSurface()
// {
//     return surface
// }

Uint32 DrawSurface::pickCol(int x, int y) const
{
    return get_dot(m_Surface, x, y);
}

void DrawSurface::clear() const
{
    clearCol(0x00FFFFFF);
}

void DrawSurface::clearCol(const Uint32 color) const
{
    for (int x = 0; x < getWidth(); x++)
        for (int y = 0; y < getHeight(); y++)
            dot(m_Surface, x, y, color);
}

void DrawSurface::drawDot(int x, int y, Uint32 color) const
{
    dot(m_Surface, x, y, color);
}

void DrawSurface::flipSurface()
{
    // SDL_RenderPresent(m_Renderer);
}

void DrawSurface::drawLine(const int x1, const int y1, const int x2, const int y2, const Uint32 color) const
{
    bline(m_Surface, x1, y1, x2, y2, color);
}

void DrawSurface::drawWuLine(const int x1, const int y1, const int x2, const int y2, const Uint32 color) const
{
    wuline(m_Surface, x1, y1, x2, y2, color);
}

void DrawSurface::drawCircle(const int cx, const int cy, const int r, const Uint32 color) const
{
    rasterCircle(m_Surface, cx, cy, r, color);
}

void DrawSurface::drawWuCircle(const int cx, const int cy, const int r, const Uint32 color) const
{
    wuCircle(m_Surface, cx, cy, r, color);
}

void DrawSurface::applyWuCircle(const int cx, const int cy, const int r, const Uint32 color) const
{
    const DrawSurface mysurface = DrawSurface(getWidth(), getHeight());
    mysurface.clear();
    mysurface.drawWuCircle(cx, cy, r, color);
    mysurface.applyTo(m_Surface);
}

void DrawSurface::applyWuLine(const int x1, const int y1, const int x2, const int y2, const Uint32 color) const
{
    const DrawSurface mysurface = DrawSurface(getWidth(), getHeight());
    mysurface.clear();
    mysurface.drawWuLine(x1, y1, x2, y2, color);
    mysurface.applyTo(m_Surface);
}

void DrawSurface::batchApplyWuLine(const int numLines, const int* lineData, const Uint32 color)
{
    const DrawSurface mysurface = DrawSurface(getWidth(), getHeight());
    mysurface.clear();
    for (int n = 0; n < numLines; n++)
    {
        const int x1 = lineData[n * 4];
        const int y1 = lineData[n * 4 + 1];
        const int x2 = lineData[n * 4 + 2];
        const int y2 = lineData[n * 4 + 3];
        mysurface.drawWuLine(x1, y1, x2, y2, color);
    }
    mysurface.applyTo(this);
}

void DrawSurface::batchApplyContigWuLine(const int numLines, const int* lineData, const Uint32 color)
{
    const DrawSurface mysurface = DrawSurface(getWidth(), getHeight());
    mysurface.clear();
    int lastx = lineData[0];
    int lasty = lineData[1];
    for (int n = 0; n < numLines; n++)
    {
        int x = lineData[n * 2 + 2];
        int y = lineData[n * 2 + 3];
        mysurface.drawWuLine(lastx, lasty, x, y, color);
        lastx = x;
        lasty = y;
    }
    mysurface.applyTo(this);
}

SDL_Surface* DrawSurface::copyOf() const
{
    if (m_Surface != nullptr)
    {
        const int xsize = getWidth();
        const int ysize = getHeight();
        Uint32 rmask, gmask, bmask, amask;
        #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
        #else
        rmask = 0x00ff0000;
        gmask = 0x0000ff00;
        bmask = 0x000000ff;
        amask = 0xff000000;
        #endif
        SDL_Surface* tempsurface = SDL_CreateRGBSurface(SDL_SWSURFACE, xsize, ysize, 32, rmask, gmask, bmask, amask);
        for (int x = 0; x < xsize; x++)
            for (int y = 0; y < ysize; y++)
                dot(tempsurface, x, y, pickCol(x, y));

        return tempsurface;
    }
    else
    {
        return nullptr;
    }
}

void DrawSurface::applyTo(DrawSurface* surfaceto, const int x, const int y) const
{
    surfaceto->applyFrom(m_Surface, x, y);
}

void DrawSurface::applyTo(SDL_Surface* surfaceto, const int x, const int y) const
{
    apply_surface(x, y, m_Surface, surfaceto);
}

void DrawSurface::applyFrom(const DrawSurface* surfacefrom, const int x, const int y) const
{
    surfacefrom->applyTo(m_Surface, x, y);
}

void DrawSurface::applyFrom(const SDL_Surface* surfacefrom, const int x, const int y) const
{
    apply_surface(x, y, surfacefrom, m_Surface);
}
