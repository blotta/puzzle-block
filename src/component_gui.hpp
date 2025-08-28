#ifndef COMPONENT_GUI_HPP
#define COMPONENT_GUI_HPP

#include "entity.hpp"
#include "vmath.hpp"
#include <functional>

enum class GuiEventType
{
    MouseMove,
    MouseDown,
    MouseUp,
    KeyDown,
    KeyUp,
};

struct GuiEvent
{
    GuiEventType type;
    vec2 mousePos;
    int button = 0;
    int key = 0;
};

class Container;

class Widget
{
  public:
    Rect rectInit; // initial rect values
    Rect rect;     // used to draw, bounds check and handled by containers
    bool visible = true;
    bool draggable = false;
    Container* parent = nullptr;
    bool isContainer = false;
    bool hovered = false;

    Widget(int x, int y, int w, int h);
    virtual ~Widget() = default;

    virtual void draw() = 0;
    virtual void update(float dt);
    virtual bool isOnDragHandle(const vec2& pos) const;
    virtual bool handleEvent(const GuiEvent& e);
    virtual void onMouseEnter();
    virtual void onMouseExit();
    virtual void onMouseDown(vec2 pos);
    virtual void onMouseUp(vec2 pos);
    virtual void onClick();
    virtual void onDragStart(vec2 pos);
    virtual void onDrag(vec2 pos);
    virtual void onDragEnd(vec2 pos);
};

class Container : public Widget
{
  public:
    std::vector<std::unique_ptr<Widget>> children;
    Container();
    Container(int x, int y);
    Container(int x, int y, int w, int h);

    template <typename T, typename... Args> T* addChild(Args&&... args)
    {
        children.push_back(std::make_unique<T>(std::forward<Args>(args)...));
        T* ptr = static_cast<T*>(children.back().get());
        ptr->parent = this;
        this->arrange();
        return ptr;
    }
    void update(float dt) override;
    void draw() override;
    bool handleEvent(const GuiEvent& e) override;
    virtual void arrange();
};

class Panel : public Container
{
  public:
    Panel(int x, int y, int w, int h);
    std::string title;
    int headerHeight = 30;
    bool hoveringDragHandle = false;
    bool dragging = false;
    vec2 dragOffset;
    vec2 dragStartPos;
    bool validDragStart = false;
    void update(float dt) override;
    void draw() override;
    void arrange() override;
    bool handleEvent(const GuiEvent& e) override;
    bool isOnDragHandle(const vec2& pos) const override;
};

class Label : public Widget
{
  public:
    std::string text;
    Label(int x, int y, int w, int h, std::string t);
    Label(int x, int y, std::string t);
    void draw() override;
};

class Button : public Widget
{
    bool hovering = false;
    bool clicking = false;

  public:
    std::string caption;
    Button(int x, int y, int w, int h, std::string cap);
    Button(int x, int y, std::string cap);
    void draw() override;
    void onMouseEnter() override;
    void onMouseExit() override;
    void onMouseDown(vec2 pos) override;
    void onMouseUp(vec2 pos) override;
    void onClick() override;
    std::function<void()> onClickEvent = nullptr;
};

class _RootContainer : public Container
{
  public:
    _RootContainer();
    bool handleEvent(const GuiEvent& e) override;
    Widget* hoveredWidget;
};

class GuiComponent : public Component
{
  public:
    CTransform* transform;
    Widget* hoveredWidget = nullptr;
    Widget* activeWidget = nullptr;
    template <typename T, typename... Args> T* addChild(Args&&... args)
    {
        T* ptr = root.addChild<T>(std::forward<Args>(args)...);
        return ptr;
    }

    void init() override;
    void update(float dt) override;
    void drawGUI() override;
    bool isInteracting() const;

  private:
    _RootContainer root;
    void dispatchEvent(const GuiEvent& e);
};

#endif