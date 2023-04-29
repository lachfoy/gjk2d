#ifndef GAME_H_
#define GAME_H_

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <SDL2/SDL_opengl.h>

#include "Input.h"
#include "Renderer.h"
#include "TextRenderer.h"
#include "Font.h"

#include "Physics.h"

class Game
{
public:
    Game() {};

    bool Init();
    void Run();

private:
    void Create();
    void HandleInput();
    void Update(float deltaTime);
    void Draw();
    void Destroy();
    
    SDL_Window* mWindow;
    SDL_GLContext mContext;

    bool mRunning = true;

    Input mInput;
    Renderer mRenderer;
    TextRenderer mTextRenderer;
    Font mFont;

    int mMousePosition[2];

    Shape mShapeA;
    Shape mShapeB;

    bool mCollision;

    glm::vec3 mBlue = { 75.0f / 255, 77.0f / 255, 227.0f / 255 };
    glm::vec3 mRed = { 227.0f / 255, 75.0f / 255, 75.0f / 255 };
    glm::vec3 mCyan = { 75.0f / 255, 227.0f / 255, 224.0f / 255 };
};

#endif