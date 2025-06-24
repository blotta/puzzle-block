#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include <SDL2/SDL.h>

struct MouseState
{
    int x;
    int y;
    uint8_t button[6] = {};
};

class Input
{
  public:
    Input(const Input&) = delete;

    static void Update(float dt);
    static bool QuitRequested();
    static bool Pressed(SDL_Scancode code);
    static bool JustPressed(SDL_Scancode code);
    static bool JustReleased(SDL_Scancode code);
    static bool MousePressed(uint8_t btn);
    static bool MouseJustPressed(uint8_t btn);
    static bool MouseJustReleased(uint8_t btn);
    static void MousePosition(int* x, int* y);

  private:
    Input();
    static Input& get();

    uint8_t mKeyState[SDL_NUM_SCANCODES] = {};
    MouseState mMouse = {};
    bool mQuitEvent = false;
};

#endif