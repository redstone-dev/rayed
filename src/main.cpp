#include <raylib.h>
#include "MainWindow.cpp"
#include "debug.hpp"
//#include <iostream>

int main()
{
    auto mainWindow = new MainWindow();

    InitWindow((int)(mainWindow->screenSize.x), (int)(mainWindow->screenSize.y), "rayed");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    SetExitKey(0);

    mainWindow->init();

    while (!WindowShouldClose()) {
        mainWindow->update();
        mainWindow->draw();
    }

    mainWindow->editor->uninit();

    delete mainWindow;

    return 0;
}
