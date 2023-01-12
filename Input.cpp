#include "Input.h"

#include <cstdio>


bool Input::keyIsPressed(KeySym ks)
{
	Display* display = XOpenDisplay(":0");
	char keysReturn[32];
	XQueryKeymap(display, keysReturn);
	KeyCode kc2 = XKeysymToKeycode(display, ks);

	bool isPressed = !!(keysReturn[kc2 >> 3] & (1 << (kc2 & 7)));
	
	XCloseDisplay(display);
	return isPressed;
}
