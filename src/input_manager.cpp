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

    for (uint8_t i = 0; i < 6; i++)
    {
        if (mMouse.button[i] == 2)
            mMouse.button[i] = 1;
        if (mMouse.button[i] == 3)
            mMouse.button[i] = 0;
    }

    SDL_GetMouseState(&mMouse.x, &mMouse.y);

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
            case SDL_MOUSEBUTTONDOWN:
            {
                mMouse.button[event.button.button] = 2;
            }
            break;
            case SDL_MOUSEBUTTONUP:
            {
                mMouse.button[event.button.button] = 3;
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

bool InputManager::mouse_pressed(uint8_t btn)
{
    uint8_t b = mMouse.button[btn];
    return b == 2 || b == 1;
}

bool InputManager::mouse_just_pressed(uint8_t btn)
{
    uint8_t b = mMouse.button[btn];
    return b == 2;
}

bool InputManager::mouse_just_released(uint8_t btn)
{
    uint8_t b = mMouse.button[btn];
    return b == 3;
}

void InputManager::mouse_position(int *x, int *y)
{
    *x = mMouse.x;
    *y = mMouse.y;
}

bool InputManager::quit_requested()
{
    return mQuitEvent;
}
