#include "scene.hpp"
#include "log.hpp"
#include <SDL2/SDL.h>

Scene::~Scene()
{
}
void Scene::init()
{
    Log::info("Loading scene\n");
}
void Scene::dispose()
{
    Log::info("unloading scene\n");
};