#include "component_gui.hpp"
#include "game.hpp"
#include "input_manager.hpp"

Widget::Widget(int x, int y, int w, int h) // : x(x), y(y), width(w), height(h)
{
    this->rect = {x, y, w, h};
}

void Widget::update(float dt)
{
}

///////////
// LABEL //
///////////

Label::Label(int x, int y, int w, int h, std::string t) : Widget(x, y, w, h), text(std::move(t))
{
    auto size = Game::TextSize(text);
    if (w <= 0)
        rect.w = size.x + 10;
    if (h <= 0)
        rect.h = size.y + 5;
}

void Label::draw()
{
    SDL_Color color = {180, 180, 180, 255};
    Game::Text(rect.x, rect.y, text, {.color = color});
}

////////////
// BUTTON //
////////////

Button::Button(int x, int y, int w, int h, std::string cap) : Widget(x, y, w, h), caption(std::move(cap))
{
    auto size = Game::TextSize(caption);
    if (w <= 0)
        rect.w = size.x + 10;
    if (h <= 0)
        rect.h = size.y + 5;
}

void Button::draw()
{
    SDL_Color color = {180, 180, 180, 255};
    SDL_Color bgColor = {30, 30, 30, 255};
    if (state == WidgetState::HOVER)
    {
        color = {255, 255, 255, 255};
        bgColor = {50, 50, 50, 255};
    }
    else if (state == WidgetState::CLICK)
    {
        color = {100, 100, 100, 255};
        bgColor = {20, 20, 20, 255};
    }

    Game::DrawFilledRect(rect.x, rect.y, rect.w, rect.h, bgColor);
    Game::DrawRect(rect.x, rect.y, rect.w, rect.h, color);
    auto center = rect.center();
    Game::Text(center.x, center.y, caption,
               {.color = color, .align = TextAlign::CENTER, .valign = VerticalAlign::MIDDLE});
}

///////////////////
// GUI COMPONENT //
///////////////////

void GuiComponent::init()
{
    transform = owner->getComponent<CTransform>();
}

void GuiComponent::update(float dt)
{
    vec2 mousePos;
    Input::MousePosition(&mousePos.x, &mousePos.y);

    bool hoverEventHandled = false;
    bool clickEventHandled = true;

    if (Input::MousePressed(SDL_BUTTON_LEFT))
        clickEventHandled = false;

    // Iterate backwards so elements on top handle the event first
    for (auto it = children.rbegin(); it != children.rend(); ++it)
    {
        Widget* w = (*it).get();

        if (false == hoverEventHandled && w->rect.contains(mousePos.x, mousePos.y))
        {
            // hovering widget
            hoverEventHandled = true;
            if (w->state == WidgetState::NORMAL)
            {
                w->state = WidgetState::HOVER;
                if (w->onHover)
                    w->onHover();
            }

            if (false == clickEventHandled)
            {
                // clicked on hovered widget
                clickEventHandled = true;
                if (w->state == WidgetState::HOVER && w->onClick)
                {
                    w->onClick();
                }
                w->state = WidgetState::CLICK;
            }
            else if (w->state == WidgetState::CLICK)
            {
                w->state = WidgetState::HOVER;
            }
        }
        else
        {
            w->state = WidgetState::NORMAL;
        }
    }

    for (auto& c : children)
    {
        if (c->visible)
            c->update(dt);
    }
}

void GuiComponent::drawGUI()
{
    for (auto& c : children)
    {
        if (c->visible)
            c->draw();
    }
}
