#include <cstdio>
#include <cassert>
#include <vector>
#include <random>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

#include "shader.h"
#include "mesh.h"
#include "camera.h"
#include "texture.h"
#include "input.h"

#include "h0k.h"
#include "hkt.h"
#include "twiddleFactors.h"
#include "fft.h"

#include "normalRenderer.h"

#include "skybox.h"

#include "framebuffer.h"
#include "renderer.h"

#include "water.h"

void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            unsigned int id,
                            GLenum severity,
                            GLsizei length,
                            const char *message,
                            const void *userParam) {
    std::string err;
    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             err +="[API]"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   err +="[Window System]"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: err +="[Shader Compiler]"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     err +="[Third Party]"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     err +="[Application]"; break;
        case GL_DEBUG_SOURCE_OTHER:           err +="[Other]"; break;
    } err += " ";

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               err += "[Error]"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: err += "[Deprecated Behaviour]"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  err += "[Undefined Behaviour]"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         err += "[Portability]"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         err += "[Performance]"; break;
        case GL_DEBUG_TYPE_MARKER:              err += "[Marker]"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          err += "[Push Group]"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           err += "[Pop Group]"; break;
        case GL_DEBUG_TYPE_OTHER:               err += "[Other]"; break;
    } err += " ";

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         err += "[high]"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       err += "[medium]"; break;
        case GL_DEBUG_SEVERITY_LOW:          err += "[low]"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: err += "[notification]"; break;
    }
    std::printf("%s\n", err.c_str());
    std::printf("Debug message (%d): %s\n", id, message);
}

int main(int argc, char *argv[]) {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    //glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    GLFWwindow *window = glfwCreateWindow(1280, 720, "Water", nullptr, nullptr);
    assert(window);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    i32 w, h;
    glfwGetWindowSize(window, &w, &h);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    RendererInit(w, h);
    InitializeInput(window);

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    glEnable(GL_DEPTH_TEST);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //glm::mat4 world = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 5.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(150.0f));

    Skybox skybox;
    SkyboxCreate(&skybox, {
            "res/skybox/right.jpg",
            "res/skybox/left.jpg",
            "res/skybox/top.jpg",
            "res/skybox/bottom.jpg",
            "res/skybox/front.jpg",
            "res/skybox/back.jpg"
        });

    f32 ZFAR = 10000.0f;

    Water water;
    water.transform = glm::scale(glm::mat4(1.0f), glm::vec3(ZFAR * 1.95f, 1.0f, ZFAR * 1.95f)) * glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, -0.5f));
    water.waterConfig.choppy = true;
    WaterCreate(&water, 128);

    auto &&render = [&]() {
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        SkyboxRender(&skybox, &renderer.camera);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    };

    while (!glfwWindowShouldClose(window)) {
        UpdateInput();
        glfwPollEvents();

        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RendererBeginFrame();

        OffscreenFramebufferBind(&renderer.primaryFbo);
        FramebufferClear();

        WaterUpdate(&water);

        WaterRender(&water, &renderer.camera, render);

        OffscreenFramebufferBind(&renderer.primaryFbo);
        render();

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        RendererEndFrame();

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
