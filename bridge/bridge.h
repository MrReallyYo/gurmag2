

#ifndef BRIDGE_H
#define BRIDGE_H

#include <Windows.h>
#include <winuser.h>

struct Buffer {
    char* ptr;
    int size;
};

struct Screen {
    long left, top, right, bottom;
    struct Buffer buffer;
};
struct Screen GetScreen(char* name, char* clazz);


struct MousePos {
    long x, y;
};

struct MousePos GetMousePos();


struct Buffer getRGBBuffer(HWND hWnd, RECT rect);


#endif