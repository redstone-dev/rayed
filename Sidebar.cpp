#include <raylib.h>
#include <vector>
#include <filesystem>

using namespace std::filesystem;

template<class T>
class Sidebar {
public:
    int width;
    int itemHeight = 16;

    const int BORDER_THICKNESS = 4;

    int scrollHeight;
    float scrollSens;

    std::vector<T> items;
    T selectedItem;

    void update() {
        scrollHeight += GetMouseWheelMoveV().y * -scrollSens;
        if (scrollHeight < 1) scrollHeight = 0;
    }

    void draw_item(int idx, const char* label) {
        auto currentY = (itemHeight * idx) - scrollHeight;
        auto itemBoundingRect = Rectangle{0, (float)currentY, (float)width, (float)itemHeight};
        if (CheckCollisionPointRec(GetMousePosition(), itemBoundingRect)) {
            DrawRectangleRec(itemBoundingRect, DARKGRAY);
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) selectedItem = items[idx - 1];
        }
        if (selectedItem == items[idx - 1]) DrawRectangleRec(itemBoundingRect, GRAY);
        DrawText(label, 0, currentY, 12, WHITE);

    }

    virtual void draw() {
        DrawRectangle(width, 0, BORDER_THICKNESS, GetScreenHeight(), GRAY);
    }
};

class FileSidebar : public Sidebar<path> {
public:
    void draw() {
        DrawRectangle(width, 0, BORDER_THICKNESS, GetScreenHeight(), GRAY);
        int i = 0;
        for (path path : items) {
            if (!path.has_filename()) continue;
            draw_item(++i, path.filename().c_str());
        }
    }
};
