#include "component_gui.hpp"
#include "game.hpp"
#include "input_manager.hpp"
#include "log.hpp"

Widget::Widget(int x, int y, int w, int h) : rectInit(x, y, w, h), rect(x, y, w, h)
{
}

void Widget::update(float dt)
{
}

bool Widget::isOnDragHandle(const vec2& pos) const
{
    return rect.contains(pos);
}

bool Widget::handleEvent(const GuiEvent& e)
{
    if (!visible)
        return false;

    switch (e.type)
    {
    case GuiEventType::MouseMove: {
        if (rect.contains(e.mousePos))
        {
            if (!hovered)
            {
                hovered = true;
                onMouseEnter();
            }
            return true; // hovered
        }
        else if (hovered)
        {
            hovered = false;
            onMouseExit();
        }
        break;
    }

    case GuiEventType::MouseDown: {
        if (rect.contains(e.mousePos))
        {
            onMouseDown(e.mousePos);
            return true;
        }
        break;
    }

    case GuiEventType::MouseUp: {
        if (rect.contains(e.mousePos))
        {
            onMouseUp(e.mousePos);
            onClick();
            return true;
        }
        else
        {
            onMouseUp(e.mousePos);
        }
        break;
    }

    default:
        break;
    }

    return false;
}

void Widget::onMouseEnter()
{
}

void Widget::onMouseExit()
{
}

void Widget::onMouseDown(vec2 pos)
{
}

void Widget::onMouseUp(vec2 pos)
{
}

void Widget::onClick()
{
}

void Widget::onDragStart(vec2 pos)
{
}

void Widget::onDrag(vec2 pos)
{
}

void Widget::onDragEnd(vec2 pos)
{
}

///////////////
// CONTAINER //
///////////////

Container::Container() : Widget(0, 0, 0, 0)
{
}

Container::Container(int x, int y) : Widget(x, y, 0, 0)
{
    isContainer = true;
}

Container::Container(int x, int y, int w, int h) : Widget(x, y, w, h)
{
    isContainer = true;
}

void Container::draw()
{
    for (auto& c : children)
    {
        if (c->visible)
        {
            c->draw();
        }
    }
}

bool Container::handleEvent(const GuiEvent& e)
{
    // Pass event to children first (iterate backwards so top-most handles first)
    for (auto it = children.rbegin(); it != children.rend(); ++it)
    {
        Widget* w = it->get();
        if (!w->visible)
            continue;

        // If it's a container, recurse
        if (w->isContainer)
        {
            if (static_cast<Container*>(w)->handleEvent(e))
            {
                return true;
            }
        }
        else
        {
            if (w->handleEvent(e))
            {
                return true;
            }
        }
    }

    // If no child consumed it, let this container handle it itself
    return Widget::handleEvent(e);
}

void Container::update(float dt)
{
    for (auto& c : children)
    {
        if (c->visible)
            c->update(dt);
    }
}

void Container::arrange()
{
    for (auto& c : children)
    {
        c->rect.x = c->rectInit.x + this->rect.x;
        c->rect.y = c->rectInit.y + this->rect.y;
        Log::debug("%d, %d", c->rect.x, c->rect.y);
    }
}

///////////
// PANEL //
///////////

Panel::Panel(int x, int y, int w, int h) : Container(x, y, w, h)
{
    draggable = true;
}

void Panel::update(float dt)
{
    Container::update(dt);
}

void Panel::draw()
{
    SDL_Color headerBgColor = {40, 40, 80, 255};
    SDL_Color headerBorderColor = {100, 100, 100, 255};
    SDL_Color bodyBgColor = {40, 40, 40, 255};
    SDL_Color bodyBorderColor = {80, 80, 80, 255};

    // header
    if (headerHeight > 0)
    {
        SDL_Rect header = {rect.x, rect.y, rect.w, headerHeight};
        if (hoveringDragHandle)
            headerBgColor = {60, 60, 90, 255};
        Game::DrawFilledRect(header.x, header.y, header.w, header.h, headerBgColor);
        Game::DrawRect(header.x, header.y, header.w, header.h, headerBorderColor); // header border
        Game::Text(header.x + 10, header.y + 5, title);
    }

    // body
    SDL_Rect body = {rect.x, rect.y + headerHeight, rect.w, rect.h - headerHeight};
    Game::DrawFilledRect(body.x, body.y, body.w, body.h, bodyBgColor);
    Game::DrawRect(body.x, body.y, body.w, body.h, bodyBorderColor);

    // draw children
    Container::draw();
}

void Panel::arrange()
{
    for (auto& c : children)
    {
        c->rect.x = c->rectInit.x + this->rect.x;
        c->rect.y = c->rectInit.y + this->rect.y + headerHeight;
    }
}

bool Panel::isOnDragHandle(const vec2& pos) const
{
    Rect header = {rect.x, rect.y, rect.w, headerHeight};
    return header.contains(pos);
}

