#include "input_manager.hpp"
#include "log.hpp"

Input::Input()
{
    Log::info("Input initialized\n");
}

Input& Input::get()
{
    static Input instance;
    return instance;
}

void Input::Update(float dt)
{
    auto& mgr = Input::get();
    // 0 not pressed
    // 1 pressed
    // 2 just pressed
    // 3 just released

    for (uint16_t i = 0; i < SDL_NUM_SCANCODES; i++)
    {
        // just pressed becomes pressed
        if (mgr.mKeyState[i] == 2)
            mgr.mKeyState[i] = 1;

        // just released becomes not pressed
        if (mgr.mKeyState[i] == 3)
            mgr.mKeyState[i] = 0;
    }

    for (uint8_t i = 0; i < 6; i++)
    {
        if (mgr.mMouse.button[i] == 2)
            mgr.mMouse.button[i] = 1;
        if (mgr.mMouse.button[i] == 3)
            mgr.mMouse.button[i] = 0;
    }

    SDL_GetMouseState(&mgr.mMouse.x, &mgr.mMouse.y);

    mgr.mMouse.moved = false;

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT: {
            mgr.mQuitEvent = true;
            break;
        }
        case SDL_KEYDOWN: {
            if (mgr.mKeyState[event.key.keysym.scancode] == 0)
                mgr.mKeyState[event.key.keysym.scancode] = 2; // just pressed
        }
        break;
        case SDL_KEYUP: {
            mgr.mKeyState[event.key.keysym.scancode] = 3; // just released
        }
        break;
        case SDL_MOUSEBUTTONDOWN: {
            mgr.mMouse.button[event.button.button] = 2;
        }
        break;
        case SDL_MOUSEBUTTONUP: {
            mgr.mMouse.button[event.button.button] = 3;
        }
        break;
        case SDL_MOUSEMOTION: {
            mgr.mMouse.moved = true;
        }
        break;
        }
    }
}

bool Input::QuitRequested()
{
    return Input::get().mQuitEvent;
}

bool Input::Pressed(SDL_Scancode code)
{
    uint8_t k = Input::get().mKeyState[code];
    return k == 1 || k == 2;
}

bool Input::JustPressed(SDL_Scancode code)
{
    uint8_t k = Input::get().mKeyState[code];
    return k == 2;
}

bool Input::JustReleased(SDL_Scancode code)
{
    uint8_t k = Input::get().mKeyState[code];
    return k == 3;
}

bool Input::MousePressed(uint8_t btn)
{
    uint8_t b = Input::get().mMouse.button[btn];
    return b == 2 || b == 1;
}

bool Input::MouseJustPressed(uint8_t btn)
{
    uint8_t b = Input::get().mMouse.button[btn];
    return b == 2;
}

bool Input::MouseJustReleased(uint8_t btn)
{
    uint8_t b = Input::get().mMouse.button[btn];
    return b == 3;
}

void Input::MousePosition(int* x, int* y)
{
    *x = Input::get().mMouse.x;
    *y = Input::get().mMouse.y;
}

bool Input::MouseMoved()
{
    return Input::get().mMouse.moved;
}
