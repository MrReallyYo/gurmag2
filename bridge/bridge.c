#include "bridge.h"

struct Screen GetScreen(char *name, char *clazz) {

  HWND hWnd = FindWindowExA(0, 0, clazz, name);

  struct Screen screen;
  if (0 != hWnd) {
    RECT rect;
    GetWindowRect(hWnd, &rect);
    screen.left = rect.left;
    screen.top = rect.top;
    screen.right = rect.right;
    screen.bottom = rect.bottom;
    screen.buffer = getRGBBuffer(hWnd, rect);
  } else {
    screen.left, screen.top, screen.right, screen.bottom = -1;
    screen.buffer.size, screen.buffer.ptr = 0;
  }
  return screen;
}

struct MousePos GetMousePos() {
  POINT mouse;
  boolean bla = GetCursorPos(&mouse);
  struct MousePos pos;
  pos.x = mouse.x;
  pos.y = mouse.y;
  return pos;
}

struct Buffer getRGBBuffer(HWND hWnd, RECT rect) {
  HDC hScreen = GetDC(HWND_DESKTOP);
  int w = rect.right - rect.left;
  int h = rect.bottom - rect.top;
  HDC hdcMem = CreateCompatibleDC(hScreen);
  HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, w, h);
  HGDIOBJ hOld = SelectObject(hdcMem, hBitmap);
  BitBlt(hdcMem, 0, 0, w, h, hScreen, rect.left, rect.top, SRCCOPY);
  SelectObject(hdcMem, hOld);

  BITMAPINFOHEADER bmi = {0};
  bmi.biSize = sizeof(BITMAPINFOHEADER);
  bmi.biPlanes = 1;
  bmi.biBitCount = 32;
  bmi.biWidth = w;
  bmi.biHeight = -h;
  bmi.biCompression = BI_RGB;
  bmi.biSizeImage = 0; // 3 * ScreenX * ScreenY;

  BYTE *ScreenData = (BYTE *)malloc(4 * w * h);

  GetDIBits(hdcMem, hBitmap, 0, h, ScreenData, (BITMAPINFO *)&bmi,
            DIB_RGB_COLORS);

  int x, y = 0;
  struct Buffer buffer;
  buffer.size = 3 * w * h;
  buffer.ptr = malloc(buffer.size);

  for (y = 0; y < h; y++) {
    for (x = 0; x < w; x++) {
      int pixel = (y * w) + x;
      buffer.ptr[3 * pixel] = (int)ScreenData[4 * pixel + 2];
      buffer.ptr[3 * pixel + 1] = (int)ScreenData[4 * pixel + 1];
      buffer.ptr[3 * pixel + 2] = (int)ScreenData[4 * pixel];
    }
  }

  OpenClipboard(hWnd);
  EmptyClipboard();

  SetClipboardData(CF_BITMAP, hBitmap);

  CloseClipboard();

  free(ScreenData);
  ReleaseDC(GetDesktopWindow(), hScreen);
  DeleteDC(hdcMem);
  DeleteObject(hBitmap);
  return buffer;
}
