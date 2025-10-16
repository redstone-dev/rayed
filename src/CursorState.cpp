#include <raylib.h>

#define CURSOR_STATE

class CursorState {
public:
    static int mouseCursor;

    static void init() {
        mouseCursor = MOUSE_CURSOR_ARROW;
    }

    static void update() {
        SetMouseCursor(mouseCursor);
    }
};
