#pragma once
#include <EGL/egl.h>
#include <X11/Xlib.h>

struct WindowProperties
{
    int x, y;
    unsigned int width, height;
    unsigned int borderWidth;
    unsigned long borderColor, backgroundColor;

    WindowProperties(
        unsigned int width = 200,
        unsigned int height = 150,
        int x = 0,
        int y = 0,
        unsigned int borderWidth = 15,
        unsigned long borderColor = 0x000000,
        unsigned long backgroundColor = 0x00ff00
    ) : x(x), y(y)
        , width(width), height(height)
        , borderWidth(borderWidth)
        , borderColor(borderColor)
        , backgroundColor(backgroundColor)
    {
    }
};

class X11Window
{
    EGLSurface surface;
    Window nativeWindow;
    Display* display;
    EGLDisplay eglDisplay;
    XEvent xev;
    int screen;
    WindowProperties properties;

    XEvent event;
    KeySym key;
    char text[255];

public:
    X11Window(WindowProperties properties = WindowProperties());
    ~X11Window();

    EGLBoolean InitEGL();

    char KeyboardHandler();
    
    EGLDisplay GetEglDisplay() const { return eglDisplay; }
    Display* GetDisplay() const { return display; }
    Window GetNativeWindow() const { return nativeWindow; }
    int GetScreen() const { return screen; }
    EGLSurface GetSurface() const { return surface; }

};