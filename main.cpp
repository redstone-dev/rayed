#include <raylib.h>
#include "MainWindow.cpp"
#include "debug.hpp"
//#include <iostream>

int main()
{
    auto mainWindow = new MainWindow();

    InitWindow((int)(mainWindow->screenSize.x), (int)(mainWindow->screenSize.y), "rayed");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    mainWindow->init();

    while (!WindowShouldClose()) {
        mainWindow->update();
        mainWindow->draw();
    }

    return 0;
}
