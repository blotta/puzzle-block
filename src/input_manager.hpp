#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include <SDL2/SDL.h>

struct MouseState
{
    int x;
    int y;
    uint8_t button[6] = {};
};

class InputManager
{
public:
    void update(float dt);
    bool pressed(SDL_Scancode code);
    bool just_pressed(SDL_Scancode code);
    bool just_released(SDL_Scancode code);
    bool mouse_pressed(uint8_t btn);
    bool mouse_just_pressed(uint8_t btn);
    bool mouse_just_released(uint8_t btn);
    void mouse_position(int* x, int* y);
    bool quit_requested();
private:
    uint8_t mKeyState[SDL_NUM_SCANCODES] = {};
    MouseState mMouse = {};
    bool mQuitEvent = false;
};
#endif