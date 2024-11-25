#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include <SDL2/SDL.h>

class InputManager
{
public:
    void update(float dt);
    bool pressed(SDL_Scancode code);
    bool just_pressed(SDL_Scancode code);
    bool just_released(SDL_Scancode code);
    bool quit_requested();
private:
    uint8_t mKeyState[SDL_NUM_SCANCODES] = {};
    bool mQuitEvent = false;
};
#endif