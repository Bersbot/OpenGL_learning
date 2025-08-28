#include "func.hpp"


int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
    {
        std::cerr << "glfwInit failed!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(Width, Height, "Battle City", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "glfwCreateWindow failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGL())
    {
        std::cerr << "Can't load GLAD!" << std::endl;
    }

    glClearColor(1, 1, 0, 1);
    Triangl tri;
    tri.Points(point);
    tri.Colors(color);
    tri.FragmentShader(fragmentShader);
    tri.VertexShader(vertexShader);
    tri.ShaiderInit();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS)
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        tri.Draw();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}