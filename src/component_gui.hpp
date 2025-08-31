#ifndef COMPONENT_GUI_HPP
#define COMPONENT_GUI_HPP

#include "entity.hpp"
#include "util.hpp"
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
class GuiComponent;
class GuiTheme;

class Widget
{
  public:
    Rect rectInit; // initial rect values
    Rect rect;     // used to draw, bounds check and handled by containers
    bool visible = true;
    bool draggable = false;
    bool focusable = false;
    Container* parent = nullptr;
    bool isContainer = false;
    bool hovered = false;
    const GuiComponent* system;

    Widget(int x, int y, int w, int h);
    virtual ~Widget() = default;

    virtual void draw() = 0;
    virtual void update(float dt);
    virtual bool isOnDragHandle(const vec2& pos) const;
    virtual Widget* handleEvent(const GuiEvent& e);
    virtual void onMouseEnter();
    virtual void onMouseExit();
    virtual void onMouseDown(vec2 pos);
    virtual void onMouseUp(vec2 pos);
    virtual void onClick();
    virtual void onDragStart(vec2 pos);
    virtual void onDrag(vec2 pos);
    virtual void onDragEnd(vec2 pos);
    const Widget* getRoot() const;
    const GuiTheme& theme() const;
    const Widget* getClosestFocusable() const;
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
        ptr->system = getRoot()->system;
        this->arrange();
        return ptr;
    }
    void update(float dt) override;
    void draw() override;
    Widget* handleEvent(const GuiEvent& e) override;
    virtual void arrange();
    void bringToFront(Widget* widget);
};

class Window : public Container
{
  public:
    Window(int x, int y, int w, int h);
    std::string title;
    int headerHeight = 20;
    bool hoveringDragHandle = false;
    bool dragging = false;
    vec2 dragOffset;
    vec2 dragStartPos;
    bool validDragStart = false;
    void update(float dt) override;
    void draw() override;
    void arrange() override;
    Widget* handleEvent(const GuiEvent& e) override;
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
    bool clicking = false;

  public:
    std::string caption;
    Button(int x, int y, int w, int h, std::string cap);
    Button(int x, int y, std::string cap);
    Button(std::string cap);
    void draw() override;
    void onMouseDown(vec2 pos) override;
    void onMouseUp(vec2 pos) override;
    void onClick() override;
    std::function<void()> onClickEvent = nullptr;
};

class _RootContainer : public Container
{
  public:
    _RootContainer(const GuiComponent* system);
    Widget* handleEvent(const GuiEvent& e) override;
};

struct GuiColorSet
{
    Color normal;
    Color hover;
    Color active;
    Color disabled;
};

struct GuiTheme
{
    int fontSize = 14;

    // Global colors
    Color textColor = {220};
    Color textDisabledColor = {128};

    Color windowBg = {30};
    Color childBg = {35};
    Color popupBg = {40};

    Color borderColor = {80};
    Color shadowColor = {0, 120};

    // Widgets
    GuiColorSet button = {{60}, {90}, {110}, {40}};
    GuiColorSet checkbox = {{50}, {70}, {90}, {35}};
    GuiColorSet panelHeader = {{45}, {65}, {90}, {25}};
    GuiColorSet inputField = {{55}, {70}, {95}, {30}};
    GuiColorSet slider = {{70}, {100}, {130}, {50}};
    GuiColorSet scrollbar = {{60}, {80}, {100}, {40, 180}};

    // Layout
    int windowPaddingX = 8;
    int windowPaddingY = 8;
    int framePaddingX = 6;
    int framePaddingY = 4;
    int itemSpacingX = 8;
    int itemSpacingy = 6;

    // Borders / Rounding
    float borderThickness = 1.0f;
    float cornerRadius = 4.0f;

    // transparency
    float windowAlpha = 1.0f;

    GuiTheme() = default;

    static const GuiTheme& Dark();
    static const GuiTheme& Light();
};

class GuiComponent : public Component
{
  public:
    GuiComponent();
    CTransform* transform;
    Widget* hoveredWidget = nullptr;
    Widget* activeWidget = nullptr;
    const GuiTheme* mTheme;
    template <typename T, typename... Args> T* addChild(Args&&... args)
    {
        T* ptr = root.addChild<T>(std::forward<Args>(args)...);
        return ptr;
    }

    void init() override;
    void update(float dt) override;
    void drawGUI() override;
    bool isInteracting() const;

    void setTheme(const GuiTheme& theme);
    const GuiTheme* theme() const;

  private:
    _RootContainer root;
    void dispatchEvent(const GuiEvent& e);
};

#endif