#ifndef WG_WINDOW_H_
#define WG_WINDOW_H_

#include <windows.h>
#include <WG/WG_ScanCodes.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define WG_MOUSEBUTTON_LEFT   0x01
#define WG_MOUSEBUTTON_MIDDLE 0x02
#define WG_MOUSEBUTTON_RIGHT  0x04

#define WG_WINDOWPOS_CENTERED -1

#define WG_PRESSED  0x01
#define WG_RELEASED 0x02

#define WG_WINDOW_SHOWN      0x001
#define WG_WINDOW_HIDDEN     0x002
#define WG_WINDOW_CAPTION    0x004
#define WG_WINDOW_RESIZABLE  0x008
#define WG_WINDOW_MINIMIZE   0x010
#define WG_WINDOW_MAXIMIZE   0x020
#define WG_WINDOW_MINIMIZED  0x040
#define WG_WINDOW_MAXIMIZED  0x080
#define WG_WINDOW_FULLSCREEN 0x100
#define WG_WINDOW_POPUP      0x200
#define WG_WINDOW_CHILD      0x400
#define WG_WINDOW_DEFAULT    (WG_WINDOW_SHOWN | WG_WINDOW_CAPTION | WG_WINDOW_RESIZABLE | WG_WINDOW_MINIMIZE | WG_WINDOW_MAXIMIZE)

typedef struct _WG_Window WG_Window;

typedef enum _WG_EventType{
    WG_FIRSTEVENT,

    WG_QUIT,

    WG_MOUSEWHEEL,
    WG_MOUSEBUTTONDOWN,
    WG_MOUSEBUTTONUP,

    WG_TEXTINPUT,

    WG_WINDOWRESIZE,

    WG_WINDOW_FOCUS_GAINED,
    WG_WINDOW_FOCUS_LOST,
    
    WG_KEYDOWN,
    WG_KEYUP,
}WG_EventType;

typedef enum _WG_KeyMod{
    WG_KEYMOD_NONE = 0x000,

    WG_KEYMOD_LCTRL = 0x001,
    WG_KEYMOD_RCTRL = 0x002,

    WG_KEYMOD_LSHIFT = 0x004,
    WG_KEYMOD_RSHIFT = 0x008,

    WG_KEYMOD_LALT = 0x010,
    WG_KEYMOD_RALT = 0x020,

    WG_KEYMOD_LGUI = 0x040,
    WG_KEYMOD_RGUI = 0x080,

    WG_KEYMOD_NUM = 0x100,
    WG_KEYMOD_CAPS = 0x200,
    WG_KEYMOD_SCROLL = 0x800,

    WG_KEYMOD_CTRL = WG_KEYMOD_LCTRL | WG_KEYMOD_RCTRL,
    WG_KEYMOD_SHIFT = WG_KEYMOD_LSHIFT | WG_KEYMOD_RSHIFT,
    WG_KEYMOD_ALT = WG_KEYMOD_LALT | WG_KEYMOD_RALT,
    WG_KEYMOD_GUI = WG_KEYMOD_LGUI | WG_KEYMOD_RGUI
}WG_KeyMod;

typedef struct _WG_Event{
    uint32_t type;

    struct{
        uint32_t button;
        float wheel;
    }mouse;

    struct{
        uint32_t scancode;
        uint8_t state;
        uint8_t repeat;
    }key;

    uint16_t text_input;
}WG_Event;

typedef void (*WG_EventCallback)(void *userdata,WG_Event *event);


LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

WG_Window* WG_CreateWindow(WNDCLASSW *window_class,WG_Window *parent,const uint16_t* title,int x,int y,int width,int height,uint32_t flags,uint32_t ex_flags);

void WG_DestroyWindow(WG_Window *window);

void WG_SetWindowTitle(WG_Window *window,const uint16_t *title);

HWND WG_GetWindowHandle(WG_Window *window);

bool WG_WindowIsVisible(WG_Window *window);

bool WG_PointOverWindow(WG_Window *window,int x,int y);


bool WG_WindowHasFocus(WG_Window *window);

void WG_WindowSetFocus(WG_Window *window);


bool WG_WindowIsEnabled(WG_Window *window);

void WG_EnableWindow(WG_Window *window,bool enable);


void WG_SetWindowPos(WG_Window *window,HWND hWndInsertAfter,int x,int y,int w,int h,uint32_t flags);

void WG_SetWindowClientSize(WG_Window *window,int width,int height);

void WG_GetWindowClientSize(WG_Window *window,int *width,int *height);

void WG_SetWindowSize(WG_Window *window,int width,int height);

void WG_GetWindowSize(WG_Window *window,int *width,int *height);

void WG_SetWindowCoordinates(WG_Window *window,int x,int y);

void WG_GetWindowCoordinates(WG_Window *window,int *x,int *y);

void WG_SetWindowMinimumSize(WG_Window *window,int min_width,int min_height);

void WG_SetWindowMaxSize(WG_Window *window,int max_width,int max_height);

void WG_SetWindowFullScreen(WG_Window *window,bool fullscreen);

void WG_ShowWindow(WG_Window *window);

void WG_HideWindow(WG_Window *window);

void WG_PollEvent(WG_Window *window,WG_Event *event);

void WG_SetEventCallback(WG_Window *window,WG_EventCallback callback,void *userdata);


uint32_t WG_GetMouseState(void);

void WG_GetMousePos(int *x,int *y);

void WG_GetMousePosInWindow(WG_Window *window,int *x,int *y);

void WG_ClientToScreen(WG_Window *window,int client_x,int client_y,int *screen_x,int *screen_y);


const uint8_t* WG_GetKeyBoardState(WG_Window *window,uint32_t *numKeys);

WG_KeyMod WG_GetKeyModState(void);


uint64_t WG_GetPerformanceFrequency(void);

uint64_t WG_GetPerformanceCounter(void);


bool WG_SetClipboardText(uint16_t *text);

uint16_t* WG_GetClipboardText(void);

#endif