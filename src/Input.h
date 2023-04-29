#ifndef INPUT_H_
#define INPUT_H_

#include <SDL2/SDL.h>
#include <iostream>

class Input
{
public:
    Input();
    ~Input();

    //void HandleEvents();

    void SetKeyDown(Uint8 key);
    void SetKeyUp(Uint8 key);

    bool IsKeyPressed(Uint8 key);
    bool IsKeyHeld(Uint8 key);

    // must be called after update
    void UpdateInput();

private:
    int mNumKeys;
    bool *mKeyboardState;
    bool *mKeyboardStateOld;

};

#endif 