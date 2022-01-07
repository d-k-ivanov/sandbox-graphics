#include "Main.h"

#include <GLFW/glfw3.h>

#include <iosfwd>


#ifdef _WIN32
#include <Windows.h>
#endif

int main(int argc, char* argv[], char* env[])
{
    // To turn off messages about unused variables.
    ((void)argc);
    ((void)argv);
    ((void)env);

    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
