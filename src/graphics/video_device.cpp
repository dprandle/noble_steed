#include <stdio.h>
#include <bx/bx.h>
#include <bx/spscqueue.h>
#include <bx/thread.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <noble_steed/logo.h>

#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
#include <GLFW/glfw3native.h>
#include <mat4x4.hpp>
#include <iostream>

#include <noble_steed/dbg.h>
#include <noble_steed/graphics/video_device.h>
#include <noble_steed/core/context.h>

static bx::DefaultAllocator s_allocator;
static bx::SpScUnboundedQueue s_apiThreadEvents(&s_allocator);

enum class EventType
{
    Exit,
    Key,
    Resize
};

struct KeyEvent
{
    EventType type = EventType::Key;
    int key;
    int action;
};

namespace noble_steed
{
Video_Device::Video_Device():
    window_(nullptr)
{}

Video_Device::~Video_Device()
{}

void Video_Device::init()
{
    
    // Create a GLFW window without an OpenGL context.
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        dbg("Can't init glfw!");
        return;
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window_ = glfwCreateWindow(1024, 768, "helloworld multithreaded", nullptr, nullptr);
    if (!window_)
    {
        dbg("Can't init window");
        return;
    }
    glfwSetKeyCallback(window_, glfw_key_press_callback);

}

void Video_Device::glfw_key_press_callback(GLFWwindow * window_, int32_t pKey, int32_t scancode_, int32_t action_, int32_t mods_)
{
    auto keyEvent = new KeyEvent;
    keyEvent->key = pKey;
    keyEvent->action = action_;
    s_apiThreadEvents.push(keyEvent);
}

void Video_Device::glfw_mouse_button_callback(GLFWwindow * window_, int32_t pButton, int32_t action_, int32_t mods_)
{

}

void Video_Device::glfw_cursor_pos_callback(GLFWwindow * window_, double x_pos, double y_pos)
{

}

void Video_Device::glfw_scroll_callback(GLFWwindow * window_, double x_offset, double y_offset)
{

}

void Video_Device::glfw_resize_window_callback(GLFWwindow* window, int32_t width, int32_t height)
{

}

void Video_Device::glfw_focus_change_callback(GLFWwindow* window, int give_or_taken)
{

}

void Video_Device::glfw_close_window_callback(GLFWwindow* window)
{

}

void Video_Device::glfw_minimize_window_callback(GLFWwindow * window, int min_or_restore)
{

}

void Video_Device::glfw_maximize_window_callback(GLFWwindow * window, int max_or_restore)
{

}

void Video_Device::glfw_window_position_callback(GLFWwindow * window, int x_pos, int y_pos)
{

}

void Video_Device::glfw_error_callback(int32_t error, const char * description)
{
    fprintf(stderr, "GLFW error %d: %s\n", error, description);
}


} // namespace noble_steed


struct ExitEvent
{
    EventType type = EventType::Exit;
};

struct ResizeEvent
{
    EventType type = EventType::Resize;
    uint32_t width;
    uint32_t height;
};

struct ApiThreadArgs
{
    bgfx::PlatformData platformData;
    uint32_t width;
    uint32_t height;
};

