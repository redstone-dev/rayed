#include <string>
#include <raylib.h>
#include <filesystem>
#include <fstream>
#include <vector>
#include <iostream>
#include "SyntaxHighlighter.cpp"

int globalCodeEditorFontSize = 20;

class CodeEditor {
    public:
    std::string buffer;
    std::string::size_type longestLineLength = 0;
    std::filesystem::path curPath;
    Font font;

    int x; int y;

    // The cursor position can never be negative
    size_t cursorPos; // Changed from unsigned long to size_t
    long scrollHeight;
    float scrollSens = 20.f;

    int tabToSpaces = 4;

    Vector2 charSpacing = { 0.5f, 1.5f };
    char xPadding;
    enum CursorStyle { VERTICAL, UNDER } cursorStyle;
    Parser* parser;

    CodeEditor() {
        font = LoadFontEx("fonts/MartianMono-Regular.ttf", globalCodeEditorFontSize, NULL, 0);
        cursorStyle = VERTICAL;
        parser = new Parser("plugins/grammars/html.grr");
    }

    void loadFile() {
        buffer.clear();
        std::ifstream file;
        file.open(curPath);
        std::string line;
        if (file.is_open()) {
            while (std::getline(file, line)) {
                buffer += line + '\n';
                if (line.length() > longestLineLength) longestLineLength = line.length();
            }
            file.close();
        }
        parser->createRules();
        parser->createHighlights(buffer);
    }

    bool IsKeyPressedAndRepeat(KeyboardKey key) {
        return IsKeyPressed(key) || IsKeyPressedRepeat(key);
    }

    std::string getLineContainsIdx(long idx, char delim) {
        long start = idx, end = idx;
        while((buffer[start] != delim && buffer[end] != delim) || (start != 0 && end == (long)buffer.size())) {
            if (buffer[start] != delim) start--;
            if (buffer[end]   != delim) end++;
            if (start < 0) start = 0;
            if (end > (long)buffer.size()) end = (long)buffer.size();
        }
        //start++; end--;
        return buffer.substr(start, end - start);
    }

    void update() {
        // Check if the cursor is within the code editor
        Vector2 mousePos = GetMousePosition();
        bool isCursorInsideEditor = mousePos.x >= x && mousePos.x <= GetScreenWidth() &&
                                    mousePos.y >= y && mousePos.y <= GetScreenHeight();
        if (isCursorInsideEditor) {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
        }

        scrollHeight += GetMouseWheelMoveV().y * -scrollSens;
        if (scrollHeight < 0) scrollHeight = 0; // User can't scroll back further than the beginning of the list

        // Handle special keypresses
        if (IsKeyPressedAndRepeat(KEY_RIGHT)) {
            cursorPos++;
            return;
        }
        if (IsKeyPressedAndRepeat(KEY_LEFT) && cursorPos > 0) {
            cursorPos--;
            return;
        }
        if (IsKeyPressedAndRepeat(KEY_BACKSPACE) && cursorPos > 0) {
            buffer.erase(buffer.begin() + --cursorPos);
            parser->createRules();
            parser->createHighlights(buffer);
            return;
        }
        if (IsKeyPressedAndRepeat(KEY_ENTER)) {
            buffer.insert(buffer.begin() + cursorPos++, '\n');
            parser->createRules();
            parser->createHighlights(buffer);
            return;
        }
        // TODO: fix line jumping
        // if (IsKeyPressedAndRepeat(KEY_DOWN)) {
        //     std::cout << getLineContainsIdx(cursorPos, '\n') << std::endl;
        // }
        // Otherwise, type it in
        auto key = (char)GetCharPressed();
        if (key) {
            buffer.insert(buffer.begin() + cursorPos++, key);
            parser->createRules();
            parser->createHighlights(buffer);
        }
    }

    void drawCursor(Vector2 pos) {
        switch(cursorStyle) {
            case VERTICAL:
                DrawRectangle(pos.x + globalCodeEditorFontSize * charSpacing.x, pos.y, 2, globalCodeEditorFontSize, WHITE);
                break;
            case UNDER:
                DrawRectangle(pos.x, pos.y + globalCodeEditorFontSize, globalCodeEditorFontSize * charSpacing.x, 2, WHITE);
                break;
        }
    }

    void draw() {
        DrawRectangle(x, y, GetScreenWidth() - x, GetScreenHeight() - y, BLACK);
        int rows = 0;
        int columns = 0;
        // Character count cannot be negative
        unsigned long charCount = 0;
        for (char c : buffer) {
            charCount++;
            auto span = parser->spanAt(charCount);
            Color syntaxHighlight = span.colour;
            if (c == '\n') {
                rows++;
                columns = 0;
                continue;
            }
            if (c == '\t') {
                columns += tabToSpaces;
                continue;
            }
            auto charPos = Vector2{x + (columns * (globalCodeEditorFontSize * charSpacing.x)),
                                   y - scrollHeight + (rows * globalCodeEditorFontSize * charSpacing.y)};
            if (charPos.y < (float)y || charPos.y > (float)y + ((float)GetScreenHeight() - (float)y)) {columns++; continue;};
            //if (scrollHeight > charPos.y) continue;
            DrawTextEx(font, (std::string() + c).c_str(), charPos, (float)globalCodeEditorFontSize, (float)globalCodeEditorFontSize * 1.5f, syntaxHighlight);
            if (charCount == cursorPos) drawCursor(charPos);
            columns++;
        }
    }

    void uninit() {
        UnloadFont(font);
    }
};
