#ifdef _WIN32
#include <Windows.h>
#endif
#include <stdlib.h>

#include <gl/GL.h>
#include <gl/GLU.h>
#include <GL/glut.h>

// GLfloat ctrlpoints[4][4][3] = {
//     {
//         {-1.5f, -1.5f, 4.0f},
//         {-0.5f, -1.5f, 2.0f},
//         {0.5f, -1.5f, -1.0f},
//         {1.5f, -1.5f, 2.0f}
//     },
//     {
//         {-1.5f, -0.5f, 1.0f},
//         {-0.5f, -0.5f, 3.0f},
//         {0.5f, -0.5f, 0.0f},
//         {1.5f, -0.5f, -1.0f}
//     },
//     {
//         {-1.5f, 0.5f, 4.0f},
//         {-0.5f, 0.5f, 0.0f},
//         {0.5f, 0.5f, 3.0f},
//         {1.5f, 0.5f, 4.0f}
//     },
//     {
//         {-1.5f, 1.5f, -2.0f},
//         {-0.5f, 1.5f, -2.0f},
//         {0.5f, 1.5f, 0.0f},
//         {1.5f, 1.5f, -1.0f}
//     }
// };

// void display(void)
// {
//     int i, j;
//
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//     glColor3f(1.0f, 1.0f, 1.0f);
//     glPushMatrix();
//     glRotatef(85.0f, 1.0f, 1.0f, 1.0f);
//     for (j = 0; j <= 8; j++)
//     {
//         glBegin(GL_LINE_STRIP);
//         for (i = 0; i <= 30; i++)
//             glEvalCoord2f((GLfloat)i / 30.0f, (GLfloat)j / 8.0f);
//         glEnd();
//         glBegin(GL_LINE_STRIP);
//         for (i = 0; i <= 30; i++)
//             glEvalCoord2f((GLfloat)j / 8.0f, (GLfloat)i / 30.0f);
//         glEnd();
//     }
//     glPopMatrix();
//     glFlush();
// }

// void init(void)
// {
//     glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//     glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
//             0, 1, 12, 4, &ctrlpoints[0][0][0]);
//     glEnable(GL_MAP2_VERTEX_3);
//     glMapGrid2f(20, 0.0f, 1.0f, 20, 0.0f, 1.0f);
//     glEnable(GL_DEPTH_TEST);
//     glShadeModel(GL_FLAT);
// }

// int main(int argc, char** argv)
// {
//     glutInit(&argc, argv);
//     glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
//     glutInitWindowSize(500, 500);
//     glutInitWindowPosition(100, 100);
//     glutCreateWindow(argv[0]);
//     init();
//     glutDisplayFunc(display);
//     glutMainLoop();
//     return 0;
// }
