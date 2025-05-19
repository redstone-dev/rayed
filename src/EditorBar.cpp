#include <iostream>
#include <raylib.h>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include "CodeEditor.cpp"

int globalEditorBarFontSize = 20 * 0.8f;

class SaveButton {
    public:
    Vector2 pos;
    std::string label = "Save file";
    std::time_t lastSavedTime = 0;
    CodeEditor* currentEditor;
    float fwidth = 200;
    float height;

    void update() {

        std::time_t now = std::time(nullptr);
        if (lastSavedTime < now) {
            label = "Save file";
        }
        else {
            label = "Saved";
        }
        auto btnBoundingRec = Rectangle{
            pos.x,
            pos.y,
            fwidth,
            height
        };
        if (CheckCollisionPointRec(GetMousePosition(), btnBoundingRec) && IsMouseButtonReleased(0))
        {
            // save edits to file
            action();
        }
    }

    void action() {
        // Save edits to current file
        std::cout << "saving to " << currentEditor->curPath << "\n";
        std::ofstream file(currentEditor->curPath);
        file << currentEditor->buffer;
        file.close();
    }

    void draw() {
        auto btnBoundingRec = Rectangle{
            pos.x,
            pos.y,
            fwidth,
            height
        };
        if (CheckCollisionPointRec(GetMousePosition(), btnBoundingRec)) {
            DrawRectangleRec(btnBoundingRec, BLACK);
        }
        else
            DrawRectangleRec(btnBoundingRec, DARKGRAY);

        DrawText(label.c_str(), pos.x, pos.y, globalEditorBarFontSize, WHITE);
    }
};

class EditorBar {
public:
    Rectangle bounds;
    std::vector<SaveButton*> buttons;

    void add(SaveButton* button) {
        // TODO: make other buttons' x positions go to right edge of the previous one

        // if (buttons.size() - 2 > -1)
            // button->pos.x  = bounds.x + buttons[(int)buttons.size() - 2]->width();
        // else
        button->pos.x  = bounds.x;
        button->pos.y  = bounds.y;
        button->height = bounds.height;
        buttons.push_back(button);
    }

    void update() {
        for (auto button : buttons) button->update();
    }

    void draw() {
        DrawRectangleRec(bounds, RED);
        for (auto button : buttons) button->draw();
    }

};
