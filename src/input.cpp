#include "input.h"

#include <cstring>

Input input;

static Key GLFWToKey(i32 key) {
    if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
        return (Key)((key - GLFW_KEY_A) + KeyA);
    }

    switch (key) {
        case GLFW_KEY_LEFT: return KeyLeft;
        case GLFW_KEY_RIGHT: return KeyRight;
        case GLFW_KEY_UP: return KeyUp;
        case GLFW_KEY_DOWN: return KeyDown;
        case GLFW_KEY_SPACE: return KeySpace;
        case GLFW_KEY_TAB: return KeyTab;
        case GLFW_KEY_ENTER: return KeyEnter;
        case GLFW_KEY_BACKSPACE: return KeyBackspace;
        case GLFW_KEY_ESCAPE: return KeyEscape;
        default: return KeyUnknown;
    }
}

void InitializeInput(GLFWwindow *window) {
    input.window = window;

    glfwSetKeyCallback(window, [](GLFWwindow *window, i32 key, i32 scancode, i32 action, i32 mods) {
        i32 k = GLFWToKey(key);
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            if (!input.keyPressed[k]) {
                input.keyPressed[k] = true;
            }
            input.keyDown[k] = true;
        } else if (action == GLFW_RELEASE) {
            input.keyDown[k] = false;
            input.keyPressed[k] = false;
        }
    });

    input.mouseMoved = false;
    f64 x, y;
    glfwGetCursorPos(window, &x, &y);
    input.xMouse = x;
    input.yMouse = y;

    glfwSetCursorPosCallback(window, [](GLFWwindow *wnd, f64 xpos, f64 ypos) {
        input.xMouse = xpos;
        input.yMouse = ypos;
        input.mouseMoved = true;
    });
}

void UpdateInput() {
    if (input.keyPressed[KeyEscape]) {
        glfwSetWindowShouldClose(input.window, true);
    }

    std::memset(input.keyPressed, 0, KeyCount);

    input.mouseMoved = false;
}
