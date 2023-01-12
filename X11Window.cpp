#include "X11Window.h"

#include <stdexcept>
#include <string.h>


X11Window::X11Window(WindowProperties properties)
    : properties(properties)
{
    static Atom s_wmDeleteMessage;
    XSetWindowAttributes swa;
    XSetWindowAttributes  xattr;
    Atom wm_state;
    XWMHints hints;

    EGLConfig ecfg;
    EGLint num_config;   

    display = XOpenDisplay(NULL);
    if (display == NULL)
    {
        //return EGL_FALSE;
    }    

    nativeWindow = DefaultRootWindow(display);

    swa.event_mask = ExposureMask | PointerMotionMask | KeyPressMask;
    nativeWindow = XCreateWindow(
        display, nativeWindow,
        0, 0, properties.width, properties.height, 0,
        CopyFromParent, InputOutput,
        CopyFromParent, CWEventMask,
        &swa);
    s_wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, nativeWindow, &s_wmDeleteMessage, 1);

    xattr.override_redirect = 0;
    XChangeWindowAttributes(display, nativeWindow, CWOverrideRedirect, &xattr);

    hints.input = 1;
    hints.flags = InputHint;
    XSetWMHints(display, nativeWindow, &hints);
    //XSelectInput(display, nativeWindow, KeyPressMask);

    // make the window visible on the screen
    XMapWindow(display, nativeWindow);
    XStoreName(display, nativeWindow, "RTS3D");

    // get identifiers for the provided atom name strings
    wm_state = XInternAtom(display, "_NET_WM_STATE", 0);

    memset(&xev, 0, sizeof(xev));
    xev.type = ClientMessage;
    xev.xclient.window = nativeWindow;
    xev.xclient.message_type = wm_state;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = 1;
    xev.xclient.data.l[1] = 0;
    XSendEvent(
        display,
        DefaultRootWindow(display),
        0,
        SubstructureNotifyMask,
        &xev);
}

X11Window::~X11Window()
{
    XCloseDisplay(display);
}

char X11Window::KeyboardHandler()
{
    XNextEvent(display, &event);
  	if (event.type == KeyPress && XLookupString(&event.xkey, text, 255, &key, nullptr) == 1)
	{
        printf("You pressed the %c key\n", text[0]);
        return text[0];
	}
}


EGLBoolean X11Window::InitEGL()
{
    const EGLint configAttribs[] =
    {
    EGL_RED_SIZE,
    8,
    EGL_GREEN_SIZE,
    8,
    EGL_BLUE_SIZE,
    8,
    EGL_ALPHA_SIZE,
    8,


    EGL_DEPTH_SIZE,8,
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    EGL_CONFORMANT,EGL_OPENGL_ES3_BIT,
    EGL_NONE
    };

    const EGLint contextAttribs[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 3,
        EGL_NONE
    };

    eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (eglDisplay == EGL_NO_DISPLAY)
    {
        return EGL_FALSE;
    }

    EGLint major, minor;
    if (!eglInitialize(eglDisplay, &major, &minor))
    {
        printf("Initialize: %c", eglGetError());
        return EGL_FALSE;
    }

    EGLConfig config;
    EGLint numConfigs;
    if (!eglChooseConfig(eglDisplay, configAttribs, &config, 1, &numConfigs))
    {
        printf("choose config: %c", eglGetError());
        return EGL_FALSE;
    }

    surface = eglCreateWindowSurface(eglDisplay, config, nativeWindow, nullptr);
    if (surface == EGL_NO_SURFACE)
    {
        switch (eglGetError())
        {
        case EGL_BAD_MATCH:
            printf("EGL_BAD_MATCH\n");           
            break;
        case EGL_BAD_CONFIG:
            printf("EGL_BAD_CONFIG\n");            
            break;
        case EGL_BAD_NATIVE_WINDOW:
            printf("EGL_BAD_NATIVE_WINDOW\n");            
            break;
        case EGL_BAD_ALLOC:
            printf("EGL_BAD_ALLOC\n");            
            break;
        }
    }
    if (surface == EGL_NO_SURFACE)
    {
        EGLint err = eglGetError();
    }    

    EGLContext context = eglCreateContext(eglDisplay, config, EGL_NO_CONTEXT, contextAttribs);
    if (context == EGL_NO_CONTEXT)
    {
        printf("context: %c", eglGetError());
        return EGL_FALSE;
    }

    if (!eglMakeCurrent(eglDisplay, surface, surface, context))
    {
        printf("Make current: %c", eglGetError());
        return EGL_FALSE;
    }

    eglSwapInterval(eglDisplay, 0);

    printf("EGL Creation Successful");

    return EGL_TRUE;
}