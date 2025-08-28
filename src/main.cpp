#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;

// Игрок
struct Player {
    glm::vec2 pos = {0.0f, 0.0f};
    glm::vec2 size = {0.1f, 0.1f};
    glm::vec2 velocity = {0.0f, 0.0f};
};

// Земля
struct Ground {
    glm::vec2 pos = {0.0f, -0.5f};
    glm::vec2 size = {2.0f, 0.2f};
};

// Шейдеры
const char* vertexShaderSrc = R"(
#version 460 core
layout(location = 0) in vec2 aPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main() {
    gl_Position = projection * view * model * vec4(aPos, 0.0, 1.0);
}
)";

const char* fragmentShaderSrc = R"(
#version 460 core
uniform vec3 color;
out vec4 FragColor;
void main() {
    FragColor = vec4(color, 1.0);
}
)";

// Функция компиляции шейдера
GLuint compileShader(GLenum type, const char* src){
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success){
        char info[512];
        glGetShaderInfoLog(shader, 512, nullptr, info);
        std::cerr << "Shader compile error: " << info << std::endl;
    }
    return shader;
}

// Создание шейдерной программы
GLuint createShaderProgram(){
    GLuint vertex = compileShader(GL_VERTEX_SHADER, vertexShaderSrc);
    GLuint fragment = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return program;
}

int main() {
    if(!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Mini Platformer", nullptr, nullptr);
    if(!window){ glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    glViewport(0,0,WIDTH,HEIGHT);
    glClearColor(0.2f, 0.2f, 0.0f, 1.0f); // темно-желтый фон

    GLuint shaderProgram = createShaderProgram();
    glUseProgram(shaderProgram);

    // Координаты квадрата (0,0 center, 1x1)
    float quadVertices[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };
    unsigned int quadIndices[] = {
        0,1,2,
        0,2,3
    };

    GLuint VAO,VBO,EBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(quadVertices),quadVertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(quadIndices),quadIndices,GL_STATIC_DRAW);

    glBindVertexArray(0);

    // Проекция ортографическая
    float zoom = 1.0f; // больше — ближе



    

    Player player = {{8.0f, 5.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}};
    Ground ground = {{0.0f, 0.0f}, {16.0f, 1.0f}};


    const float gravity = -0.01f;

    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 projection = glm::ortho(0.0f, 16.0f / zoom, 0.0f, 9.0f / zoom, -1.0f, 1.0f);
        GLuint projLoc = glGetUniformLocation(shaderProgram,"projection");
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(projLoc,1,GL_FALSE,glm::value_ptr(projection));

        // Управление
        if(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) zoom += 0.05f;
        if(glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) zoom -= 0.05f;
        if(zoom < 0.1f) zoom = 0.1f;
        if(zoom > 5.0f) zoom = 5.0f;

        if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) player.pos.x -= 0.1f;
        if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) player.pos.x += 0.1f;
        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
            // прыжок если на земле
            if(player.pos.y <= ground.pos.y + ground.size.y) player.velocity.y = 0.1f;
        }

        // Гравитация
        player.velocity.y += gravity;
        player.pos.y += player.velocity.y;

        // Коллизия с землей
        if(player.pos.y <= ground.pos.y + ground.size.y){
            player.pos.y = ground.pos.y + ground.size.y;
            player.velocity.y = 0.0f;
        }

        glBindVertexArray(VAO);

        glm::vec2 cameraPos;
        cameraPos.x = player.pos.x - (8.0f / zoom); // половина ширины видимой области / zoom
        cameraPos.y = player.pos.y - (4.5f / zoom); // половина высоты видимой области / zoom
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraPos.x, -cameraPos.y, 0.0f));



        GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(ground.pos.x, ground.pos.y,0.0f));
        model = glm::scale(model, glm::vec3(ground.size.x, ground.size.y,1.0f));
        GLuint modelLoc = glGetUniformLocation(shaderProgram,"model");
        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
        GLuint colorLoc = glGetUniformLocation(shaderProgram,"color");
        glUniform3f(colorLoc,0.0f,1.0f,0.0f); // зеленая
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

        // Игрок
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(player.pos.x, player.pos.y,0.0f));
        model = glm::scale(model, glm::vec3(player.size.x, player.size.y,1.0f));
        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
        glUniform3f(colorLoc,0.0f,0.0f,1.0f); // синий
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
