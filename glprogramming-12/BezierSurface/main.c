#ifdef _WIN32
#include <Windows.h>
#endif

#include <stdlib.h>
#include <math.h>

#include <gl/GL.h>
#include <gl/GLU.h>
#include <GL/glut.h>

GLfloat ctrlpoints[4][4][3] = {
    {
        {-1.5f, -1.5f, 4.0f},
        {-0.5f, -1.5f, 2.0f},
        {0.5f, -1.5f, -1.0f},
        {1.5f, -1.5f, 2.0f}
    },
    {
        {-1.5f, -0.5f, 1.0f},
        {-0.5f, -0.5f, 3.0f},
        {0.5f, -0.5f, 0.0f},
        {1.5f, -0.5f, -1.0f}
    },
    {
        {-1.5f, 0.5f, 4.0f},
        {-0.5f, 0.5f, 0.0f},
        {0.5f, 0.5f, 3.0f},
        {1.5f, 0.5f, 4.0f}
    },
    {
        {-1.5f, 1.5f, -2.0f},
        {-0.5f, 1.5f, -2.0f},
        {0.5f, 1.5f, 0.0f},
        {1.5f, 1.5f, -1.0f}
    }
};

GLfloat texpts[2][2][2] = {
    {{0.0f, 0.0f}, {0.0f, 1.0f}},
    {{1.0f, 0.0f}, {1.0f, 1.0f}}
};

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);
    glEvalMesh2(GL_FILL, 0, 20, 0, 20);
    glFlush();
}

#define IMAGE_WIDTH 64
#define IMAGE_HEIGHT 64
GLubyte image[3 * IMAGE_WIDTH * IMAGE_HEIGHT];

void makeImage(void)
{
    int i, j;
    float ti, tj;

    for (i = 0; i < IMAGE_WIDTH; i++)
    {
        ti = 2.0f * 3.14159265f * i / IMAGE_WIDTH;
        for (j = 0; j < IMAGE_HEIGHT; j++)
        {
            tj = 2.0f * 3.14159265f * j / IMAGE_HEIGHT;
            image[3 * (IMAGE_HEIGHT * i + j)] =
                (GLubyte)127.0f * (1.0f + sinf(ti));
            image[3 * (IMAGE_HEIGHT * i + j) + 1] =
                (GLubyte)127.0f * (1.0f + cosf(2 * tj));
            image[3 * (IMAGE_HEIGHT * i + j) + 2] =
                (GLubyte)127.0f * (1.0f + cosf(ti + tj));
        }
    }
}

void init(void)
{
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
            0, 1, 12, 4, &ctrlpoints[0][0][0]);
    glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2,
            0, 1, 4, 2, &texpts[0][0][0]);
    glEnable(GL_MAP2_TEXTURE_COORD_2);
    glEnable(GL_MAP2_VERTEX_3);
    glMapGrid2f(20, 0.0f, 1.0f, 20, 0.0f, 1.0f);
    makeImage();
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, IMAGE_WIDTH, IMAGE_HEIGHT, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-4.0, 4.0, -4.0 * (GLfloat)h / (GLfloat)w,
                4.0 * (GLfloat)h / (GLfloat)w, -4.0, 4.0);
    else
        glOrtho(-4.0 * (GLfloat)w / (GLfloat)h,
                4.0 * (GLfloat)w / (GLfloat)h, -4.0, 4.0, -4.0, 4.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(85.0f, 1.0f, 1.0f, 1.0f);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
