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

Widget::Widget(Widget* parent) : Widget(parent, 0, 0, 0, 0)
{
}

Widget::Widget(Widget* parent, int x, int y, int w, int h) : rectInit(x, y, w, h), rect(x, y, w, h), parent(parent)
{
    if (parent != nullptr)
        system = parent->system;
    this->applyTheme();
}

void Widget::draw()
{
    for (auto& c : children)
    {
        if (c->visible)
        {
            c->draw();
        }
    }
}

void Widget::update(float dt)
{
    for (auto& c : children)
    {
        if (c->visible)
            c->update(dt);
    }

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

    // Pass event to children first (iterate backwards so top-most handles first)
    for (auto it = children.rbegin(); it != children.rend(); ++it)
    {
        Widget* w = it->get();
        if (!w->visible)
            continue;

        Widget* widgetHandled = w->handleEvent(e);
        if (widgetHandled != nullptr)
        {
            return widgetHandled;
        }
    }

    // if no child handled event, try handling it
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
        if (this->isInsideInteractArea(e.mousePos))
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

Widget* Widget::getRoot() const
{
    Widget* p = (Widget*)this;
    while (p->parent != nullptr)
    {
        p = p->parent;
    }
    return p;
}

void Widget::applyTheme()
{
    padding.left = theme().framePaddingX;
    padding.right = theme().framePaddingX;
    padding.top = theme().framePaddingY;
    padding.bottom = theme().framePaddingY;
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

void Widget::bringToFront(Widget* widget)
{
    auto it = std::find_if(children.begin(), children.end(), [widget](auto& w) { return w.get() == widget; });

    if (it != children.end())
    {
        auto ptr = std::move(*it);
        children.erase(it);
        children.push_back(std::move(ptr));
    }
}

void Widget::calcFitSize()
{
    // First let children compute their own sizes
    for (auto& child : children)
        child->calcFitSize();

    int w = 0;
    int h = 0;

    if (children.empty())
    {
        // Leaf widget: FIT is content-defined, so we don’t override here.
        if (widthSizing.sizing == AxisSizing::FIXED)
            w = widthSizing.value;
        else
            w = rect.w;

        if (heightSizing.sizing == AxisSizing::FIXED)
            h = heightSizing.value;
        else
            h = rect.h;
    }
    else
    {
        // Container widget: compute based on children + layout
        if (layout == LayoutType::TopToBottom)
        {
            for (auto& child : children)
            {
                h += child->rect.h;
                w = std::max(w, child->rect.w);
            }
            if (children.size() > 1)
                h += gap * (children.size() - 1);
        }
        else if (layout == LayoutType::LeftToRight)
        {
            for (auto& child : children)
            {
                w += child->rect.w;
                h = std::max(h, child->rect.h);
            }
            if (children.size() > 1)
                w += gap * (children.size() - 1);
        }
        else // None
        {
            int minX = INT_MAX, maxX = INT_MIN;
            int minY = INT_MAX, maxY = INT_MIN;
            for (auto& child : children)
            {
                minX = std::min(minX, child->rectInit.x);
                maxX = std::max(maxX, child->rectInit.right());
                minY = std::min(minY, child->rectInit.y);
                maxY = std::max(maxY, child->rectInit.bottom());
            }
            if (minX != INT_MAX)
            {
                w = maxX - minX;
                h = maxY - minY;
            }
        }

        // Add padding
        w += padding.left + padding.right;
        h += padding.top + padding.bottom;
    }

    contentWidth = w;
    contentHeight = h;

    // Apply sizing rules
    if (widthSizing.sizing == AxisSizing::FIXED)
        rect.w = widthSizing.value;
    else if (widthSizing.sizing == AxisSizing::FIT)
        rect.w = w; // from children or content
    // Grow is deferred to parent’s available space

    if (heightSizing.sizing == AxisSizing::FIXED)
        rect.h = heightSizing.value;
    else if (heightSizing.sizing == AxisSizing::FIT)
        rect.h = h;

}

void Widget::calcGrowShrinkSize()
{
    // First, apply min/max constraints
    if (widthSizing.min > 0)
        rect.w = std::max(widthSizing.min, rect.w);
    if (widthSizing.max > 0)
        rect.w = std::min(widthSizing.max, rect.w);

    if (heightSizing.min > 0)
        rect.h = std::max(heightSizing.min, rect.h);
    if (heightSizing.max > 0)
        rect.h = std::min(heightSizing.max, rect.h);

    // If no children, nothing else to do
    if (children.empty())
        return;

    // Determine axis orientation
    bool isRow = (layout == LayoutType::TopToBottom);
    int containerMainSize = isRow ? rect.h - (padding.top + padding.bottom) : rect.w - (padding.left + padding.right);

    // Compute total fixed size of children
    int totalFixed = 0;
    int growCount = 0;
    for (auto& child : children)
    {
        if ((!isRow && child->widthSizing.sizing == AxisSizing::GROW) ||
            (isRow && child->heightSizing.sizing == AxisSizing::GROW))
        {
            growCount++;
        }
        else
        {
            totalFixed += isRow ? child->rect.h : child->rect.w;
        }
    }

    // Remaining free space for grow children
    int freeSpace = containerMainSize - totalFixed - gap * (children.size() > 1 ? children.size() - 1 : 0);
    int perGrow = (growCount > 0) ? std::max(0, freeSpace / growCount) : 0;

    // Assign sizes
    for (auto& child : children)
    {
        if (isRow)
        {
            if (child->heightSizing.sizing == AxisSizing::GROW)
                child->rect.h = perGrow;
        }
        else
        {
            if (child->widthSizing.sizing == AxisSizing::GROW)
                child->rect.w = perGrow;
        }

        // Recurse for nested containers
        child->calcGrowShrinkSize();
    }

    if (scrollableX)
        scrollX = std::clamp(scrollX, 0, std::max(0, contentWidth - rect.w));
    if (scrollableY)
        scrollY = std::clamp(scrollY, 0, std::max(0, contentHeight - rect.h));
}

void Widget::calcPosition()
{
    if (children.empty())
        return;

    bool isRow = (layout == LayoutType::TopToBottom);

    // Available space after padding
    int innerMainSize = isRow ? rect.h - (padding.top + padding.bottom) : rect.w - (padding.left + padding.right);

    int innerCrossSize = isRow ? rect.w - (padding.left + padding.right) : rect.h - (padding.top + padding.bottom);

    // Compute total main size of children
    int totalMain = 0;
    for (auto& child : children)
        totalMain += isRow ? child->rect.h : child->rect.w;

    int freeSpace = innerMainSize - totalMain - gap * (children.size() > 1 ? children.size() - 1 : 0);

    // Compute offset and spacing based on justifyContent
    int offset = 0, spacing = gap;
    switch (justifyContent)
    {
    case JustifyContent::Start:
        offset = 0;
        break;
    case JustifyContent::End:
        offset = freeSpace;
        break;
    case JustifyContent::Center:
        offset = freeSpace / 2;
        break;
    case JustifyContent::SpaceBetween:
        spacing = (children.size() > 1) ? freeSpace / (children.size() - 1) : 0;
        break;
    case JustifyContent::SpaceAround:
        spacing = (children.size() > 0) ? freeSpace / children.size() : 0;
        offset = spacing / 2;
        break;
    case JustifyContent::SpaceEvenly:
        spacing = (children.size() > 0) ? freeSpace / (children.size() + 1) : 0;
        offset = spacing;
        break;
    }

    // Starting position
    int mainPos = (isRow ? padding.top : padding.left) + offset;

    // Place children
    for (auto& child : children)
    {
        if (isRow)
        {
            child->rect.y = rect.y + mainPos - scrollY;

            switch (alignItems)
            {
            case AlignItems::Start:
                child->rect.x = rect.x + padding.left;
                break;
            case AlignItems::End:
                child->rect.x = rect.x + rect.w - padding.right - child->rect.w;
                break;
            case AlignItems::Center:
                child->rect.x = rect.x + padding.left + (innerCrossSize - child->rect.w) / 2;
                break;
            case AlignItems::Stretch:
                child->rect.x = rect.x + padding.left;
                child->rect.w = innerCrossSize;
                break;
            }

            mainPos += child->rect.h + spacing;
        }
        else // LeftToRight layout
        {
            child->rect.x = rect.x + mainPos - scrollX;

            // Cross axis alignment
            switch (alignItems)
            {
            case AlignItems::Start:
                child->rect.y = rect.y + padding.top;
                break;
            case AlignItems::End:
                child->rect.y = rect.y + rect.h - padding.bottom - child->rect.h;
                break;
            case AlignItems::Center:
                child->rect.y = rect.y + padding.top + (innerCrossSize - child->rect.h) / 2;
                break;
            case AlignItems::Stretch:
                child->rect.y = rect.y + padding.top;
                child->rect.h = innerCrossSize;
                break;
            }

            mainPos += child->rect.w + spacing;
        }

        // Recurse into child containers
        child->calcPosition();
    }
}


///////////
// PANEL //
///////////

Panel::Panel(Widget* parent) : Panel(parent, 0, 0, 0, 0)
{
}

Panel::Panel(Widget* parent, int w, int h) : Panel(parent, 0, 0, w, h)
{
}

Panel::Panel(Widget* parent, int x, int y, int w, int h) : Widget(parent, x, y, w, h)
{
    draggable = false;
    focusable = false;
    this->applyTheme();
}

void Panel::draw()
{
    Game::DrawFilledRect(rect.x, rect.y, rect.w, rect.h, backgroundColor.toSDL());

    // draw children
    Game::PushClipRect(rect);
    Widget::draw();
    Game::PopClipRect();

    Game::DrawRect(rect.x, rect.y, rect.w, rect.h, borderColor.toSDL());
}

void Panel::applyTheme()
{
    backgroundColor = theme().windowBg;
    borderColor = theme().borderColor;
}

////////////
// WINDOW //
////////////

Window::Window(Widget* parent, int x, int y, int w, int h) : Widget(parent, x, y, w, h)
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
    Widget::draw();
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
    return Widget::handleEvent(e);
}

bool Window::isInsideInteractArea(const vec2& pos) const
{
    Rect interactArea = {rect.x, rect.y - headerHeight, rect.w, rect.h + headerHeight};
    return interactArea.contains(pos);
}

///////////
// LABEL //
///////////

Label::Label(Widget* parent, int x, int y, int w, int h, std::string t) : Widget(parent, x, y, w, h), text(std::move(t))
{
    this->applyTheme();
}

Label::Label(Widget* parent, int x, int y, std::string t) : Label(parent, x, y, 0, 0, std::move(t))
{
}

Label::Label(Widget* parent, std::string t) : Label(parent, 0, 0, 0, 0, std::move(t))
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

Button::Button(Widget* parent, int x, int y, int w, int h, std::string t)
    : Widget(parent, x, y, w, h), text(std::move(t))
{
    this->applyTheme();
}

Button::Button(Widget* parent, int x, int y, std::string cap) : Button(parent, x, y, 0, 0, cap)
{
}

Button::Button(Widget* parent, std::string cap) : Button(parent, 0, 0, 0, 0, cap)
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

/////////////////
// ProgressBar //
/////////////////

ProgressBar::ProgressBar(Widget* parent, int x, int y, int w, int h) : Widget(parent, x, y, w, h)
{
    applyTheme();
}

void ProgressBar::draw()
{
    Game::DrawFilledRect(rect.x, rect.y, rect.w, rect.h, backgroundColor.toSDL());

    float prog = clamp(value / maxValue, 0.f, 1.f);

    SDL_Rect r = {
        rect.x + padding.left,
        rect.y + padding.top,
        (int)((rect.w - padding.left - padding.right) * prog),
        rect.h - padding.top - padding.bottom,
    };
    Game::DrawFilledRect(r.x, r.y, r.w, r.h, barColor.toSDL());

    Game::DrawRect(rect.x, rect.y, rect.w, rect.h, borderColor.toSDL());
}

void ProgressBar::applyTheme()
{
    padding.left = theme().framePaddingX;
    padding.right = theme().framePaddingX;
    padding.top = theme().framePaddingY;
    padding.bottom = theme().framePaddingY;

    backgroundColor = theme().childBg;
    borderColor = theme().borderColor;
    barColor = theme().textColor;
}

////////////
// Cursor //
////////////

Cursor::Cursor(Widget* parent) : Widget(parent, 0, 0, 0, 0)
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

    Game::DrawFilledRect(selected->rect.x, selected->rect.y, selected->rect.w, selected->rect.h, {230, 230, 150, 20});
}