bool Panel::handleEvent(const GuiEvent& e)
{
    switch (e.type)
    {
    case GuiEventType::MouseDown:
        Log::debug("mouse down");
        if (isOnDragHandle(e.mousePos))
        {
            dragOffset = e.mousePos - vec2(rect.x, rect.y);
            dragStartPos = e.mousePos;
            validDragStart = true;
            dragging = false;
            return true;
        }
        break;

    case GuiEventType::MouseMove:
        Log::debug("mouse move; Dragging: %d", dragging);
        if ((e.mousePos - dragStartPos).length() > 5.0f && !dragging && validDragStart && isOnDragHandle(dragStartPos))
        {
            dragging = true;
            onDragStart(dragStartPos);
        }
        if (dragging)
        {
            Rect header = rect;
            header.h = headerHeight;
            header.x = e.mousePos.x - dragOffset.x;
            header.y = e.mousePos.y - dragOffset.y;
            header.clampInside(parent->rect);
            rect.x = header.x;
            rect.y = header.y;

            arrange();
            onDrag(e.mousePos);
            return true;
        }
        hoveringDragHandle = isOnDragHandle(e.mousePos);
        break;

    case GuiEventType::MouseUp:
        if (dragging)
        {
            Log::debug("mouse up & drag");
            onDragEnd(e.mousePos);
            dragging = false;
            validDragStart = false;
            return true;
        }
        break;

    default:
        break;
    }

    // fallback: normal child handling
    return Container::handleEvent(e);
}

///////////
// LABEL //
///////////

Label::Label(int x, int y, int w, int h, std::string t) : Widget(x, y, w, h), text(std::move(t))
{
    auto size = Game::TextSize(text);
    if (w <= 0)
        rectInit.w = size.x + 10;
    if (h <= 0)
        rectInit.h = size.y + 5;
    rect = rectInit;
}

Label::Label(int x, int y, std::string t) : Label(x, y, 0, 0, t)
{
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
        rectInit.w = size.x + 10;
    if (h <= 0)
        rectInit.h = size.y + 5;
    rect = rectInit;
}

Button::Button(int x, int y, std::string cap) : Button(x, y, 0, 0, cap)
{
}

void Button::draw()
{
    SDL_Color color = {180, 180, 180, 255};
    SDL_Color bgColor = {30, 30, 30, 255};
    if (hovering)
    {
        color = {255, 255, 255, 255};
        bgColor = {50, 50, 50, 255};
    }
    if (clicking)
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

void Button::onMouseEnter()
{
    hovering = true;
}

void Button::onMouseExit()
{
    hovering = false;
}

void Button::onMouseDown(vec2 pos)
{
    clicking = true;
}

void Button::onMouseUp(vec2 pos)
{
    clicking = false;
}

void Button::onClick()
{
    if (onClickEvent)
        onClickEvent();
}

///////////////////////////////
// ROOT CONTAINER (internal) //
///////////////////////////////

_RootContainer::_RootContainer() : Container(0, 0, Game::ScreenWidth(), Game::ScreenHeight())
{
}

bool _RootContainer::handleEvent(const GuiEvent& e)
{
    // Only the children pass from the Container widget
    for (auto it = children.rbegin(); it != children.rend(); ++it)
    {
        Widget* w = it->get();
        if (!w->visible)
            continue;

        // If it's a container, recurse
        if (w->isContainer)
        {
            if (static_cast<Container*>(w)->handleEvent(e))
            {
                this->hoveredWidget = w;
                return true;
            }
        }
        else
        {
            if (w->handleEvent(e))
            {
                this->hoveredWidget = w;
                return true;
            }
        }
    }

    // Unlike the "Container" widget, _RootContainer itself can't handle events
    return false;
}

///////////////////
// GUI COMPONENT //
///////////////////

void GuiComponent::dispatchEvent(const GuiEvent& e)
{
    hoveredWidget = nullptr;

    if (root.handleEvent(e))
        hoveredWidget = root.hoveredWidget;

    if (e.type == GuiEventType::MouseDown)
        activeWidget = hoveredWidget;
    else if (e.type == GuiEventType::MouseUp)
        activeWidget = nullptr;
}

void GuiComponent::init()
{
    this->transform = owner->getComponent<CTransform>();
}

void GuiComponent::update(float dt)
{
    GuiEvent e;
    Input::MousePosition(&e.mousePos.x, &e.mousePos.y);

    if (Input::MouseJustPressed(SDL_BUTTON_LEFT))
    {
        e.type = GuiEventType::MouseDown;
        dispatchEvent(e);
    }
    else if (Input::MouseJustReleased(SDL_BUTTON_LEFT))
    {
        e.type = GuiEventType::MouseUp;
        dispatchEvent(e);
    }
    else if (Input::MouseMoved())
    {
        e.type = GuiEventType::MouseMove;
        dispatchEvent(e);
    }

    root.update(dt);
}

void GuiComponent::drawGUI()
{
    root.draw();
}

bool GuiComponent::isInteracting() const
{
    return hoveredWidget != nullptr || activeWidget != nullptr;
}
