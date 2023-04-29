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
};

#endif