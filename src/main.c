#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <cglm/cglm.h>
#include <stdlib.h>

char* load_file(const char* path)
{
    FILE* f = fopen(path, "rb");
    if (!f) {
        printf("cannot open file %s\n", path);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char* buffer = malloc(size + 1);
    fread(buffer, 1, size, f);

    buffer[size] = 0;

    fclose(f);
    return buffer;
}

int main(void)
{
    if (!glfwInit()){
        printf("GLFW init failed\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window =
        glfwCreateWindow(800, 600, "Fucking C Game", NULL, NULL);

    if (!window){
        printf("Window is fucking gone\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        printf("GLAD is failed as fuck\n");
        return -1;
    }
    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);

    char* vertexShaderSource = load_file("src/shaders/vertex.glsl");
    char* fragmentShaderSource = load_file("src/shaders/fragment.glsl");

    float vertices[] = {
        -3.0f, -1.0f, 2.0f,
         1.0f, -1.0f, 0.0f,
         0.0f,  1.0f, 0.0f
    };

    //VAO-shit
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //VBO-shit
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(vertices),
                 vertices,
                 GL_STATIC_DRAW);

    //vertex struct shit
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3*sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    //unbind VAO shit
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //shaders compile
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const char**)&vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("Vertex shader compilation failed:\n%s\n", infoLog);
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const char**)&fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("Shader program linking failed:\n%s\n", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // getting uniform var location (in shader)
    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");

    //matrix inits
    mat4 model;
    mat4 view;
    mat4 projection;

    glm_mat4_identity(model);
    glm_mat4_identity(view);
    glm_mat4_identity(projection);

   //perspective
    glm_perspective(
        glm_rad(45.0f),
        800.0f/600.0f,
        0.1f,
        100.0f,
        projection
    );

    //camera
    vec3 eye    = {0.0f,0.0f,3.0f};
    vec3 center = {0.0f,0.0f,0.0f};
    vec3 up     = {0.0f,1.0f,0.0f};
    glm_lookat(eye, center, up, view);


    while (!glfwWindowShouldClose(window)){
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        // Перемножаем матрицы: projection * view * model
        mat4 pv;
        mat4 transform;

        glm_mat4_mul(projection, view, pv);
        glm_mat4_mul(pv, model, transform);
            // Передаем в шейдер
        glUniformMatrix4fv(transformLoc,1,GL_FALSE,(float*)transform);
        glDrawArrays(GL_TRIANGLES,0,3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
}
