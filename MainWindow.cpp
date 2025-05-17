#include <filesystem>
#include <raylib.h>
#include <string>
#include "Sidebar.cpp"
#include "raygui-4.0/src/raygui.h"

class MainWindow {
public:
    Vector2 screenSize = { 1920.f / 2, 1080.f / 2 };
    FileSidebar* sidebar;

    void init()
    {
        sidebar = new FileSidebar();
        sidebar->width = 200;
        for (int i = 0; i < 11; i++) sidebar->items.push_back(std::to_string(i));
        sidebar->scrollSens = 4.f;
    }

    void update()
    {
        SetMouseCursor(MOUSE_CURSOR_ARROW);
        if (IsWindowResized()) {
            screenSize.x = GetScreenWidth();
            screenSize.y = GetScreenHeight();
        }
        sidebar->update();
    }

    void draw()
    {
        BeginDrawing();

        ClearBackground(BLACK);
        sidebar->draw();

        auto text = "Current selection: " + sidebar->selectedItem.string();
        DrawText(text.c_str(), screenSize.x - MeasureText(text.c_str(), 12) - 10, 0, 12, WHITE);

        EndDrawing();
    }
};
