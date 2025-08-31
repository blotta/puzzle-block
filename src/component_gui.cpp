#include "component_gui.hpp"
#include "game.hpp"
#include "input_manager.hpp"
#include "log.hpp"
#include <format>

Widget::Widget(int x, int y, int w, int h) : rectInit(x, y, w, h), rect(x, y, w, h)
{
}

void Widget::update(float dt)
{
    if (hovered && system->hoveredWidget != this)
    {
        hovered = false;
        onMouseExit();
    }
}

bool Widget::isOnDragHandle(const vec2& pos) const
{
    return rect.contains(pos);
}

Widget* Widget::handleEvent(const GuiEvent& e)
{
    if (!visible)
        return nullptr;

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
            return this; // hovered
        }
        break;
    }

    case GuiEventType::MouseDown: {
        if (rect.contains(e.mousePos))
        {
            onMouseDown(e.mousePos);
            return this;
        }
        break;
    }

    case GuiEventType::MouseUp: {
        if (rect.contains(e.mousePos))
        {
            onMouseUp(e.mousePos);
            onClick();
            return this;
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

    return nullptr;
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

const Widget* Widget::getRoot() const
{
    const Widget* p = this;
    while (p->parent != nullptr)
    {
        p = p->parent;
    }
    return p;
}

const GuiTheme& Widget::theme() const
{
    return *this->system->theme();
}

const Widget* Widget::getClosestFocusable() const
{
    const Widget* p = this;

    while (p != nullptr)
    {
        if (p->focusable)
            return p;

        p = p->parent;
    }

    return nullptr;
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

Widget* Container::handleEvent(const GuiEvent& e)
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
            auto widgetHandled = static_cast<Container*>(w)->handleEvent(e);
            if (widgetHandled != nullptr)
            {
                return widgetHandled;
            }
        }
        else
        {
            auto widgetHandled = w->handleEvent(e);
            if (widgetHandled != nullptr)
            {
                return widgetHandled;
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
    }
}

void Container::bringToFront(Widget* widget)
{
    auto it = std::find_if(children.begin(), children.end(), [widget](auto& w) { return w.get() == widget; });

    if (it != children.end())
    {
        auto ptr = std::move(*it);
        children.erase(it);
        children.push_back(std::move(ptr));
    }
}

////////////
// WINDOW //
////////////

Window::Window(int x, int y, int w, int h) : Container(x, y, w, h)
{
    draggable = true;
    focusable = true;
}

void Window::update(float dt)
{
    Container::update(dt);
}

void Window::draw()
{
    Color headerBgColor = theme().panelHeader.normal;
    Color headerBorderColor = theme().borderColor;
    Color bodyBgColor = theme().windowBg;
    Color bodyBorderColor = theme().borderColor;

    // header
    if (headerHeight > 0)
    {
        SDL_Rect header = {rect.x, rect.y, rect.w, headerHeight};
        if (hoveringDragHandle)
            headerBgColor = theme().panelHeader.hover.toSDL(); // {60, 60, 90, 255};
        Game::DrawFilledRect(header.x, header.y, header.w, header.h, headerBgColor.toSDL());
        Game::DrawRect(header.x, header.y, header.w, header.h, headerBorderColor.toSDL()); // header border
        Game::Text(header.x + theme().windowPaddingX, header.y + header.h / 2, title,
                   {.color = theme().textColor.toSDL(), .valign = VerticalAlign::MIDDLE});
    }

    // body
    SDL_Rect body = {rect.x, rect.y + headerHeight, rect.w, rect.h - headerHeight};
    Game::DrawFilledRect(body.x, body.y, body.w, body.h, bodyBgColor.toSDL());
    Game::DrawRect(body.x, body.y, body.w, body.h, bodyBorderColor.toSDL());

    // draw children
    Container::draw();
}

void Window::arrange()
{
    int maxX = rect.right();
    int maxY = rect.bottom();
    for (auto& c : children)
    {
        c->rect.x = c->rectInit.x + this->rect.x + theme().windowPaddingX;
        c->rect.y = c->rectInit.y + this->rect.y + headerHeight + theme().windowPaddingY;

        if (c->rect.right() + theme().windowPaddingX > maxX)
            maxX = c->rect.right() + theme().windowPaddingX;

        if (c->rect.bottom() + theme().windowPaddingY > maxY)
            maxY = c->rect.bottom() + theme().windowPaddingY;
    }
    rect.w = maxX - rect.x;
    rect.h = maxY - rect.y;
}

bool Window::isOnDragHandle(const vec2& pos) const
{
    Rect header = {rect.x, rect.y, rect.w, headerHeight};
    return header.contains(pos);
}

Widget* Window::handleEvent(const GuiEvent& e)
{
    switch (e.type)
    {
    case GuiEventType::MouseDown:
        if (isOnDragHandle(e.mousePos))
        {
            dragOffset = e.mousePos - vec2(rect.x, rect.y);
            dragStartPos = e.mousePos;
            validDragStart = true;
            dragging = false;
            return this;
        }
        break;

    case GuiEventType::MouseMove:
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
            return this;
        }
        hoveringDragHandle = isOnDragHandle(e.mousePos);
        break;

    case GuiEventType::MouseUp:
        validDragStart = false;
        if (dragging)
        {
            onDragEnd(e.mousePos);
            dragging = false;
            return this;
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
    Game::Text(rect.x, rect.y, text, {.color = theme().textColor.toSDL()});
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

Button::Button(std::string cap) : Button(0, 0, 0, 0, cap)
{
}

void Button::draw()
{
    Color color = theme().textColor;
    Color bgColor = theme().button.normal;
    if (system->hoveredWidget == this)
    {
        bgColor = theme().button.hover;
    }
    if (system->activeWidget == this)
    {
        bgColor = theme().button.active;
    }

    Game::DrawFilledRect(rect.x, rect.y, rect.w, rect.h, bgColor.toSDL());
    Game::DrawRect(rect.x, rect.y, rect.w, rect.h, color.toSDL());
    auto center = rect.center();
    Game::Text(center.x, center.y, caption,
               {.color = color.toSDL(), .align = TextAlign::CENTER, .valign = VerticalAlign::MIDDLE});
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

_RootContainer::_RootContainer(const GuiComponent* system) : Container(0, 0, Game::ScreenWidth(), Game::ScreenHeight())
{
    this->system = system;
}

Widget* _RootContainer::handleEvent(const GuiEvent& e)
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
            auto widgetHandled = static_cast<Container*>(w)->handleEvent(e);
            if (widgetHandled != nullptr)
            {
                return widgetHandled;
            }
        }
        else
        {
            auto widgetHandled = w->handleEvent(e);
            if (widgetHandled != nullptr)
            {
                return widgetHandled;
            }
        }
    }

    // Unlike the "Container" widget, _RootContainer itself can't handle events
    return nullptr;
}

///////////////////
// GUI COMPONENT //
///////////////////

void GuiComponent::dispatchEvent(const GuiEvent& e)
{
    hoveredWidget = root.handleEvent(e);

    if (e.type == GuiEventType::MouseDown)
        activeWidget = hoveredWidget;
    else if (e.type == GuiEventType::MouseUp)
        activeWidget = nullptr;

    if (activeWidget != nullptr)
    {
        auto focused = activeWidget->getClosestFocusable();
        if (focused)
        {
            focused->parent->bringToFront((Widget*)focused);
        }
    }
}

GuiComponent::GuiComponent() : root(this)
{
    this->mTheme = &GuiTheme::Dark();
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
    Game::SetFontSize(theme()->fontSize);
    root.draw();

    if (auto* btn = dynamic_cast<Button*>(hoveredWidget))
    {
        Game::Text(10, 10, std::format("Hovering button {}", btn->caption));
    }
    else if (auto* lbl = dynamic_cast<Label*>(hoveredWidget))
    {
        Game::Text(10, 10, std::format("Hovering label {}", lbl->text));
    }
    else if (auto* win = dynamic_cast<Window*>(hoveredWidget))
    {
        Game::Text(10, 10, std::format("Hovering window {}", win->title));
    }
    else if (auto* cont = dynamic_cast<Container*>(hoveredWidget))
    {
        Game::Text(10, 10, std::format("Hovering container {},{}", cont->rect.x, cont->rect.y));
    }
    else if (auto* wid = dynamic_cast<Widget*>(hoveredWidget))
    {
        Game::Text(10, 10, std::format("Hovering widget {},{}", wid->rect.x, wid->rect.y));
    }
    else
    {
        Game::Text(10, 10, "Not hovering a widget");
    }

    if (auto* btn = dynamic_cast<Button*>(activeWidget))
    {
        Game::Text(10, 30, std::format("Active button {}", btn->caption));
    }
    else if (auto* lbl = dynamic_cast<Label*>(activeWidget))
    {
        Game::Text(10, 30, std::format("Active label {}", lbl->text));
    }
    else if (auto* win = dynamic_cast<Window*>(activeWidget))
    {
        Game::Text(10, 30, std::format("Active window {}", win->title));
    }
    else if (auto* cont = dynamic_cast<Container*>(activeWidget))
    {
        Game::Text(10, 30, std::format("Active container {},{}", cont->rect.x, cont->rect.y));
    }
    else if (auto* wid = dynamic_cast<Widget*>(activeWidget))
    {
        Game::Text(10, 30, std::format("Active widget {},{}", wid->rect.x, wid->rect.y));
    }
    else
    {
        Game::Text(10, 30, "No active widget");
    }
}

bool GuiComponent::isInteracting() const
{
    return hoveredWidget != nullptr || activeWidget != nullptr;
}

void GuiComponent::setTheme(const GuiTheme& theme)
{
    mTheme = &theme;
}

const GuiTheme* GuiComponent::theme() const
{
    return mTheme;
}

const GuiTheme& GuiTheme::Dark()
{
    static GuiTheme theme;
    return theme;
}

const GuiTheme& GuiTheme::Light()
{
    static GuiTheme t;
    t.textColor = {30};
    t.textDisabledColor = {150};
    t.windowBg = {240};
    t.childBg = {250};
    t.popupBg = {255};
    t.borderColor = {200};
    t.shadowColor = {0, 60};

    t.button = {{220}, {200}, {180}, {240}};

    t.panelHeader = {{210}, {190}, {170}, {240}};

    return t;
}
