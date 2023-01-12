#pragma once

#include <linux/input.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

class Input
{
public:
	bool keyIsPressed(KeySym ks);
};
