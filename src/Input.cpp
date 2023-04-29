#include "Input.h"

#include <iostream>

Input::Input()
{
    // find the number of keys
    SDL_GetKeyboardState(&mNumKeys);
    mKeyboardState = new bool[mNumKeys];
    mKeyboardStateOld = new bool[mNumKeys];

    // intialize the states as false
    for (int i = 0; i < mNumKeys; i++) {
        mKeyboardState[i] = false;
        mKeyboardStateOld[i] = false;
    }
}

Input::~Input()
{
    std::cout << "Input::Destructor called!\n";
    // delete arrays
    delete[] mKeyboardState;
    delete[] mKeyboardStateOld;
}

void Input::SetKeyDown(Uint8 key)
{
    mKeyboardState[key] = true;
}

void Input::SetKeyUp(Uint8 key)
{
    mKeyboardState[key] = false;
}

bool Input::IsKeyPressed(Uint8 key)
{
    return mKeyboardState[key] && !mKeyboardStateOld[key];
}

bool Input::IsKeyHeld(Uint8 key)
{
    return mKeyboardState[key];
}

void Input::UpdateInput()
{
    // update last state
    for (int i = 0; i < mNumKeys; i++) {
        mKeyboardStateOld[i] = mKeyboardState[i];
    }
}