//Práctica 2
//Celis Hernández Ronie
//24/08/2025
//318143093

#include <iostream>

//#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Shaders
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 800, HEIGHT = 600;

int main() {
    glfwInit();
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 2. Celis Hernandez Ronie", NULL, NULL);
    glfwSetFramebufferSizeCallback(window, resize);

    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;

    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialise GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Info de OpenGL
    std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    Shader ourShader("Shader/core.vs", "Shader/core.frag");


    const float Xc[10] = { -0.4875f,-0.4125f,-0.3375f,-0.2625f,-0.1875f,-0.1125f,-0.0375f, 0.0375f, 0.1125f, 0.1875f };
    const float Yc[10] = { -0.8f,   -0.7f,   -0.6f,   -0.5f,   -0.4f,   -0.3f,   -0.2f,   -0.1f,    0.0f,     0.1f };
    const float DX = 0.075f;
    const float DY = 0.1f;

    float CR[4] = { 0.0f, 1.0f, 0.678f, 1.0f };
    float CG[4] = { 0.0f, 0.0f, 0.847f, 1.0f };
    float CB[4] = { 0.0f, 0.0f, 0.902f, 1.0f };


#define franjas 41
    int stripRow[franjas] = {
        // y = -0.8
        0,
        // y = -0.7
        1,1,1,1,1,
        // y = -0.6
        2,2,2,2,2,
        // y = -0.5
        3,3,3,
        // y = -0.4
        4,4,4,4,4,
        // y = -0.3
        5,5,5,5,5,5,5,
        // y = -0.2
        6,6,6,6,6,6,6,6,
        // y = -0.1
        7,7,7,
        // y =  0.0
        8,8,8,
        // y =  0.1
        9
    };
    int stripC0[franjas] = {
        // -0.8
        1,
        // -0.7
        1, 2, 4, 5, 7,
        // -0.6
        1, 2, 3, 5, 7,
        // -0.5
        1, 2, 7,
        // -0.4
        0, 5, 7, 8, 9,
        // -0.3
        0, 1, 4, 5, 7, 8, 9,
        // -0.2
        0, 1, 2, 4, 5, 7, 8, 9,
        // -0.1
        0, 5, 7,
        //  0.0
        1, 2, 6,
        //  0.1
        2
    };
    int stripC1[franjas] = {
        // -0.8
        7,
        // -0.7
        1, 3, 4, 6, 7,
        // -0.6
        1, 2, 4, 6, 7,
        // -0.5
        1, 6, 9,
        // -0.4
        4, 6, 7, 8, 9,
        // -0.3
        0, 3, 4, 6, 7, 8, 9,
        // -0.2
        0, 1, 3, 4, 6, 7, 8, 9,
        // -0.1
        4, 6, 9,
        //  0.0
        1, 5, 7,
        //  0.1
        6
    };
    int stripCol[franjas] = {
        // -0.8
        0,
        // -0.7
        0, 1, 0, 1, 0,
        // -0.6
        0, 1, 0, 1, 0,
        // -0.5
        0, 1, 0,
        // -0.4
        0, 1, 0, 1, 0,
        // -0.3
        0, 2, 0, 1, 0, 1, 0,
        // -0.2
        0, 2, 3, 0, 1, 0, 1, 0,
        // -0.1
        0, 1, 0,
        //  0.0
        0, 1, 0,
        //  0.1
        0
    };

    // -------------------------------------------
    // Construcción del VBO: cada racha = 1 rectángulo
    // TRIANGLE_STRIP con 4 vértices (xl,yb)-(xl,yt)-(xr,yb)-(xr,yt)
    // Cada vértice: pos(x,y,0) + color(r,g,b) => 6 floats
    // -------------------------------------------
    float vertices[franjas * 4 * 6];
    int   starts[franjas];
    int   counts[franjas];
    int v = 0;
    for (int i = 0; i < franjas; ++i) {
        int r = stripRow[i];
        int c0 = stripC0[i];
        int c1 = stripC1[i];
        int col = stripCol[i];

        float xl = Xc[c0] - DX * 0.5f;
        float xr = Xc[c1] + DX * 0.5f;
        float yb = Yc[r] - DY * 0.5f;
        float yt = Yc[r] + DY * 0.5f;

        float rr = CR[col], gg = CG[col], bb = CB[col];

        starts[i] = v;
        counts[i] = 4;

        float quad[4][6] = {
            { xl, yb, 0.0f, rr, gg, bb },
            { xl, yt, 0.0f, rr, gg, bb },
            { xr, yb, 0.0f, rr, gg, bb },
            { xr, yt, 0.0f, rr, gg, bb }
        };

        for (int k = 0; k < 4; ++k) {
            int base = v * 6;
            vertices[base + 0] = quad[k][0];
            vertices[base + 1] = quad[k][1];
            vertices[base + 2] = quad[k][2];
            vertices[base + 3] = quad[k][3];
            vertices[base + 4] = quad[k][4];
            vertices[base + 5] = quad[k][5];
            ++v;
        }
    }
    // v == NSTRIPS * 4

    // -------------------------------------------
    // VAO / VBO (no necesitamos EBO para TRIANGLE_STRIP)
    // -------------------------------------------
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Posición: layout(location=0) vec3 (x,y,z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Color: layout(location=1) vec3 (r,g,b)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // -------------------------------------------
    // Loop de render
    // -------------------------------------------
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.Use();
        glBindVertexArray(VAO);


        for (int i = 0; i < franjas; ++i) {
            glDrawArrays(GL_TRIANGLE_STRIP, starts[i], counts[i]);
        }

        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
