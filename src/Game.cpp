#include "Game.h"

#include <cstdio>
#include <iostream>
#include <cmath> // floor

#include "Colors.h"
#include <glm/gtx/matrix_transform_2d.hpp>

bool Game::Init()
{
    // initialize SDL video
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL video could not be initialized: %s\n", SDL_GetError());
        return false;
    }

    // create main window
    mWindow = SDL_CreateWindow(
        "test",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );
    if (!mWindow) {
        printf("SDL window could not be created: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    // initialize renderering context
    //Use OpenGL 3.3 core // should be ok for anything about a decade old?
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // double buffering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    mContext = SDL_GL_CreateContext(mWindow);
    if (!mContext) {
        printf("GL Context could not be created: %s\n", SDL_GetError());
        SDL_DestroyWindow(mWindow);
        SDL_Quit();
        return false;
    }

    // initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        printf("GLAD could not be loaded\n");
        SDL_GL_DeleteContext(mContext);
        SDL_DestroyWindow(mWindow);
        SDL_Quit();
        return false;
    }

    // enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // this functionality exists i guess?
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(3);
    
    return true;
}

void Game::Run()
{
    Create();

    Uint32 lastTime = 0;

    SDL_Event event;
    int fps = 0;
    int fpsCount = 0;
    float fpsInterval = 0.0f;

    // main loop
    while (mRunning) {
        // handle input events
        while (SDL_PollEvent(&event) != 0) {
            switch(event.type) {
            case SDL_QUIT:
                mRunning = false;
                break;
            case SDL_KEYDOWN:
                mInput.SetKeyDown(event.key.keysym.scancode);
                break;
            case SDL_KEYUP:
                mInput.SetKeyUp(event.key.keysym.scancode);
                break;
            case SDL_MOUSEMOTION:
                SDL_GetMouseState(&mMousePosition[0], &mMousePosition[1]);
            }
        }

        HandleInput();

        // update input
        mInput.UpdateInput();

        // calculate delta time
        Uint32 currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        
        // calculate fps
        fpsCount++;
        fpsInterval += deltaTime;
        if (fpsInterval > 1.0f) {
            fps = fpsCount;
            fpsCount = 0;
            fpsInterval = 0.0f;
        }

        // update
        Update(deltaTime);

        // clear screen
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        //glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw
        Draw();

        mTextRenderer.DrawText("FPS: " + std::to_string(fps) ,//+
            //"\nFrametime: " + std::to_string(deltaTime) +
            //"\nMouse pos: " + std::to_string(mMousePosition[0]) + ", " + std::to_string(mMousePosition[1]),
            4, 4, mFont, 2.0f);

        // swap buffers
        SDL_GL_SwapWindow(mWindow);
    }

    Destroy();
}

void Game::Create()
{
    mRenderer.Init(800, 600);
    mTextRenderer.Init(800, 600);
    mFont.Load("fonts/arial_16px.fnt");

    mShapeA = {};
    mShapeB = {};

    mShapeB.points = {
        glm::vec2(392.0f, 129.0f),
        glm::vec2(174.0f, 229.0f),
        glm::vec2(205.0f, 410.0f),
        glm::vec2(420.0f, 468.0f),
        glm::vec2(581.0f, 304.0f),
        glm::vec2(542.0f, 164.0f)
    };
}

void Game::HandleInput()
{
    if (mInput.IsKeyPressed(SDL_SCANCODE_ESCAPE)) mRunning = false; //esc quit
}

void Game::Update(float deltaTime)
{
    // make shape A a 100x100 box around the mouse pos
    mShapeA.points = {
        glm::vec2(mMousePosition[0] - 50.0f, mMousePosition[1] - 50.0f),
        glm::vec2(mMousePosition[0] - 50.0f, mMousePosition[1] + 50.0f),
        glm::vec2(mMousePosition[0] + 50.0f, mMousePosition[1] + 50.0f),
        glm::vec2(mMousePosition[0] + 50.0f, mMousePosition[1] - 50.0f)
    };

    mCollision = false;

    if (Gjk(mShapeA, mShapeB)) {
        mCollision = true;
    }
}

void Game::Draw()
{
    mShapeA.Draw(mRenderer, mCollision ? mRed : mCyan);
    mShapeB.Draw(mRenderer, mBlue);
}

void Game::Destroy()
{
    SDL_GL_DeleteContext(mContext);
    SDL_DestroyWindow(mWindow);
    
    SDL_Quit();
}