#ifndef FONT_H_
#define FONT_H_

#include <unordered_map>
#include <glad/glad.h>

#include "CharInfo.h"

class Font
{
public:
    Font() = default;
    ~Font();

    void Load(const char* path);

    GLuint textureHandle;

    unsigned short lineHeight;
    std::unordered_map<unsigned int, CharInfo> chars;

};

#endif