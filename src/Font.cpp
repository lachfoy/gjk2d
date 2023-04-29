#include "Font.h"

#include <iostream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Font::~Font()
{
    std::cout << "Font:: Destructor called\n";

    glDeleteTextures(1, &textureHandle);

    chars.clear();
}

void Font::Load(const char* path)
{
    std::streampos size;
    std::ifstream myfile;
    myfile.open(path, std::ios::in | std::ios::binary | std::ios::ate); //, std::ios::in | std::ios::binary
    if (myfile.is_open()) {
        size = myfile.tellg(); // make sure std::ios::ate is set, so the position is at the end of the file
        myfile.seekg(0, std::ios::beg); // set position to start of file

        // get file identifier and version
        char fileIdentifier[3];
        myfile.read((char*)&fileIdentifier[0], 3); // should be "BMF"

        char version;
        myfile.read((char*)&version, 1);
        if (version != 3) {
            std::cout << "Error loading .fnt file: format version must be 3.\n";
        }

        char blockTypeIdentifier;
        int blockSize; 

        // first block (info) 
        myfile.read((char*)&blockTypeIdentifier, 1);
        myfile.read((char*)&blockSize, 4);
        //std::cout << "First block type, size: " << (int)blockTypeIdentifier << ", " << blockSize << "\n";
        
        char* infoBlock = new char[blockSize];
        myfile.read(infoBlock, blockSize);

        delete[] infoBlock; // Most of the time this block can simply be ignored.

        // second block (common)
        myfile.read((char*)&blockTypeIdentifier, 1);
        myfile.read((char*)&blockSize, 4);
        //std::cout << "Second block type, size: " << (int)blockTypeIdentifier << ", " << blockSize << "\n";

        char* commonBlock = new char[blockSize];
        myfile.read(commonBlock, blockSize);

        lineHeight = commonBlock[0];

        delete[] commonBlock; // todo :: read baseline and other info?

        // third block (pages)
        myfile.read((char*)&blockTypeIdentifier, 1);
        myfile.read((char*)&blockSize, 4);
        //std::cout << "Third block type, size: " << (int)blockTypeIdentifier << ", " << blockSize << "\n";

        char* pagesBlock = new char[blockSize];
        myfile.read(pagesBlock, blockSize);

        std::string texturePath = "fonts/";
        texturePath += (const char*)pagesBlock;

        // load data
        int w, h, nChannels;
        unsigned char* data = stbi_load(texturePath.c_str(), &w, &h, &nChannels, 0);

        // create texture
        glGenTextures(1, &textureHandle);
        glBindTexture(GL_TEXTURE_2D, textureHandle);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        
        // set texture wrap and filter modes
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);

        // free data
        stbi_image_free(data);

        std::cout << "Font:: Created texture from " << (const char*)pagesBlock << " with id " << textureHandle << std::endl;

        delete[] pagesBlock;

        // fourth block (chars)
        myfile.read((char*)&blockTypeIdentifier, 1);
        myfile.read((char*)&blockSize, 4);
        //std::cout << "Fourth block type, size: " << (int)blockTypeIdentifier << ", " << blockSize << "\n";
        
        int numChars = blockSize / 20;
        //std::cout << "Num characters in block: " << numChars << "\n";

        for (int i = 0; i < numChars; i++) {
            char* charBlock = new char[20];
            myfile.read(charBlock, 20);
            
            unsigned int charId; 
            CharInfo charInfo;

            charId = charBlock[0];
            charInfo.texx0 = charBlock[4] * (1.0f / w);
            charInfo.texy0 = charBlock[6] * (1.0f / h);
            charInfo.texx1 = charInfo.texx0 + charBlock[8] * (1.0f / w);
            charInfo.texy1 = charInfo.texy0 + charBlock[10] * (1.0f / h);
            charInfo.width = charBlock[8];
            charInfo.height = charBlock[10];
            charInfo.xoffset = charBlock[12];
            charInfo.yoffset = charBlock[14];
            charInfo.advance = charBlock[16];

            //std::cout << "Char " << i << " id: " << (char)charId << "\n";
            chars.insert({ charId, charInfo });
            delete[] charBlock;
        }

        // ignore the fifth block (kerning pairs)

        myfile.close();
    }
    

}
