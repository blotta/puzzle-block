#include <SDL2/SDL.h>

#include "scene.hpp"

Scene::~Scene()
{
}
void Scene::init()
{
    SDL_Log("Loading scene\n");
}
void Scene::dispose()
{
    SDL_Log("unloading scene\n");
};