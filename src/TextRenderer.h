#ifndef TEXT_RENDERER_H_
#define TEXT_RENDERER_H_

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <map>
#include <string>

#include "Font.h"

class TextRenderer
{
public:
    TextRenderer() = default;
    ~TextRenderer();

    void Init(int screenWidth, int screenHeight);
    void DrawText(std::string text, float x, float y, Font& font, float scale = 1.0f);
    void ResizeRenderer(int newWidth, int newHeight);

private:
    GLuint mShader;
    GLuint mTextVBO;
    GLuint mTextVAO;

    void CompileShaderProgram(GLuint* shader, const char* vertexSrc, const char* fragmentSrc);
    void CheckShaderCompilationError(GLuint id);
    
};

#endif