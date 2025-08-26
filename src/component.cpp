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
void Component::drawGUI()
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
CTransform::CTransform()
{
}
CTransform::CTransform(const vec2f& p) : pos(p)
{
}