static int32_t runApiThread(bx::Thread * self, void * userData)
{
    auto args = (ApiThreadArgs *)userData;
    // Initialize bgfx using the native window handle and window resolution.
    bgfx::Init init;
    init.platformData = args->platformData;
    init.resolution.width = args->width;
    init.resolution.height = args->height;
    init.resolution.reset = BGFX_RESET_VSYNC;
    if (!bgfx::init(init))
        return 1;
    // Set view 0 to the same dimensions as the window and to clear the color buffer.
    const bgfx::ViewId kClearView = 0;
    bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR);
    bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
    uint32_t width = args->width;
    uint32_t height = args->height;
    bool showStats = false;
    bool exit = false;
    while (!exit)
    {
        // Handle events from the main thread.
        while (auto ev = (EventType *)s_apiThreadEvents.pop())
        {
            if (*ev == EventType::Key)
            {
                auto keyEvent = (KeyEvent *)ev;
                if (keyEvent->key == GLFW_KEY_F1 && keyEvent->action == GLFW_RELEASE)
                    showStats = !showStats;
            }
            else if (*ev == EventType::Resize)
            {
                auto resizeEvent = (ResizeEvent *)ev;
                bgfx::reset(resizeEvent->width, resizeEvent->height, BGFX_RESET_VSYNC);
                bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
                width = resizeEvent->width;
                height = resizeEvent->height;
            }
            else if (*ev == EventType::Exit)
            {
                exit = true;
            }
            delete ev;
        }
        // This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
        bgfx::touch(kClearView);
        // // Use debug font to print information about this example.
        bgfx::dbgTextClear();
        bgfx::dbgTextImage(bx::max<uint16_t>(uint16_t(width / 2 / 8), 20) - 20,
                           bx::max<uint16_t>(uint16_t(height / 2 / 16), 6) - 6,
                           40,
                           12,
                           s_logo,
                           160);
        bgfx::dbgTextPrintf(0, 0, 0x0f, "Press F1 to toggle stats.");
        bgfx::dbgTextPrintf(
            0,
            1,
            0x0f,
            "Color can be changed with ANSI "
            "\x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
        bgfx::dbgTextPrintf(80,
                            1,
                            0x0f,
                            "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; "
                            "5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
        bgfx::dbgTextPrintf(80,
                            2,
                            0x0f,
                            "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    "
                            "\x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");
        const bgfx::Stats * stats = bgfx::getStats();
        bgfx::dbgTextPrintf(0,
                            2,
                            0x0f,
                            "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.",
                            stats->width,
                            stats->height,
                            stats->textWidth,
                            stats->textHeight);
        // Enable stats or debug text.
        bgfx::setDebug(showStats ? BGFX_DEBUG_STATS : BGFX_DEBUG_TEXT);
        // Advance to next frame. Main thread will be kicked to process submitted rendering primitives.
        bgfx::frame();
    }
    bgfx::shutdown();
    return 0;
}

int GLFW_func(void)
{
    noble_steed::Context ctxt;    
    noble_steed::Video_Device device;
    noble_steed::System sys;
    sys.set_internal("Scooby");
    device.init();

    // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
    // Most graphics APIs must be used on the same thread that created the window.
    bgfx::renderFrame();
    // Create a thread to call the bgfx API from (except bgfx::renderFrame).
    ApiThreadArgs apiThreadArgs;

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    apiThreadArgs.platformData.ndt = glfwGetX11Display();
    apiThreadArgs.platformData.nwh = (void *)(uintptr_t)glfwGetX11Window(device.window_);
#elif BX_PLATFORM_OSXrttr_core
    apiThreadArgs.platformData.nwh = glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
    apiThreadArgs.platformData.nwh = glfwGetWin32Window(window);
#endif

    int width, height;
    glfwGetWindowSize(device.window_, &width, &height);
    apiThreadArgs.width = (uint32_t)width;
    apiThreadArgs.height = (uint32_t)height;
    bx::Thread apiThread;
    apiThread.init(runApiThread, &apiThreadArgs);
    
    // Run GLFW message pump.
    bool exit = false;
    while (!exit)
    {
        glfwPollEvents();
        // Send window close event to the API thread.
        if (glfwWindowShouldClose(device.window_))
        {
            s_apiThreadEvents.push(new ExitEvent);
            exit = true;
        }
        // Send window resize event to the API thread.
        int oldWidth = width, oldHeight = height;
        glfwGetWindowSize(device.window_, &width, &height);
        if (width != oldWidth || height != oldHeight)
        {
            auto resize = new ResizeEvent;
            resize->width = (uint32_t)width;
            resize->height = (uint32_t)height;
            s_apiThreadEvents.push(resize);
        }

        // Wait for the API thread to call bgfx::frame, then process submitted rendering primitives.
        bgfx::renderFrame();
    }
    // Wait for the API thread to finish before shutting down.
    while (bgfx::RenderFrame::NoContext != bgfx::renderFrame())
    {
    }
    apiThread.shutdown();
    glfwTerminate();
    return apiThread.getExitCode();
}