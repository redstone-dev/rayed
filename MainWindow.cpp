#include <filesystem>
#include <iostream>
#include <raylib.h>
#include <string>
#include "Sidebar.cpp"
// #include "raygui-4.0/src/raygui.h"z
// #include "raygui/src/raygui.h"
#include "debug.hpp"
#include "CodeEditor.cpp"

class MainWindow {
public:
    Vector2 screenSize = { 1920.f / 2, 1080.f / 2 };
    FileSidebar* sidebar;
    CodeEditor* editor;

    void init()
    {
        sidebar = new FileSidebar();
        sidebar->update_items();
        editor = new CodeEditor();
        editor->x = 204;
        SetWindowState(FLAG_WINDOW_RESIZABLE);
        // sidebar->yOffset = 20;
        //for (int i = 0; i < 11; i++) sidebar->items.push_back(std::to_string(i));
    }

    void update()
    {
        SetMouseCursor(MOUSE_CURSOR_ARROW);
        if (IsWindowResized()) {
            screenSize.x = GetScreenWidth();
            screenSize.y = GetScreenHeight();
        }
        editor->update();
        sidebar->update();
    }

    void draw()
    {
        BeginDrawing();

        ClearBackground(BLACK);
        editor->draw();
        sidebar->draw();
        if (sidebar->wasItemSelected) {
            std::cout << "Item selection changed\n";
            editor->curPath = sidebar->selectedItem;
            editor->loadFile();
        }

        auto text = "Current selection: " + sidebar->selectedItem.string();
        //DrawText(text.c_str(), screenSize.x - MeasureText(text.c_str(), 12) - 10, 0, 12, WHITE);

        auto sidebarWidth = (float)(sidebar->width + sidebar->BORDER_THICKNESS);
        //auto textInputBounds = Rectangle{sidebarWidth, 0, (float)(GetScreenWidth() - sidebarWidth), (float)GetScreenHeight()};
        //GuiTextBox(textInputBounds, (char*)"test", 32, true);

        EndDrawing();
    }
};