////////////////////////////
// ROOT WIDGET (internal) //
////////////////////////////

_RootWidget::_RootWidget(GuiComponent* system) : Widget(nullptr, 0, 0, Game::ScreenWidth(), Game::ScreenHeight())
{
    this->system = system;
}

Widget* _RootWidget::handleEvent(const GuiEvent& e)
{
    // Only the children pass from the Container widget
    for (auto it = children.rbegin(); it != children.rend(); ++it)
    {
        Widget* w = it->get();
        if (!w->visible)
            continue;

        Widget* widgetHandled = w->handleEvent(e);
        if (widgetHandled != nullptr)
        {
            return widgetHandled;
        }
    }

    // Unlike the "Container" widget, _RootWidget itself can't handle events
    return nullptr;
}

void _RootWidget::update(float dt)
{
    for (auto& c : children)
    {
        if (c->visible)
            c->update(dt);
    }
}

void _RootWidget::applyTheme()
{
}

void _RootWidget::calcFitSize()
{
    for (auto& child : this->children)
    {
        child->calcFitSize();
    }
}

void _RootWidget::calcGrowShrinkSize()
{
    for (auto& child : this->children)
    {
        child->calcGrowShrinkSize();
    }
}

void _RootWidget::calcPosition()
{
    for (auto& child : children)
    {
        child->calcPosition();
    }
}

