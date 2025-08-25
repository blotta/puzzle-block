#include "entity.hpp"

void Component::init()
{
}
void Component::update(float dt)
{
}
void Component::draw()
{
}
void Component::onEnable()
{
}
void Component::onDisable()
{
}
void Component::onDestroy()
{
}

/////////////////////////
// Transform Component //
/////////////////////////
CTransform::CTransform(const vec2f& p) : pos(p)
{
}