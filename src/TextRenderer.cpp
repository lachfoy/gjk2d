#include "TextRenderer.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

TextRenderer::~TextRenderer()
{
    std::cout << "TextRenderer:: Cleaning up text renderer...\n";

    glDeleteVertexArrays(1, &mTextVAO);
    glDeleteBuffers(1, &mTextVBO);

    // delete shaders
    glDeleteProgram(mShader);
}

void TextRenderer::Init(int screenWidth, int screenHeight)
{    
    // create shader program
    CompileShaderProgram(
        &mShader,
        "#version 330 core\n"
        "layout (location = 0) in vec4 vertex;\n"
        "out vec2 TexCoords;\n"
        "uniform mat4 projection;\n"
        "void main()\n"
        "{\n"
        "   TexCoords = vertex.zw;\n"
        "   gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
        "}\0",
        "#version 330 core\n"
        "in vec2 TexCoords;\n"
        "out vec4 FragColor;\n"
        "uniform sampler2D image;\n"
        "uniform vec4 color;\n"
        "void main()\n"
        "{\n"
        "   vec4 texColor = color * texture(image, TexCoords);\n"
        "   FragColor = texColor;\n"
        "}\0"
    );

    // set projection matrix
    // if/when the view gets resized this will need to be updated
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(screenWidth), static_cast<float>(screenHeight), 0.0f);
    glUseProgram(mShader);
    glUniformMatrix4fv(glGetUniformLocation(mShader, "projection"), 1, false, glm::value_ptr(projection));

    // create the buffers that will be used for rendering text
    glGenVertexArrays(1, &mTextVAO);
    glGenBuffers(1, &mTextVBO);
    glBindVertexArray(mTextVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mTextVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TextRenderer::DrawText(std::string text, float x, float y, Font& font, float scale)
{
    glUseProgram(mShader);

    // set color
    glUniform4f(glGetUniformLocation(mShader, "color"), 1.f, 1.f, 1.f, 1.f);

    // bind
    glBindTexture(GL_TEXTURE_2D, font.textureHandle);
    glBindVertexArray(mTextVAO);

    // iterate through all characters
    float startX = x;
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        if (*c == '\n') {
            y += font.lineHeight * scale;
            x = startX;
        } else {
            CharInfo charInfo = font.chars[*c];

            float posx = x + charInfo.xoffset * scale;
            float posy = y + charInfo.yoffset * scale;
            float w = charInfo.width * scale;
            float h = charInfo.height * scale;

            // ccw winding order
            float vertices[24] = { 
                // xy                       // tex coords
                posx, posy + h,             charInfo.texx0, charInfo.texy1, // bottom left
                posx + w, posy,             charInfo.texx1, charInfo.texy0, // top right
                posx, posy,                 charInfo.texx0, charInfo.texy0, // top left

                posx, posy + h,             charInfo.texx0, charInfo.texy1, // bottom left
                posx + w, posy + h,         charInfo.texx1, charInfo.texy1, // bottom right
                posx + w, posy,             charInfo.texx1, charInfo.texy0  // top right
            };

            glBindBuffer(GL_ARRAY_BUFFER, mTextVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // draw
            glDrawArrays(GL_TRIANGLES, 0, 6);

            x += charInfo.advance * scale;
        }
    }
    
    glBindVertexArray(0);
}

void TextRenderer::ResizeRenderer(int newWidth, int newHeight)
{
    // idk if this is needed?
    glViewport(0, 0, newWidth, newHeight);

    // update the shader with the new width and height
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(newWidth), static_cast<float>(newHeight), 0.0f);
    glUseProgram(mShader);
    glUniformMatrix4fv(glGetUniformLocation(mShader, "projection"), 1, false, glm::value_ptr(projection));
}

void TextRenderer::CompileShaderProgram(GLuint* shader, const char* vertexSrc, const char* fragmentSrc)
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

void TextRenderer::CheckShaderCompilationError(GLuint id)
{
    int success;
    char infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        printf("Failed to compile shader:\n%s\n", infoLog);
        //assert(false, "Failed to compile shader" + infoLog);
    }
}
