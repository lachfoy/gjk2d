#ifndef RENDERER_H_
#define RENDERER_H_

#include <glm/glm.hpp>
#include <glad/glad.h>

class Renderer
{
public:
    Renderer() = default;
    ~Renderer();

    void Init(int screenWidth, int screenHeight);
    void DrawLine(glm::vec2 p0, glm::vec2 p1, glm::vec3 color = glm::vec3(1));
    void DrawBox(glm::vec2 pos, glm::vec2 size, glm::vec3 color = glm::vec3(1));
    void DrawCircle(glm::vec2 pos, float radius, glm::vec3 color = glm::vec3(1));

private:
    GLuint mShader;
    GLuint mLineVBO;
    GLuint mLineVAO;

    void CompileShaderProgram(GLuint* shader, const char* vertexSrc, const char* fragmentSrc);
    void CheckShaderCompilationError(GLuint id);
    
};

#endif