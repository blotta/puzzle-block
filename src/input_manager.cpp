#include "input_manager.hpp"

void InputManager::update(float dt)
{
    // 0 not pressed
    // 1 pressed
    // 2 just pressed
    // 3 just released

    for (uint16_t i = 0; i < SDL_NUM_SCANCODES; i++)
    {
        // just pressed becomes pressed
        if (mKeyState[i] == 2)
            mKeyState[i] = 1;

        // just released becomes not pressed
        if (mKeyState[i] == 3)
            mKeyState[i] = 0;
    }

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
            {
                mQuitEvent = true;
                break;
            }
            case SDL_KEYDOWN:
            {
                mKeyState[event.key.keysym.scancode] = 2; // just pressed
            }
            break;
            case SDL_KEYUP:
            {
                mKeyState[event.key.keysym.scancode] = 3; // just released
            }
            break;
        }
    }
}

bool InputManager::pressed(SDL_Scancode code)
{
    uint8_t k = mKeyState[code];
    return k == 1 || k == 2;
}

bool InputManager::just_pressed(SDL_Scancode code)
{
    uint8_t k = mKeyState[code];
    return k == 2;
}

bool InputManager::just_released(SDL_Scancode code)
{
    uint8_t k = mKeyState[code];
    return k == 3;
}

bool InputManager::quit_requested()
{
    return mQuitEvent;
}
