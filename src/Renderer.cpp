#include "Renderer.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath> // tan

Renderer::~Renderer()
{
    std::cout << "Renderer:: Cleaning up renderer...\n";

    glDeleteVertexArrays(1, &mLineVAO);
    glDeleteBuffers(1, &mLineVBO);

    // delete shader
    glDeleteProgram(mShader);
}

void Renderer::Init(int screenWidth, int screenHeight)
{
    // shader stuff
    CompileShaderProgram(
        &mShader,
        "#version 330 core\n"
        "layout (location = 0) in vec2 vertex;\n"
        "uniform mat4 projection;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
        "}\0",
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "uniform vec4 color;\n"
        "void main()\n"
        "{\n"
        "   FragColor = color;\n"
        "}\0"
    );
    
    // set projection matrix
    // if/when the view gets resized this will need to be updated
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(screenWidth), static_cast<float>(screenHeight), 0.0f);
    glUseProgram(mShader);
    glUniformMatrix4fv(glGetUniformLocation(mShader, "projection"), 1, false, glm::value_ptr(projection));

    // create the buffers that will be used for rendering text
    glGenVertexArrays(1, &mLineVAO);
    glGenBuffers(1, &mLineVBO);
    glBindVertexArray(mLineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mLineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 2, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::DrawLine(glm::vec2 p0, glm::vec2 p1, glm::vec3 color)
{
    glUseProgram(mShader);

    // set color
    glUniform4f(glGetUniformLocation(mShader, "color"), color.r, color.g, color.b, 1.f);

    // bind
    glBindVertexArray(mLineVAO);


    float vertices[4] = { 
        // xy
        p0.x, p0.y,
        p1.x, p1.y,
    };

    glBindBuffer(GL_ARRAY_BUFFER, mLineVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // draw
    glDrawArrays(GL_LINES, 0, 2);

    glBindVertexArray(0);
}

void Renderer::DrawBox(glm::vec2 pos, glm::vec2 size, glm::vec3 color)
{
    glm::vec2 p0 = glm::vec2{ pos.x - (size.x / 2.0f), pos.y + (size.y / 2.0f) };
    glm::vec2 p1 = glm::vec2{ pos.x - (size.x / 2.0f), pos.y - (size.y / 2.0f) };
    glm::vec2 p2 = glm::vec2{ pos.x + (size.x / 2.0f), pos.y - (size.y / 2.0f) };
    glm::vec2 p3 = glm::vec2{ pos.x + (size.x / 2.0f), pos.y + (size.y / 2.0f) };

    DrawLine(p0, p1, color);
    DrawLine(p1, p2, color);
    DrawLine(p2, p3, color);
    DrawLine(p3, p0, color);
}

void Renderer::DrawCircle(glm::vec2 pos, float radius, glm::vec3 color)
{
    const int segements = 96;

    float theta = 3.14159265f * 2 / static_cast<float>(segements);
    float tanFactor = tan(theta);
    float radialFactor = cos(theta);
    float x = radius;
    float y = 0;

    for (int i = 0; i < segements; i++) {
        glm::vec2 p0 = glm::vec2{ x + pos.x, y + pos.y };
        
        float tx = -y;
        float ty = x;

        x += tx * tanFactor;
        y += ty * tanFactor;

        x *= radialFactor;
        y *= radialFactor;

        glm::vec2 p1 = glm::vec2{ x + pos.x, y + pos.y };
        
        DrawLine(p0, p1, color);
    }
}

void Renderer::CompileShaderProgram(GLuint* shader, const char* vertexSrc, const char* fragmentSrc)
{
    GLuint sVertex, sFragment;
    
    // vertex shader
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexSrc, 0);
    glCompileShader(sVertex);
    CheckShaderCompilationError(sVertex);

    // fragment shader
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragmentSrc, 0);
    glCompileShader(sFragment);
    CheckShaderCompilationError(sFragment);

    // link to shader program
    *shader = glCreateProgram();
    glAttachShader(*shader, sVertex);
    glAttachShader(*shader, sFragment);
    glLinkProgram(*shader);
    
    // error check linking
    int success;
    char infoLog[512];
    glGetProgramiv(*shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(*shader, 512, NULL, infoLog);
        printf("Failed to link shader:\n%s\n", infoLog);
    }

    // the vertex and fragment shader are no longer used after they have been linked into the shader program
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
}

void Renderer::CheckShaderCompilationError(GLuint id)
{
    int success;
    char infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        printf("Failed to compile shader:\n%s\n", infoLog);
    }
}
