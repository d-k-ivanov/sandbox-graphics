#include "Algebraic.h"
#include "CameraController.h"
#include "Complex.h"
#include "DrawSurface.h"
#include "Vector2.h"

#include <iostream>
#include <limits>
#include <vector>

#include <SDL.h>
#include <SDL_opengl.h>
#include <gl/glu.h>

bool sRunning;
int mouseposx;
int mouseposy;
CameraController cam;

int getMousePosX();
int getMousePosY();
bool sLoop();
void sInit();
void sHandleEvents();
void sSync();
void sQuit();
bool mousedown = false;
int mousezoom = 0;
double ndcmousex = 0;
double ndcmousey = 0;
constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

//square function. used in texture generation.
float sq(const float arg)
{
    return arg * arg;
}

void putblob(const float x, const float y, const float r)
{
    glTexCoord2f(1, 1);
    glVertex2f(x + r * 16, y + r * 16);
    glTexCoord2f(1, 0);
    glVertex2f(x + r * 16, y - r * 16);
    glTexCoord2f(0, 0);
    glVertex2f(x - r * 16, y - r * 16);
    glTexCoord2f(0, 1);
    glVertex2f(x - r * 16, y + r * 16);
}

//generate circular OpenGL mipmapped texture.
GLuint othertex(const unsigned sz)
{
    GLuint ret;
    glGenTextures(1, &ret);
    glBindTexture(GL_TEXTURE_2D, ret);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int n, x, y;
    const int ys = static_cast<int>(sz);
    const int xs = static_cast<int>(sz);
    auto* td = new unsigned char[xs * ys * 3];
    float f;

    for (y = ys - 1; y >= 0; y--)
        for (x = xs - 1; x >= 0; x--)
        {
            n = (y * xs + x) * 3;
            f = sq(static_cast<float>(sz) / 2.0f) / (1 + sq(static_cast<float>(x) - static_cast<float>(xs) / 2.0f) + sq(static_cast<float>(y) - static_cast<float>(ys) / 2.0f));
            f = floor(f);
            if (f > 255) f = 255;
            td[n] = td[n + 1] = td[n + 2] = static_cast<unsigned char>(f);
        }
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, xs, ys, GL_RGB, GL_UNSIGNED_BYTE, td);
    delete[] td;
    return ret;
}

int main(int argc, char** argv)
{
    // (void)argc;
    // (void)argv;

    sInit();
    GLuint list = 0;
    const std::vector<Point> p = precalc(14);
    const GLuint tex = othertex(256);

    while (sLoop())
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        cam.applyCameraTransform();
        // glScaled(.8, .8 * 1280 / 800.0, 1);
        // glTranslated(-.5, -.30, 0);
        if (!list)
        {
            list = glGenLists(1);
            glNewList(list, GL_COMPILE_AND_EXECUTE);
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE);
            glDisable(GL_DEPTH_TEST);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, tex);
            glBegin(GL_QUADS);
            const float k1 = .125f;
            const float k2 = .55f;
            for (const auto& n : p)
            {
                switch (n.o)
                {
                case 1: glColor3f(1, 0, 0);
                    break;
                case 2: glColor3f(0, 1, 0);
                    break;
                case 3: glColor3f(0, 0, 1);
                    break;
                case 4: glColor3f(0.7f, 0.7f, 0);
                    break;
                case 5: glColor3f(1, 0.6f, 0);
                    break;
                case 6: glColor3f(0, 1, 1);
                    break;
                case 7: glColor3f(1, 0, 1);
                    break;
                case 8: glColor3f(0.6f, 0.6f, 0.6f);
                    break;
                default: glColor3f(1, 1, 1);
                    break;
                }
                // Alternative coloring
                // float u = static_cast<float>(n.o);
                // if (u > 8)
                //     u = 8;
                // u /= 8;
                // float c1[4] = {55, 126, 184, 0.1};
                // float c2[4] = {255, 127, 0, 1};
                // float c3[4] = { (c1[0] * (1.0f - u) + c2[0] * u) / 255.0f,
                //                 (c1[1] * (1.0f - u) + c2[1] * u) / 255.0f,
                //                 (c1[2] * (1.0f - u) + c2[2] * u) / 255.0f };
                // glColor4fv(c3);
                putblob(
                    static_cast<float>(n.x),
                    static_cast<float>(n.y),
                    k1 * powf(k2, static_cast<float>(n.h) - 3.0f)
                );
                // std::cout << Vector2(static_cast<float>(n.x), static_cast<float>(n.y)) << std::endl;
            }
            glEnd();
            glEndList();
        }
        else if (list)
        {
            glCallList(list);
            // std::cout << list << std::endl;
        }
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        cam.update(1);
        sSync();
        // std::cout << Complex(1,1).length2() << std::endl;
    }
    sQuit();

    return 0;
}

bool sLoop()
{
    sHandleEvents();
    // std::cout << sRunning << std::endl;
    return sRunning;
}

void sInit()
{
    sRunning = true;
    cam = CameraController(0, 0, 0, 1280.0 / 800);
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Video initialization failed: " << SDL_GetError() << std::endl;
    }

    window = SDL_CreateWindow(
        "Algebraic Numbers",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_OPENGL
    );

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void sHandleEvents()
{
    SDL_Event e;
    mousezoom = 0;
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            sRunning = false;
            break;
        case SDL_KEYDOWN:
            switch (e.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                sRunning = false;
                break;
            }
        case SDL_MOUSEBUTTONDOWN:
            switch (e.button.button)
            {
            case SDL_BUTTON_LEFT:
                mousedown = true;
                ndcmousex = e.button.x * 2.0 / WIDTH - 1;
                ndcmousey = -e.button.y * 2.0 / HEIGHT + 1;
                break;
            case SDL_MOUSEWHEEL:
                if (e.wheel.y < 0)
                    mousezoom += 1;
                else
                    mousezoom -= 1;
                break;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                mousedown = false;
                cam.enddrag();
            }
            break;
        case SDL_MOUSEMOTION:
            ndcmousex = e.motion.x * 2.0 / WIDTH - 1;
            ndcmousey = -e.motion.y * 2.0 / HEIGHT + 1;
            break;
        }
    }
    if (mousezoom != 0)
    {
        cam.mouseZoom(mousezoom * .05);
    }
    cam.setMouseZoomPos(ndcmousex, ndcmousey);
    if (mousedown)
    {
        cam.drag(ndcmousex, ndcmousey);
    }
}

void sSync()
{
    SDL_GL_SwapWindow(window);
}

void sQuit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
