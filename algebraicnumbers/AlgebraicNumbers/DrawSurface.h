#pragma once

#include <SDL.h>

// SDL_Surface wrapper class provides functionality of an SDL_Surface, including pixel/drawing operations.
class DrawSurface
{
public:
    // ====================================Constructors=====================================//
    // Default constructor: sets the surface to nullptr.
    DrawSurface();

    // FileName constructor: Loads an image using the ngraphics/Utils load_image function.
    DrawSurface(const char* file);

    // Blank Image constructor: creates a blank, transparent image of size xsize,ysize.
    DrawSurface(int xsize, int ysize);

    /* Image reference constructor
        This does not copy the SDL_Surface pointer to
        by surfaceptr, but instead sets the internal
        SDL_Surface* to surfaceptr. If "dealloc" is true,
        the destructor will try to deallocate the surface
        passed in.

        Correct usage:
        SDL_Surface* mysfc=load_image("abc.png");
        {
            DrawSurface mysfc=DrawSurface(mysfc,true);
        }

        or:
        SDL_Surface* mysfc=load_image("abc.png");

        {
            DrawSurface mysfc=DrawSurface(mysfc,false);
        }

        SDL_FreeSurface(mysfc);
    */
    DrawSurface(SDL_Surface* surfaceptr, bool dealloc);

    // Copy Constructor: Constructs a new surface by duplicating the surface.
    DrawSurface(const DrawSurface& r);

    // Assignment Operator: Copies the drawsurface passed in, like the copy constructor.
    DrawSurface& operator =(const DrawSurface& r);

    DrawSurface& assign(const DrawSurface& r);
    DrawSurface& assignAsReferenceTo(const DrawSurface& r);
    DrawSurface& assignAsCopyOf(const DrawSurface& r);

    /*
        IF the surface was allocated with "DrawSurface(SDL_Surface*,false)",
        then the destructor does nothing. ELSE the destructor calls SDL_FreeSurface()
        on the internal surface pointer.
    */
    virtual ~DrawSurface();

    // ==============================Const Getters=============================//
    /* Returns an SDL_Surface* copy of the current surface.
     * The returned pointer must be deallocated through SDL_FreeSurface
    */
    SDL_Surface* copyOf() const;

    // gets the surface pointer's pixel data
    Uint32 getFlags() const;
    const SDL_PixelFormat* getFormat() const;
    int getWidth() const;
    int getHeight() const;
    Uint16 getPitch() const;
    const Uint32* getPixels() const;

    // gets the color at pixel x/y. RETURNS 0 if x/y are out of bounds of the image data.
    Uint32 pickCol(int x, int y) const;


    const SDL_Surface* constGetSurface() const;

    // =============================Drawing Functions===============================//
    // exposes the SDL_Surface member.
    // SDL_Surface* getSurface();

    // calls SDL_Flip on the surface.
    void flipSurface();

    // sets all pixels to transparent white.
    void clear() const;

    // sets all pixels to the given color
    void clearCol(Uint32 color) const;

    // puts a color at the given point. If x/y are out of bounds this does nothing.
    void drawDot(int x, int y, Uint32 color) const;

    // draws an aliased line.
    void drawLine(int x1, int y1, int x2, int y2, Uint32 color) const;

    // draws an aliased circle.
    void drawCircle(int cx, int cy, int radius, Uint32 color) const;

    // draw functions OVERWRITE colors to transparency.
    // If you draw an antialiased line to a picture directly it will look aliased! Use the apply functions.

    // draws a xiaolin-wu anti-aliased circle.
    void drawWuCircle(int cx, int cy, int radius, Uint32 color) const;

    // draws a xiaolin-wu anti-aliased line.
    void drawWuLine(int x1, int y1, int x2, int y2, Uint32 color) const;

    // application functions: creates and draws on a separate surface, then SDLBlits them.
    void applyWuCircle(int cx, int cy, int radius, Uint32 color) const;
    void applyWuLine(int x1, int y1, int x2, int y2, Uint32 color) const;

    // int array format: [x1,y1,x2,y2, x1,y1,x2,y2, etc.]
    // draws a series of lines on a single surface THEN applies them.
    void batchApplyWuLine(int numLines, const int* lineData, Uint32 color);

    // draws a chain of lines: [x1,y1,x2,y2,x3,y3,x4,y4 etc.]
    void batchApplyContigWuLine(int numLines, const int* lineData, Uint32 color);

    // apply functions use SDL_Blit. ApplyFrom modifies this, applyTo modifies surfaceto.
    void applyTo(DrawSurface* surfaceto, int x = 0, int y = 0) const;
    void applyTo(SDL_Surface* surfaceto, int x = 0, int y = 0) const;
    void applyFrom(const DrawSurface* surfacefrom, int x = 0, int y = 0) const;
    void applyFrom(const SDL_Surface* surfacefrom, int x = 0, int y = 0) const;


private:
    SDL_Surface* m_Surface;
    SDL_Renderer* m_Renderer;
    bool m_Dealloc;
};
