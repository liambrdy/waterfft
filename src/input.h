#ifndef INPUT_H_
#define INPUT_H_

#include "types.h"

#include <GLFW/glfw3.h>

enum Key {
KeyUnknown,
KeyA,
KeyB,
KeyC,
KeyD,
KeyE,
KeyF,
KeyG,
KeyH,
KeyI,
KeyJ,
KeyK,
KeyL,
KeyM,
KeyN,
KeyO,
KeyP,
KeyQ,
KeyR,
KeyS,
KeyT,
KeyU,
KeyV,
KeyW,
KeyX,
KeyY,
KeyZ,
KeyLeft,
KeyRight,
KeyUp,
KeyDown,
KeySpace,
KeyTab,
KeyEnter,
KeyBackspace,
KeyEscape,
KeyCount,
};

struct Input {
    b8 keyDown[KeyCount];
    b8 keyPressed[KeyCount];

    GLFWwindow *window;
};

void InitializeInput(GLFWwindow *window);
void UpdateInput();

extern Input input;

#endif // INPUT_H_
