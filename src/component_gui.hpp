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

struct RectSizing
{
    RectSizing();
    RectSizing(int v);
    RectSizing(int x, int y);

    void set(int v);
    void setX(int v);
    void setY(int v);
    int left = 0;
    int top = 0;
    int right = 0;
    int bottom = 0;
};

enum class JustifyContent
{
    Start,
    End,
    Center,
    SpaceBetween,
    SpaceAround,
    SpaceEvenly
};

enum class AlignItems
{
    Start,
    End,
    Center,
    Stretch
};

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
    GuiComponent* system;

    bool growWidth = false;
    bool growHeight = false;

    Widget(Container* parent, int x, int y, int w, int h);
    virtual ~Widget() = default;

    virtual void draw() = 0;
    virtual void update(float dt);
    virtual bool isInsideInteractArea(const vec2& pos) const;
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
    Container* getRoot() const;
    virtual void applyTheme();
    const GuiTheme& theme() const;
    const Widget* getClosestFocusable() const;
    virtual void calcFitSize();
    virtual void calcGrowShrinkSize();
};

enum class LayoutType
{
    None,
    Row,
    Column
};

class Container : public Widget
{
  public:
    std::vector<std::unique_ptr<Widget>> children;
    LayoutType layout = LayoutType::None;
    JustifyContent justifyContent = JustifyContent::Start;
    AlignItems alignItems = AlignItems::Start;
    int gap = 5;
    int minWidth = 0;
    int minHeight = 0;
    RectSizing padding;
    Container(Container* parent);
    Container(Container* parent, int x, int y);
    Container(Container* parent, int x, int y, int w, int h);

    template <typename T, typename... Args> T* addChild(Args&&... args)
    {
        children.push_back(std::make_unique<T>(this, std::forward<Args>(args)...));
        T* ptr = static_cast<T*>(children.back().get());
        auto root = getRoot();
        root->calcFitSize();
        root->calcGrowShrinkSize();
        root->calcPosition();
        return ptr;
    }
    void update(float dt) override;
    void draw() override;
    Widget* handleEvent(const GuiEvent& e) override;
    void bringToFront(Widget* widget);
    void calcFitSize() override;
    void calcGrowShrinkSize() override;
    virtual void calcPosition();
    void applyTheme() override;
};

class Panel : public Container
{
  public:
    Panel(Container* parent, int x, int y, int w, int h);
    std::string title;
    bool hoveringDragHandle = false;
    bool dragging = false;
    vec2 dragOffset;
    vec2 dragStartPos;
    bool validDragStart = false;
    void draw() override;

    Color backgroundColor;
    void applyTheme() override;
};

class Window : public Container
{
  public:
    Window(Container* parent, int x, int y, int w, int h);
    std::string title;
    int headerHeight = 20;
    bool hoveringDragHandle = false;
    bool dragging = false;
    vec2 dragOffset;
    vec2 dragStartPos;
    bool validDragStart = false;
    void draw() override;
    Widget* handleEvent(const GuiEvent& e) override;
    bool isInsideInteractArea(const vec2& pos) const override;
    bool isOnDragHandle(const vec2& pos) const override;

    RectSizing headerPadding;
    Color headerBackgroundColor;
    Color headerBackgroundColorHover;
    Color headerBorderColor;
    Color headerTextColor;
    Color bodyBackgroundColor;
    Color bodyBorderColor;
    void applyTheme() override;
};

class Label : public Widget
{
  public:
    Label(Container* parent, int x, int y, int w, int h, std::string t);
    Label(Container* parent, int x, int y, std::string t);
    Label(Container* parent, std::string t);
    void draw() override;
    Color textColor;
    void applyTheme() override;
    const std::string& getText() const;
    void setText(const std::string& text);
    void setFontSize(int fontSize);

  private:
    std::string text;
    int fontSize;
};

class Button : public Widget
{
    bool clicking = false;

  public:
    Button(Container* parent, int x, int y, int w, int h, std::string cap);
    Button(Container* parent, int x, int y, std::string cap);
    Button(Container* parent, std::string cap);
    void draw() override;
    void onMouseDown(vec2 pos) override;
    void onMouseUp(vec2 pos) override;
    void onClick() override;
    std::function<void()> onClickEvent = nullptr;

    Color textColor;
    Color backgroundColor;
    Color backgroundColorHover;
    Color backgroundColorActive;
    Color borderColor;
    void applyTheme() override;

    const std::string& getText() const;
    void setText(const std::string& text);
    void setFontSize(int fontSize);

  private:
    std::string text;
    int fontSize;
};

class ProgressBar : public Widget
{
  public:
    ProgressBar(Container* parent, int x, int y, int w, int h);
    void draw() override;

    int maxValue = 10;
    int value = 2;
};

class Cursor : public Widget
{
  public:
    std::vector<Widget*> selectables;
    std::unordered_map<Widget*,std::function<void()>> actions;
    Widget* selected = nullptr;
    bool enabled = false;
    Cursor(Container* parent);
    void addSelectable(Widget* w, std::function<void()> action);
    void focusNext();
    void focusPrevious();
    void update(float dt) override;
    void draw() override;
};

class _RootContainer : public Container
{
  public:
    _RootContainer(GuiComponent* system);
    Widget* handleEvent(const GuiEvent& e) override;
    void update(float dt) override;
    void applyTheme() override;
    void calcFitSize() override;
    void calcPosition() override;
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

    static GuiTheme& Current();
    static void Set(const GuiTheme& theme);
    static GuiTheme& Dark();
    static GuiTheme& Light();
};

class GuiComponent : public Component
{
  public:
    GuiComponent();
    CTransform* transform;
    Widget* hoveredWidget = nullptr;
    Widget* activeWidget = nullptr;
    _RootContainer root;
    template <typename T, typename... Args> T* addChild(Args&&... args)
    {
        T* ptr = root.addChild<T>(std::forward<Args>(args)...);
        return ptr;
    }

    void init() override;
    void update(float dt) override;
    void drawGUI() override;
    bool isInteracting() const;
    void drawHoverAndActive(int x, int y) const;
    int drawHierarchy(Container* cont, int x, int y) const;

  private:
    void calcAll();
    void dispatchEvent(const GuiEvent& e);
};

#endif