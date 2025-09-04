#include "component_gui.hpp"
#include "game.hpp"
#include "input_manager.hpp"
#include "log.hpp"
#include <format>

RectSizing::RectSizing()
{
}

RectSizing::RectSizing(int v) : left(v), top(v), right(v), bottom(v)
{
}

RectSizing::RectSizing(int x, int y) : left(x), top(y), right(x), bottom(y)
{
}

void RectSizing::set(int v)
{
    left = v;
    right = v;
    top = v;
    bottom = v;
}

void RectSizing::setX(int v)
{
    left = v;
    right = v;
}

void RectSizing::setY(int v)
{
    top = v;
    bottom = v;
}

Widget::Widget(Container* parent, int x, int y, int w, int h) : rectInit(x, y, w, h), rect(x, y, w, h), parent(parent)
{
    if (parent != nullptr)
        system = parent->system;
    this->applyTheme();
}

void Widget::update(float dt)
{
    if (hovered && system->hoveredWidget != this)
    {
        hovered = false;
        onMouseExit();
    }
}

bool Widget::isInsideInteractArea(const vec2& pos) const
{
    return rect.contains(pos);
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
        if (this->isInsideInteractArea(e.mousePos))
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
        if (this->isInsideInteractArea(e.mousePos))
        {
            onMouseDown(e.mousePos);
            return this;
        }
        break;
    }

    case GuiEventType::MouseUp: {
        if (this->isInsideInteractArea(e.mousePos)) // rect.contains(e.mousePos))
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

Container* Widget::getRoot() const
{
    Container* p = this->isContainer ? (Container*)this : (Container*)this->parent;
    while (p->parent != nullptr)
    {
        p = p->parent;
    }
    return p;
}

void Widget::applyTheme()
{
}

const GuiTheme& Widget::theme() const
{
    return GuiTheme::Current();
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

void Widget::calcFitSize()
{
}

void Widget::calcGrowShrinkSize()
{
}

///////////////
// CONTAINER //
///////////////

Container::Container(Container* parent) : Widget(parent, 0, 0, 0, 0)
{
    isContainer = true;
    this->applyTheme();
}

Container::Container(Container* parent, int x, int y) : Widget(parent, x, y, 0, 0)
{
    isContainer = true;
    this->applyTheme();
}

Container::Container(Container* parent, int x, int y, int w, int h) : Widget(parent, x, y, w, h)
{
    isContainer = true;
    minWidth = w;
    minHeight = h;
    this->applyTheme();
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

void Container::calcFitSize()
{
    int w = 0;
    int h = 0;
    int minX = Game::ScreenWidth();
    int maxX = 0;
    int minY = Game::ScreenHeight();
    int maxY = 0;

    for (auto& child : this->children)
    {
        child->calcFitSize();

        if (layout == LayoutType::Row)
        {
            w += child->rect.w;
            h = max(h, child->rect.h);
        }
        else if (layout == LayoutType::Column)
        {
            h += child->rect.h;
            w = max(w, child->rect.w);
        }
        else if (layout == LayoutType::None)
        {
            minX = min(minX, child->rectInit.x);
            maxX = max(maxX, child->rectInit.right());
            minY = min(minY, child->rectInit.y);
            maxY = max(maxY, child->rectInit.bottom());
        }
    }

    if (this->children.size())
    {
        if (layout == LayoutType::None)
        {
            w = maxX - minX;
            h = maxY - minY;
        }
        else
        {
            int childGap = gap * (this->children.size() - 1);
            if (layout == LayoutType::Row)
                w += childGap;
            else if (layout == LayoutType::Column)
                h += childGap;
        }
    }

    w += padding.left + padding.right;
    h += padding.top + padding.bottom;

    this->rect.w = max(w, minWidth);
    this->rect.h = max(h, minHeight);
}

void Container::calcGrowShrinkSize()
{
}

void Container::calcPosition()
{
    if (layout == LayoutType::None)
    {
        for (auto& c : children)
        {
            c->rect.x = c->rectInit.x + this->rect.x;
            c->rect.y = c->rectInit.y + this->rect.y;
        }
    }
    else
    {
        if (children.empty())
            return;

        bool isRow = (layout == LayoutType::Row);

        // total size of children + gaps
        int totalMainSize = 0;
        for (auto& child : children)
        {
            totalMainSize += isRow ? child->rect.w : child->rect.h;
        }

        // available inner space (after padding)
        int containerMainSize =
            isRow ? rect.w - (padding.left + padding.right) : rect.h - (padding.top + padding.bottom);

        int freeSpace = containerMainSize - totalMainSize;

        // decide offset + spacing
        int offset = 0;
        int spacing = 0;
        int totalGaps = gap * (children.size() > 1 ? children.size() - 1 : 0);
        switch (justifyContent)
        {
        case JustifyContent::Start:
            freeSpace -= totalGaps;
            offset = 0;
            spacing = gap;
            break;
        case JustifyContent::End:
            freeSpace -= totalGaps;
            offset = freeSpace;
            spacing = gap;
            break;
        case JustifyContent::Center:
            freeSpace -= totalGaps;
            offset = freeSpace / 2;
            spacing = gap;
            break;
        case JustifyContent::SpaceBetween:
            spacing = (children.size() > 1) ? (freeSpace / (children.size() - 1)) : 0;
            offset = 0;
            break;
        case JustifyContent::SpaceAround:
            spacing = (children.size() > 0) ? (freeSpace / children.size()) : 0;
            offset = spacing / 2;
            break;
        case JustifyContent::SpaceEvenly:
            spacing = (children.size() > 0) ? (freeSpace / (children.size() + 1)) : 0;
            offset = spacing;
            break;
        }

        // start main position after padding
        int mainPos = (isRow ? padding.left : padding.top) + offset;

        // assign positions
        for (auto& child : children)
        {
            if (isRow)
            {
                child->rect.x = rect.x + mainPos;
                // cross-axis alignment
                switch (alignItems)
                {
                case AlignItems::Start:
                    child->rect.y = rect.y + padding.top;
                    break;
                case AlignItems::End:
                    child->rect.y = rect.y + rect.h - padding.bottom - child->rect.h;
                    break;
                case AlignItems::Center:
                    child->rect.y = rect.y + (rect.h - child->rect.h) / 2;
                    break;
                case AlignItems::Stretch:
                    child->rect.y = rect.y + padding.top;
                    child->rect.h = rect.h - (padding.top + padding.bottom);
                    break;
                }
                mainPos += child->rect.w + spacing;
            }
            else
            {
                child->rect.y = rect.y + mainPos;
                // cross-axis alignment
                switch (alignItems)
                {
                case AlignItems::Start:
                    child->rect.x = rect.x + padding.left;
                    break;
                case AlignItems::End:
                    child->rect.x = rect.x + rect.w - padding.right - child->rect.w;
                    break;
                case AlignItems::Center:
                    child->rect.x = rect.x + (rect.w - child->rect.w) / 2;
                    break;
                case AlignItems::Stretch:
                    child->rect.x = rect.x + padding.left;
                    child->rect.w = rect.w - (padding.left + padding.right);
                    break;
                }
                mainPos += child->rect.h + spacing;
            }
        }
    }

    for (auto& child : children)
    {
        if (auto* cont = dynamic_cast<Container*>(child.get()))
        {
            cont->calcPosition();
        }
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

void Container::applyTheme()
{
    padding.left = theme().framePaddingX;
    padding.right = theme().framePaddingX;
    padding.top = theme().framePaddingY;
    padding.bottom = theme().framePaddingY;
}

///////////
// PANEL //
///////////

Panel::Panel(Container* parent, int x, int y, int w, int h) : Container(parent, x, y, w, h)
{
    draggable = false;
    focusable = false;
    this->applyTheme();
}

void Panel::draw()
{
    Game::DrawFilledRect(rect.x, rect.y, rect.w, rect.h, backgroundColor.toSDL());
    Game::DrawRect(rect.x, rect.y, rect.w, rect.h, backgroundColor.toSDL());

    // draw children
    Game::PushClipRect(rect);
    Container::draw();
    Game::PopClipRect();
}

void Panel::applyTheme()
{
    backgroundColor = theme().windowBg;
}

////////////
// WINDOW //
////////////

Window::Window(Container* parent, int x, int y, int w, int h) : Container(parent, x, y, w, h)
{
    draggable = true;
    focusable = true;
    this->applyTheme();
}

void Window::draw()
{

    // header
    if (headerHeight > 0)
    {
        SDL_Rect header = {rect.x, rect.y - headerHeight, rect.w, headerHeight};
        Game::DrawFilledRect(header.x, header.y, header.w, header.h,
                             hoveringDragHandle ? headerBackgroundColorHover.toSDL() : headerBackgroundColor.toSDL());
        Game::DrawRect(header.x, header.y, header.w, header.h, headerBorderColor.toSDL()); // header border
        Game::Text(header.x + padding.left, header.y + header.h / 2, title,
                   {.color = headerTextColor.toSDL(), .valign = VerticalAlign::MIDDLE});
    }

    // body
    SDL_Rect body = {rect.x, rect.y, rect.w, rect.h};
    Game::DrawFilledRect(body.x, body.y, body.w, body.h, bodyBackgroundColor.toSDL());
    Game::DrawRect(body.x, body.y, body.w, body.h, bodyBorderColor.toSDL());

    // draw children
    Container::draw();
}

bool Window::isOnDragHandle(const vec2& pos) const
{
    Rect header = {rect.x, rect.y - headerHeight, rect.w, headerHeight};
    return header.contains(pos);
}

void Window::applyTheme()
{
    headerBackgroundColor = theme().panelHeader.normal;
    headerBackgroundColorHover = theme().panelHeader.hover;
    headerBorderColor = theme().borderColor;
    headerTextColor = theme().textColor;

    bodyBackgroundColor = theme().windowBg;
    bodyBorderColor = theme().borderColor;
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

            calcPosition();
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

bool Window::isInsideInteractArea(const vec2& pos) const
{
    Rect interactArea = {rect.x, rect.y - headerHeight, rect.w, rect.h + headerHeight};
    return interactArea.contains(pos);
}

///////////
// LABEL //
///////////

Label::Label(Container* parent, int x, int y, int w, int h, std::string t)
    : Widget(parent, x, y, w, h), text(std::move(t))
{
    this->applyTheme();
}

Label::Label(Container* parent, int x, int y, std::string t) : Label(parent, x, y, 0, 0, std::move(t))
{
}

Label::Label(Container* parent, std::string t) : Label(parent, 0, 0, 0, 0, std::move(t))
{
}

void Label::draw()
{
    Game::SetFontSize(this->fontSize);
    Game::Text(rect.x, rect.y, text,
               {
                   .color = textColor.toSDL(),
                   .align = TextAlign::LEFT,
               });
}

void Label::applyTheme()
{
    textColor = theme().textColor;

    this->setFontSize(theme().fontSize);
}

const std::string& Label::getText() const
{
    return text;
}

void Label::setText(const std::string& t)
{
    this->text = t;
    this->setFontSize(this->fontSize);
}

void Label::setFontSize(int fs)
{
    this->fontSize = fs;
    Game::SetFontSize(this->fontSize);
    auto size = Game::TextSize(text);
    rect.w = size.x;
    rect.h = size.y;
}

////////////
// BUTTON //
////////////

Button::Button(Container* parent, int x, int y, int w, int h, std::string t)
    : Widget(parent, x, y, w, h), text(std::move(t))
{
    this->applyTheme();
}

Button::Button(Container* parent, int x, int y, std::string cap) : Button(parent, x, y, 0, 0, cap)
{
}

Button::Button(Container* parent, std::string cap) : Button(parent, 0, 0, 0, 0, cap)
{
}

void Button::draw()
{
    Color bgColor = backgroundColor;
    if (system->hoveredWidget == this)
    {
        bgColor = backgroundColorHover;
    }
    if (system->activeWidget == this)
    {
        bgColor = backgroundColorActive;
    }

    Game::DrawFilledRect(rect.x, rect.y, rect.w, rect.h, bgColor.toSDL());
    Game::DrawRect(rect.x, rect.y, rect.w, rect.h, borderColor.toSDL());
    auto center = rect.center();
    Game::SetFontSize(this->fontSize);
    Game::Text(center.x, center.y, text,
               {.color = textColor.toSDL(), .align = TextAlign::CENTER, .valign = VerticalAlign::MIDDLE});
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

const std::string& Button::getText() const
{
    return text;
}

void Button::setText(const std::string& t)
{
    this->text = t;
    this->setFontSize(this->fontSize);
}

void Button::setFontSize(int fs)
{
    this->fontSize = fs;
    Game::SetFontSize(this->fontSize);
    auto size = Game::TextSize(text);
    rect.w = size.x + 10;
    rect.h = size.y + 5;
}

void Button::applyTheme()
{
    textColor = theme().textColor;
    backgroundColor = theme().button.normal;
    backgroundColorHover = theme().button.hover;
    backgroundColorActive = theme().button.active;
    borderColor = theme().borderColor;

    setFontSize(theme().fontSize);
}

///////////////////////////////
// ROOT CONTAINER (internal) //
///////////////////////////////

_RootContainer::_RootContainer(GuiComponent* system)
    : Container(nullptr, 0, 0, Game::ScreenWidth(), Game::ScreenHeight())
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

void _RootContainer::update(float dt)
{
    Container::update(dt);
}

void _RootContainer::applyTheme()
{
}

void _RootContainer::calcFitSize()
{
    for (auto& child : this->children)
    {
        child->calcFitSize();
    }
}

void _RootContainer::calcPosition()
{
    // for (auto& c : children)
    // {
    //     c->rect.x = c->rectInit.x + this->rect.x;
    //     c->rect.y = c->rectInit.y + this->rect.y;
    // }

    for (auto& child : children)
    {
        if (auto* cont = dynamic_cast<Container*>(child.get()))
        {
            cont->calcPosition();
        }
    }
}

///////////////////
// GUI COMPONENT //
///////////////////

GuiComponent::GuiComponent() : root(this)
{
}

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

    root.rect.x = this->transform->pos.x;
    root.rect.y = this->transform->pos.y;

    root.update(dt);

    this->calcAll();
}

void GuiComponent::drawGUI()
{
    Game::SetFontSize(GuiTheme::Current().fontSize);
    root.draw();

    // this->drawHoverAndActive(Game::ScreenWidth() - 10, 10);
    // this->drawHierarchy(static_cast<Container*>(&root), Game::ScreenWidth() - 300, 100);
}

bool GuiComponent::isInteracting() const
{
    return hoveredWidget != nullptr || activeWidget != nullptr;
}

void GuiComponent::calcAll()
{
    root.calcFitSize();
    root.calcGrowShrinkSize();
    root.calcPosition();
}

void GuiComponent::drawHoverAndActive(int x, int y) const
{
    std::string hoverTxt = "Not hovering a widget";
    if (auto* btn = dynamic_cast<Button*>(hoveredWidget))
    {
        hoverTxt = std::format("Hovering button {}", btn->getText());
    }
    else if (auto* lbl = dynamic_cast<Label*>(hoveredWidget))
    {
        hoverTxt = std::format("Hovering label {}", lbl->getText());
    }
    else if (auto* win = dynamic_cast<Window*>(hoveredWidget))
    {
        hoverTxt = std::format("Hovering window {}", win->title);
    }
    else if (auto* wid = dynamic_cast<Panel*>(hoveredWidget))
    {
        hoverTxt = std::format("Hovering panel {},{}", wid->rect.x, wid->rect.y);
    }
    else if (auto* cont = dynamic_cast<Container*>(hoveredWidget))
    {
        hoverTxt = std::format("Hovering container {},{}", cont->rect.x, cont->rect.y);
    }
    else if (auto* wid = dynamic_cast<Widget*>(hoveredWidget))
    {
        hoverTxt = std::format("Hovering widget {},{}", wid->rect.x, wid->rect.y);
    }

    Game::Text(x, y, hoverTxt, {.align = TextAlign::RIGHT});

    std::string activeTxt = "No active widget";
    if (auto* btn = dynamic_cast<Button*>(activeWidget))
    {
        activeTxt = std::format("Active button {}", btn->getText());
    }
    else if (auto* lbl = dynamic_cast<Label*>(activeWidget))
    {
        activeTxt = std::format("Active label {}", lbl->getText());
    }
    else if (auto* win = dynamic_cast<Window*>(activeWidget))
    {
        activeTxt = std::format("Active window {}", win->title);
    }
    else if (auto* win = dynamic_cast<Panel*>(activeWidget))
    {
        activeTxt = std::format("Active panel {}", win->title);
    }
    else if (auto* cont = dynamic_cast<Container*>(activeWidget))
    {
        activeTxt = std::format("Active container {},{}", cont->rect.x, cont->rect.y);
    }
    else if (auto* wid = dynamic_cast<Widget*>(activeWidget))
    {
        activeTxt = std::format("Active widget {},{}", wid->rect.x, wid->rect.y);
    }

    Game::Text(x, y + 25, activeTxt, {.align = TextAlign::RIGHT});
}

int GuiComponent::drawHierarchy(Container* cont, int x, int y) const
{
    int lines = 0;

    std::string contTxt = std::format("Container {},{},{},{}", cont->rect.x, cont->rect.y, cont->rect.w, cont->rect.h);
    if (auto* win = dynamic_cast<Window*>(cont))
    {
        contTxt =
            std::format("Window {} {},{},{},{}", win->title, cont->rect.x, cont->rect.y, cont->rect.w, cont->rect.h);
    }
    else if (auto* panel = dynamic_cast<Panel*>(cont))
    {
        contTxt = std::format("Panel {},{},{},{}", panel->rect.x, panel->rect.y, panel->rect.w, panel->rect.h);
    }
    else if (auto* rt = dynamic_cast<_RootContainer*>(cont))
    {
        contTxt = std::format("ROOT {},{},{},{}", rt->rect.x, rt->rect.y, rt->rect.w, rt->rect.h);
    }
    Game::Text(x, y, contTxt);
    lines++;

    const int indentation = 10;
    const int lineHeight = 20;
    x += indentation;
    y += lineHeight;

    for (auto& child : cont->children)
    {
        std::string childTxt =
            std::format("Widget {},{},{},{}", child->rect.x, child->rect.y, child->rect.w, child->rect.h);

        if (auto* btn = dynamic_cast<Button*>(child.get()))
        {
            childTxt = std::format("Button {} {},{},{},{}", btn->getText(), child->rect.x, child->rect.y, child->rect.w,
                                   child->rect.h);
            Game::Text(x, y, childTxt);
            y += lineHeight;
            lines++;
        }
        else if (auto* lbl = dynamic_cast<Label*>(child.get()))
        {
            childTxt = std::format("Label {} {},{},{},{}", lbl->getText(), child->rect.x, child->rect.y, child->rect.w,
                                   child->rect.h);
            Game::Text(x, y, childTxt);
            y += lineHeight;
            lines++;
        }
        else if (auto* childCont = dynamic_cast<Container*>(child.get()))
        {
            auto childLines = drawHierarchy(childCont, x, y);
            y += childLines * lineHeight;
            lines += childLines;
        }
    }

    return lines;
}

GuiTheme& GuiTheme::Current()
{
    static GuiTheme instance = GuiTheme::Dark();
    return instance;
}

void GuiTheme::Set(const GuiTheme& theme)
{
    Current() = theme;
}

GuiTheme& GuiTheme::Dark()
{
    static GuiTheme theme;
    return theme;
}

GuiTheme& GuiTheme::Light()
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

/////////////////
// ProgressBar //
/////////////////

ProgressBar::ProgressBar(Container* parent, int x, int y, int w, int h) : Widget(parent, x, y, w, h)
{
}

void ProgressBar::draw()
{
    // SDL_RenderFillRect(Game::GetRenderer(), &sfx_rect);
    Game::DrawFilledRect(rect.x, rect.y, rect.w, rect.h, {255, 255, 255, 255});

    float prog = min(1.f, ((float)value / maxValue));

    SDL_Rect r = {
        rect.x,
        rect.y,
        (int)(rect.w * prog),
        rect.h,
    };
    Game::DrawFilledRect(r.x, r.y, r.w, r.h, {0, 0, 0, 255});
}

////////////
// Cursor //
////////////

Cursor::Cursor(Container* parent) : Widget(parent, 0, 0, 0, 0)
{
}

void Cursor::addSelectable(Widget* w, std::function<void()> action)
{
    selectables.push_back(w);
    actions[w] = action;
    if (!selected)
        selected = w;
}

void Cursor::focusNext()
{
    if (selectables.empty())
        return;

    if (!selected || selectables.size() == 1)
    {
        selected = selectables.front();
    }
    else
    {
        auto it = std::find_if(selectables.begin(), selectables.end(), [&](auto& c) { return c == selected; });
        ++it;
        if (it == selectables.end())
            selected = selectables.front();
        else
            selected = *it;
    }
}

void Cursor::focusPrevious()
{
    if (selectables.empty())
        return;

    if (!selected || selectables.size() == 1)
    {
        selected = selectables.front();
    }
    else
    {
        auto it = std::find_if(selectables.begin(), selectables.end(), [&](auto& c) { return c == selected; });
        if (it == selectables.begin())
            selected = selectables.back();
        else
            selected = *std::prev(it);
    }
}

void Cursor::update(float dt)
{
    if (!selected || !enabled)
        return;
    
    actions[selected]();
}

void Cursor::draw()
{
    if (!selected || !enabled)
        return;

    Game::DrawLine(selected->rect.x, selected->rect.bottom() + 5, selected->rect.right(), selected->rect.bottom() + 5,
                   {255, 255, 255, 255});
}
