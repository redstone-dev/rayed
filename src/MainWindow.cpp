#include <filesystem>
#include <iostream>
#include <raylib.h>

#include "Sidebar.cpp"
#include "EditorBar.cpp"
#include "SyntaxHighlighter.cpp"

class MainWindow {
public:
    Vector2 screenSize = { 1920.f / 2, 1080.f / 2 };
    FileSidebar* sidebar;
    CodeEditor*  editor;
    SaveButton*  saveBtn;
    Parser*      parser;

    void init()
    {
        sidebar = new FileSidebar();
        sidebar->update_items();

        editor = new CodeEditor();
        editor->x = 204;
        editor->y = 20;

        saveBtn = new SaveButton();
        saveBtn->label = "Save";
        saveBtn->currentEditor = editor;
        saveBtn->pos.x = editor->x;
        saveBtn->pos.y = 0;
        saveBtn->height = editor->y;

        parser = new Parser("build/plugins/grammars/html.grammar");
        parser->createRules();

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

        if (IsKeyReleased(KEY_F11))
            parser->highlightText(editor->buffer);
        sidebar->update();
        saveBtn->update();
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
        saveBtn->draw();

        auto text = "Current selection: " + sidebar->selectedItem.string();

        EndDrawing();
    }
};
