#ifdef _WIN32
#include <Windows.h>
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <gl/GL.h>
#include <gl/GLU.h>
#include <GL/glut.h>

GLfloat ctlpoints[4][4][3];
int showPoints = 0;

GLUnurbsObj* theNurb;

void initSurface(void)
{
    int u, v;
    for (u = 0; u < 4; u++)
    {
        for (v = 0; v < 4; v++)
        {
            ctlpoints[u][v][0] = 2.0f * ((GLfloat)u - 1.5f);
            ctlpoints[u][v][1] = 2.0f * ((GLfloat)v - 1.5f);

            if ((u == 1 || u == 2) && (v == 1 || v == 2))
                ctlpoints[u][v][2] = 3.0f;
            else
                ctlpoints[u][v][2] = -3.0f;
        }
    }
}

void nurbsError(GLenum errorCode)
{
    const GLubyte* estring;

    estring = gluErrorString(errorCode);
    fprintf(stderr, "Nurbs Error: %s\n", estring);
    exit(0);
}

void init(void)
{
    GLfloat mat_diffuse[] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat mat_shininess[] = {100.0f};

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);

    initSurface();

    theNurb = gluNewNurbsRenderer();
    gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0f);
    gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
    gluNurbsCallback(theNurb, GLU_ERROR,
                     (GLvoid (*)())nurbsError);
}

void display(void)
{
    GLfloat knots[8] = {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glRotatef(330.0f, 1.f, 0.f, 0.f);
    glScalef(0.5f, 0.5f, 0.5f);

    gluBeginSurface(theNurb);
    gluNurbsSurface(
        theNurb,
        8, knots,
        8, knots,
        4 * 3, 3,
        &ctlpoints[0][0][0],
        4, 4, GL_MAP2_VERTEX_3);
    gluEndSurface(theNurb);

    if (showPoints)
    {
        int j;
        int i;
        glPointSize(5.0f);
        glDisable(GL_LIGHTING);
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POINTS);
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 4; j++)
            {
                glVertex3f(
                    ctlpoints[i][j][0],
                    ctlpoints[i][j][1],
                    ctlpoints[i][j][2]);
            }
        }
        glEnd();
        glEnable(GL_LIGHTING);
    }
    glPopMatrix();
    glFlush();
}

void displayTrim(void)
{
    GLfloat knots[8] = {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat edgePt[5][2] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f}}; /* counter clockwise */
    GLfloat curvePt[4][2] = {{0.25f, 0.5f}, {0.25f, 0.75f}, {0.75f, 0.75f}, {0.75f, 0.5f}}; /* clockwise */
    GLfloat curveKnots[8] = {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat pwlPt[4][2] = {{0.75f, 0.5f}, {0.5f, 0.25f}, {0.25f, 0.5f}}; /* clockwise */

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glRotatef(330.0f, 1.f, 0.f, 0.f);
    glScalef(0.5f, 0.5f, 0.5f);

    gluBeginSurface(theNurb);
    gluNurbsSurface(
        theNurb,
        8, knots,
        8, knots,
        4 * 3, 3,
        &ctlpoints[0][0][0],
        4, 4, GL_MAP2_VERTEX_3);

    gluBeginTrim(theNurb);
    gluPwlCurve(theNurb, 5, &edgePt[0][0], 2,GLU_MAP1_TRIM_2);
    gluEndTrim(theNurb);

    gluBeginTrim(theNurb);
    gluNurbsCurve(theNurb,8, curveKnots,2, &curvePt[0][0], 4, GLU_MAP1_TRIM_2);
    gluPwlCurve(theNurb, 3, &pwlPt[0][0], 2,GLU_MAP1_TRIM_2);
    gluEndTrim(theNurb);
    gluEndSurface(theNurb);

    if (showPoints)
    {
        int j;
        int i;
        glPointSize(5.0f);
        glDisable(GL_LIGHTING);
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POINTS);
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 4; j++)
            {
                glVertex3f(
                    ctlpoints[i][j][0],
                    ctlpoints[i][j][1],
                    ctlpoints[i][j][2]);
            }
        }
        glEnd();
        glEnable(GL_LIGHTING);
    }

    glPopMatrix();
    glFlush();
}

void reshape(const int w, const int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLdouble)w / (GLdouble)h, 3.0, 8.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -5.0f);
}

void keyboard(unsigned char key, int x, int y)
{
    (void)x;
    (void)y;
    switch (key)
    {
    case 'c':
    case 'C':
        showPoints = !showPoints;
        glutPostRedisplay();
        break;
    case 27:
        exit(0);
    default:
        break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutReshapeFunc(reshape);
    // glutDisplayFunc(display);
    glutDisplayFunc(displayTrim);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
