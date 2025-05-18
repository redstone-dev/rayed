#include <string>
#include <raylib.h>
#include <filesystem>
#include <fstream>

int globalCodeEditorFontSize = 20;

class CodeEditor {
    public:
    std::string buffer;
    std::filesystem::path curPath;

    int x; int y;

    unsigned long cursorPos;
    long scrollHeight;
    float scrollSens = 20.f;

    void loadFile() {
        buffer.clear();
        std::ifstream file;
        file.open(curPath);
        std::string line;
        if (file.is_open()) {
            while (std::getline(file, line)) buffer += line + '\n';
            file.close();
        }
    }

    void update() {
        scrollHeight += GetMouseWheelMoveV().y * -scrollSens;
        if (scrollHeight < 0) scrollHeight = 0; // User can't scroll back further than the beginning of the list
    }

    void draw() {
        SetTextLineSpacing((int)(1.5f * globalCodeEditorFontSize));
        DrawText(buffer.c_str(), x, y - scrollHeight, globalCodeEditorFontSize, WHITE);
    }
};