void _RootWidget::draw()
{
    for (auto& c : children)
    {
        if (c->visible)
        {
            c->draw();
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
    // else if (auto* cont = dynamic_cast<Container*>(hoveredWidget))
    // {
    //     hoverTxt = std::format("Hovering container {},{}", cont->rect.x, cont->rect.y);
    // }
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
    // else if (auto* cont = dynamic_cast<Container*>(activeWidget))
    // {
    //     activeTxt = std::format("Active container {},{}", cont->rect.x, cont->rect.y);
    // }
    else if (auto* wid = dynamic_cast<Widget*>(activeWidget))
    {
        activeTxt = std::format("Active widget {},{}", wid->rect.x, wid->rect.y);
    }

    Game::Text(x, y + 25, activeTxt, {.align = TextAlign::RIGHT});
}

int GuiComponent::drawHierarchy(Widget* cont, int x, int y) const
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
    else if (auto* rt = dynamic_cast<_RootWidget*>(cont))
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
        else if (auto* childCont = dynamic_cast<Widget*>(child.get()))
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

GuiTheme& GuiTheme::Game()
{
    static GuiTheme t;
    t.textColor = {200};
    t.textDisabledColor = {150};
    t.windowBg = {10, 250};
    t.childBg = {15, 250};
    t.popupBg = {20, 250};
    t.borderColor = {200};
    t.shadowColor = {0, 60};

    return t;
}
