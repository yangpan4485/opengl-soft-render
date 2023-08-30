#pragma once

#ifdef WIN32
#include <Windows.h>
#else
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#endif

#ifdef WIN32
#define WindowID HWND
#define DisplayID int
#else
#define WindowID Window
#define DisplayID Display*
#endif