#include <raylib.h>
#include <vector>
#include <filesystem>

using namespace std::filesystem;

int globalSidebarFontSize = 20;

template<class T>
class Sidebar {
public:
    std::string title;
    int width;
    int itemHeight = globalSidebarFontSize + 2;

    const int BORDER_THICKNESS = 4;

    int scrollHeight;
    float scrollSens;

    std::vector<T> items;
    T selectedItem;

    virtual void update() {
        scrollHeight += GetMouseWheelMoveV().y * -scrollSens;
        if (scrollHeight < 1) scrollHeight = 0; // User can't scroll back further than the beginning of the list
    }

    virtual void after_update() {}

    // Draws a sidebar item
    void draw_item(int idx, const char* label) {
        auto currentY = (itemHeight * idx) - scrollHeight;

        auto itemBoundingRect = Rectangle{0, (float)currentY, (float)width, (float)itemHeight};

        // Draw mouseover highlight (if any)
        if (CheckCollisionPointRec(GetMousePosition(), itemBoundingRect)) {
            DrawRectangleRec(itemBoundingRect, DARKGRAY);
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) selectedItem = items[idx - 1];
        }
        // Draw selection highlight (if this item is selected)
        if (selectedItem == items[idx - 1]) DrawRectangleRec(itemBoundingRect, GRAY);

        // Item label text
        DrawText(label, 0, currentY, globalSidebarFontSize, WHITE);

    }

    virtual void draw() {
        DrawRectangle(width, 0, BORDER_THICKNESS, GetScreenHeight(), GRAY);
    }
};

class FileSidebar : public Sidebar<path> {
public:
    std::string title = "Getting pwd...";
    int width = 200;
    float scrollSens = 4.f;

    path currentDir = current_path();

    void after_update() {
        title = "Files in " + currentDir.stem().string();
    }

    void draw() {
        DrawRectangle(width, 0, BORDER_THICKNESS, GetScreenHeight(), GRAY);
        int i = 0;
        for (path path : items) {
            if (!path.has_filename()) continue;
            draw_item(++i, path.filename().c_str());
        }
        // Draw title
        DrawRectangle(0, 0, width, itemHeight, DARKGRAY);
        DrawText(title.c_str(), 0, 0, globalSidebarFontSize, WHITE);
    }
};
