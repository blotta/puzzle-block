#ifndef COMPONENT_GUI_HPP
#define COMPONENT_GUI_HPP

#include "entity.hpp"
#include "vmath.hpp"
#include <functional>

enum class WidgetState
{
    NORMAL,
    HOVER,
    CLICK
};

class Widget
{
  public:
    Rect rect;
    bool visible = true;
    WidgetState state = WidgetState::NORMAL;
    std::function<void()> onHover;
    std::function<void()> onClick;

    Widget(int x, int y, int w, int h);
    virtual ~Widget() = default;

    virtual void draw() = 0;
    virtual void update(float dt);
};

class Label : public Widget
{
  public:
    std::string text;
    Label(int x, int y, int w, int h, std::string t);
    void draw() override;
};

class Button : public Widget
{
  public:
    std::string caption;
    Button(int x, int y, int w, int h, std::string cap);
    void draw() override;
};


class GuiComponent : public Component
{
public:
    std::vector<std::unique_ptr<Widget>> children;
    CTransform* transform;
    template<typename T, typename... Args>
    T* addChild(Args&&... args)
    {
        children.push_back(std::make_unique<T>(std::forward<Args>(args)...));
        T* ptr = static_cast<T*>(children.back().get());
        return ptr;
    }

    void init() override;
    void update(float dt) override;
    void drawGUI() override;
};

#endif