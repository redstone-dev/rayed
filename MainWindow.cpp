#include <filesystem>
#include <iostream>
#include <raylib.h>

#include "Sidebar.cpp"
#include "EditorBar.cpp"

class MainWindow {
public:
    Vector2 screenSize = { 1920.f / 2, 1080.f / 2 };
    FileSidebar* sidebar;
    CodeEditor* editor;
    EditorBar* topbar;

    void init()
    {
        sidebar = new FileSidebar();
        sidebar->update_items();

        editor = new CodeEditor();
        editor->x = 204;
        editor->y = 20;

        topbar = new EditorBar();
        topbar->bounds = Rectangle{(float)editor->x, 0, GetScreenWidth() - (float)editor->x, (float)editor->y};

        auto sb = new SaveButton();
        sb->label = "Save";
        sb->currentEditor = editor;
        topbar->add(sb);

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
        topbar->update();
    }

    void draw()
    {
        BeginDrawing();

        ClearBackground(BLACK);
        sidebar->draw();
        editor->draw();
        if (sidebar->wasItemSelected) {
            editor->curPath = sidebar->selectedItem;
            editor->loadFile();
        }
        topbar->draw();

        auto text = "Current selection: " + sidebar->selectedItem.string();

        EndDrawing();
    }
};
