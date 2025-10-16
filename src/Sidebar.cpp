#include <raylib.h>
#include <vector>
#include <filesystem>
#include "debug.hpp"

#ifndef CURSOR_STATE
#include "CursorState.cpp"
#endif

#ifdef DEBUG
#include <iostream>
#endif

using namespace std::filesystem;

int globalSidebarFontSize = 20;

template<class T>
class Sidebar {
public:
    std::string title;
    int width;
    int yOffset;
    int itemHeight = globalSidebarFontSize + 2;

    const int BORDER_THICKNESS = 4;

    int scrollHeight;
    float scrollSens;

    std::vector<T> items;
    T selectedItem;

    bool wasItemSelected = false;

    virtual void update() {
        wasItemSelected = false;

        // Ensure width is properly initialized
        if (width <= 0) {
            width = 200; // Default width if not set
        }

        // Check if the cursor is within the sidebar
        Vector2 mousePos = GetMousePosition();
        bool isCursorInsideSidebar = mousePos.x >= 0 && mousePos.x <= width &&
                                     mousePos.y >= yOffset && mousePos.y <= GetScreenHeight();

        bool isHoveringItem = false;
        for (size_t i = 0; i < items.size(); ++i) {
            auto currentY = yOffset + (itemHeight * (i + 1)) - scrollHeight;
            auto itemBoundingRect = Rectangle{0, (float)currentY, (float)width, (float)itemHeight};
            if (CheckCollisionPointRec(mousePos, itemBoundingRect)) {
                isHoveringItem = true;
                break;
            }
        }

        if (isCursorInsideSidebar && !isHoveringItem) {
            CursorState::mouseCursor = MOUSE_CURSOR_DEFAULT;
        }

        scrollHeight += GetMouseWheelMoveV().y * -scrollSens;
        if (scrollHeight < 0) scrollHeight = 0; // User can't scroll back further than the beginning of the list
        after_update();
    }

    virtual void after_update() {}

    // Draws a sidebar item
    void draw_item(int idx, const char* label, Color colour) {
        auto currentY = yOffset + (itemHeight * idx) - scrollHeight;

        auto itemBoundingRect = Rectangle{0, (float)currentY, 200, (float)itemHeight};
        //     Hardcoding this is the only way it works. Evil ^

        // Draw mouseover highlight (if any)
        if (CheckCollisionPointRec(GetMousePosition(), itemBoundingRect)) {
            DrawRectangleRec(itemBoundingRect, DARKGRAY);
            CursorState::mouseCursor = MOUSE_CURSOR_POINTING_HAND;
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                selectedItem = items[idx - 1];
                wasItemSelected = true;
            }
        }
        // Draw selection highlight (if this item is selected)
        if (selectedItem == items[idx - 1]) DrawRectangleRec(itemBoundingRect, GRAY);

        // Item label text
        DrawText(label, yOffset, currentY, globalSidebarFontSize, colour);

    }

    virtual void draw() {
        DrawRectangle(width, 0, BORDER_THICKNESS, GetScreenHeight(), GRAY);
    }
};

class FileSidebar : public Sidebar<path> {
public:
    const Color FILE_COLOUR = WHITE;
    const Color DIR_COLOUR  = LIME;
    const Color EXE_COLOUR  = SKYBLUE;

    std::string title = "Getting pwd...";
    int width = 200;
    float scrollSens = 4.f;

    path currentDir = current_path().append("src");

    void after_update() {
        title = "Files in " + currentDir.stem().string();
        int i = 0;
        for (path path : items) {
            auto currentY = yOffset + (itemHeight * ++i) - scrollHeight;
            auto itemBoundingRect = Rectangle{0, (float)currentY, 200, (float)itemHeight};
            //     Hardcoding this is the only way it works. Evil ^

            // Draw mouseover highlight (if any)
            if (CheckCollisionPointRec(GetMousePosition(), itemBoundingRect)
                && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)
                && is_directory(path)) {
                currentDir = path;
                update_items();
            }
        }
    }

    void update_items() {
        items.clear();
        for (const auto& file : directory_iterator(currentDir))
            items.push_back(file.path());
    }

    void draw() {
        DrawRectangle(width, yOffset, BORDER_THICKNESS, GetScreenHeight(), GRAY);
        int i = 0;
        for (path path : items) {
            if (!path.has_filename()) continue;
            auto label = path.filename().string();
            if (is_directory(path)) label += "/";
            draw_item(++i, label.c_str(), is_directory(path) ? DIR_COLOUR : FILE_COLOUR);
        }
        // Draw title
        DrawRectangle(0, yOffset, width, itemHeight, DARKGRAY);
        DrawText(title.c_str(), 0, yOffset, globalSidebarFontSize * 0.8f, WHITE);

        // draw little button to go to parent directory
        auto parentDirBtnBounds = Rectangle { (float)width - 20, 0, 20, 20 };
        DrawRectangleRec(parentDirBtnBounds, GRAY);
        if (CheckCollisionPointRec(GetMousePosition(), parentDirBtnBounds)) {
            DrawRectangle(parentDirBtnBounds.x + 5, parentDirBtnBounds.y,
                          parentDirBtnBounds.width - 5, parentDirBtnBounds.height, DARKGRAY);
            if (IsMouseButtonReleased(0)) {
                currentDir = currentDir.parent_path();
                update_items();
            }
        }
        DrawText("..", width - 20, 0, globalSidebarFontSize * 0.8f, WHITE);

    }
};
