#include "scene.hpp"
#include "log.hpp"
#include <SDL.h>

Scene::~Scene()
{
}
void Scene::preload()
{
    Log::info("scene preload");
}
void Scene::init()
{
    Log::info("scene init");
}
void Scene::update(float dt)
{
}
void Scene::draw()
{
}
void Scene::drawGUI()
{
}
void Scene::dispose()
{
    Log::info("scene dispose");
}
void Scene::onPopReturn() {};
