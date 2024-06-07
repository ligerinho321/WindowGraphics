#include <WG/WG_Window.h>

struct _WG_Window{
    HWND hwnd;
    
    WG_Window *parent;

    WINDOWPLACEMENT wpPrev;
    uint32_t flags;
    uint32_t ex_flags;
    bool fullscreen;
    
    struct{
        bool left;
        bool middle;
        bool right;
    }mouse;

    int32_t min_width;
    int32_t min_height;
    int32_t max_width;
    int32_t max_height;

    uint8_t keyboardstate[WG_NUM_SCANCODES];
    
    WG_Event *event;
    bool processed_events;

    WG_EventCallback event_callback;
    void *event_userdata;
};


LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam){

    LRESULT result = -1;

    WG_Window *window = (WG_Window*)GetWindowLongPtr(hwnd,GWLP_USERDATA);

    if(window != NULL){

        WG_Event *event = window->event;
        
        if(window->processed_events){
            memset(window->event,0,sizeof(WG_Event));
            window->processed_events = false;
        }

        switch (uMsg){

            case WM_DESTROY:
                event->type = WG_QUIT;
                PostQuitMessage(0);
                result = 0;
                break;

            case WM_ACTIVATE:

                if(LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE){     

                    window->event->type = WG_WINDOW_FOCUS_GAINED;
                    
                }else if(LOWORD(wParam) == WA_INACTIVE){

                    window->event->type = WG_WINDOW_FOCUS_LOST;
                }

                result = 0;
                break;

            case WM_GETMINMAXINFO:
                MINMAXINFO *min_max_info = (MINMAXINFO*)lParam;
                
                min_max_info->ptMaxTrackSize.x = window->max_width;
                min_max_info->ptMaxTrackSize.y = window->max_height;

                min_max_info->ptMinTrackSize.x = window->min_width;
                min_max_info->ptMinTrackSize.y = window->min_height;

                result = 0;
                break;

            case WM_SETCURSOR:
                if(LOWORD(lParam) == HTLEFT || LOWORD(lParam) == HTRIGHT){
                    SetCursor(LoadCursor(NULL,IDC_SIZEWE));
                }else if(LOWORD(lParam) == HTTOP || LOWORD(lParam) == HTBOTTOM){
                    SetCursor(LoadCursor(NULL,IDC_SIZENS));
                }else if(LOWORD(lParam) == HTTOPLEFT || LOWORD(lParam) == HTBOTTOMRIGHT){
                    SetCursor(LoadCursor(NULL,IDC_SIZENWSE));
                }else if(LOWORD(lParam) == HTTOPRIGHT || LOWORD(lParam) == HTBOTTOMLEFT){
                    SetCursor(LoadCursor(NULL,IDC_SIZENESW));
                }else{
                    SetCursor(LoadCursor(NULL,IDC_ARROW));
                }
                result = 1;
                break;

            case WM_MOUSEWHEEL:
                event->mouse.wheel = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
                event->type = WG_MOUSEWHEEL;
                result = 0;
                break;  
 
            case WM_RBUTTONDOWN:
                if(window->mouse.right == false){
                    SetCapture(window->hwnd);
                    event->mouse.button = WG_MOUSEBUTTON_RIGHT;
                    event->type = WG_MOUSEBUTTONDOWN;
                    window->mouse.right = true;
                }
                result = 0;
                break;
            case WM_RBUTTONUP:
                if(window->mouse.right == true){
                    ReleaseCapture();
                    event->mouse.button = WG_MOUSEBUTTON_RIGHT;
                    event->type = WG_MOUSEBUTTONUP;
                    window->mouse.right = false;
                }
                result = 0;
                break;

            case WM_MBUTTONDOWN:
                if(window->mouse.middle == false){
                    SetCapture(window->hwnd);
                    event->mouse.button = WG_MOUSEBUTTON_MIDDLE;
                    event->type = WG_MOUSEBUTTONDOWN;
                    window->mouse.middle = true;
                }
                result = 0;
                break;
            case WM_MBUTTONUP:
                if(window->mouse.middle == true){
                    ReleaseCapture();
                    event->mouse.button = WG_MOUSEBUTTON_MIDDLE;
                    event->type = WG_MOUSEBUTTONUP;
                    window->mouse.middle = false;
                }
                result = 0;
                break;

            case WM_LBUTTONDOWN:
                if(window->mouse.left == false){
                    SetCapture(window->hwnd);
                    event->mouse.button = WG_MOUSEBUTTON_LEFT;
                    event->type = WG_MOUSEBUTTONDOWN;
                    window->mouse.left = true;
                }
                result = 0;
                break;
            case WM_LBUTTONUP:
                if(window->mouse.left == true){
                    ReleaseCapture();
                    event->mouse.button = WG_MOUSEBUTTON_LEFT;
                    event->type = WG_MOUSEBUTTONUP;
                    window->mouse.left = false;
                }
                result = 0;
                break;

            case WM_SIZE:
                event->type = WG_WINDOWRESIZE;
                result = 0;
                break;
            
            case WM_SYSCHAR: case WM_CHAR:

                if((uint16_t)wParam >= 0x20){

                    event->type = WG_TEXTINPUT;

                    event->text_input = (uint16_t)wParam;
                }

                result = 0;

                break;

            case WM_KEYDOWN: case WM_SYSKEYDOWN: case WM_KEYUP: case WM_SYSKEYUP:

                WORD vkCode = LOWORD(wParam);

                WORD keyFlags = HIWORD(lParam);

                WORD scanCode = LOBYTE(keyFlags);

                BOOL isExtendedKey = (keyFlags & KF_EXTENDED) == KF_EXTENDED;

                if(isExtendedKey) scanCode = MAKEWORD(scanCode,0xE0);
                
                BOOL wasKeyDown = (keyFlags & KF_REPEAT) == KF_REPEAT;

                WORD repeatCount = LOWORD(lParam);

                BOOL isKeyReleased = (keyFlags & KF_UP) == KF_UP;

                
                event->key.scancode = window_scancode_table[LOBYTE(scanCode) | (HIBYTE(scanCode) ? 0x80 : 0x00)];

                event->key.repeat = repeatCount;


                if(isKeyReleased == FALSE){

                    event->type = WG_KEYDOWN;

                    event->key.state = WG_PRESSED;

                    window->keyboardstate[event->key.scancode] = 1;

                }
                else if(wasKeyDown == TRUE && isKeyReleased == TRUE){
                    
                    event->type = WG_KEYUP;

                    event->key.state = WG_RELEASED;

                    window->keyboardstate[event->key.scancode] = 0;
                }

                result = 0;

                break;
        }

        if(window->event_callback && result > -1){
            window->event_callback(window->event_userdata,window->event);
        }
    }

    if(result > -1){
        return result;
    }
    else{
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}


static uint32_t WG_GetWindowStyle(uint32_t flags){
    
    uint32_t window_flags = 0x00;

    if(flags & (WG_WINDOW_FULLSCREEN | WG_WINDOW_POPUP)){
        window_flags |= WS_POPUP;
    }
    else if(flags & WG_WINDOW_CHILD){
        window_flags |= WS_CHILD;
    }
    else{
        if(flags & WG_WINDOW_CAPTION){
            window_flags |= WS_CAPTION | WS_SYSMENU;
        }
        if(flags & WG_WINDOW_RESIZABLE){
            window_flags |= WS_THICKFRAME;
        }
        if(flags & WG_WINDOW_MINIMIZE){
            window_flags |= WS_MINIMIZEBOX;
        }
        if(flags & WG_WINDOW_MAXIMIZE){
            window_flags |= WS_MAXIMIZEBOX;
        }

        if(flags & WG_WINDOW_MAXIMIZED){
            window_flags |= WS_MAXIMIZE;
        }
        else if(flags & WG_WINDOW_MINIMIZED){
            window_flags |= WS_MINIMIZE;
        }
    }

    return window_flags;
}

static void WG_PrintWindowError(void){

    DWORD error_code = GetLastError();
    
    if(error_code == ERROR_SUCCESS) return;

    LPSTR error_message = NULL;

    DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM;

    DWORD size_message_error = FormatMessageA(flags,NULL,error_code,LOCALE_SYSTEM_DEFAULT,(LPSTR)&error_message,0,NULL);

    if(size_message_error > 0){

        fprintf(stderr,"Error: %s",error_message);

        LocalFree(error_message);
    }
    else{
        WG_PrintWindowError();
    }
}


WG_Window* WG_CreateWindow(WNDCLASSW *window_class,WG_Window *parent,const uint16_t* title,int x,int y,int width,int height,uint32_t flags,uint32_t ex_flags){
    
    int monitor_width = GetSystemMetrics(SM_CXSCREEN);
    int monitor_height = GetSystemMetrics(SM_CYSCREEN);
    uint32_t style = WG_GetWindowStyle(flags);

    if(flags & WG_WINDOW_FULLSCREEN){
        x = 0;
        y = 0;
        width = monitor_width;
        height = monitor_height;
    }
    else{
        RECT rect = {0,0,width,height};

        AdjustWindowRect(&rect,style,false);

        width = rect.right - rect.left;
        height = rect.bottom - rect.top;

        if(x == WG_WINDOWPOS_CENTERED){
            x =  monitor_width * 0.5f - width * 0.5f;
        }
        if(y == WG_WINDOWPOS_CENTERED){
            y = monitor_height * 0.5f - height * 0.5f;
        }
    }

    HWND hwnd = CreateWindowEx(
        ex_flags,
        window_class->lpszClassName,
        title,
        style,
        x,
        y,
        width,
        height,
        (parent) ? parent->hwnd : NULL,
        NULL,
        window_class->hInstance,
        NULL
    );

    if(hwnd == NULL){
        WG_PrintWindowError();
        return NULL;
    }

    WG_Window *window = calloc(1,sizeof(WG_Window));

    window->event = malloc(sizeof(WG_Event));

    window->hwnd = hwnd;

    window->parent = parent;

    window->flags = flags;
    window->ex_flags = ex_flags;
    
    window->fullscreen = (window->flags & WG_WINDOW_FULLSCREEN) ? true : false;

    window->wpPrev.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(window->hwnd,&window->wpPrev);

    window->max_width = monitor_width;
    window->max_height = monitor_height;

    memset(window->keyboardstate,0,WG_NUM_SCANCODES);

    SetWindowLongPtr(hwnd,GWLP_USERDATA,(LONG_PTR)window);

    if(flags & WG_WINDOW_SHOWN){
        ShowWindow(hwnd,SW_SHOW);
    }
    else{
        ShowWindow(hwnd,SW_HIDE);
    }
    
    return window;
}

void WG_DestroyWindow(WG_Window *window){
    if(!window) return;
    DestroyWindow(window->hwnd);
    free(window->event);
    free(window); 
}


void WG_SetWindowTitle(WG_Window *window,const uint16_t *title){
    bool result = SetWindowTextW(window->hwnd,title);
    if(result == false){
        WG_PrintWindowError();
    }
}

HWND WG_GetWindowHandle(WG_Window *window){
    return window->hwnd;
}

bool WG_WindowIsVisible(WG_Window *window){
    return IsWindowVisible(window->hwnd);
}

bool WG_PointOverWindow(WG_Window *window,int x,int y){
    POINT point;
    point.x = x;
    point.y = y;
    return WindowFromPoint(point) == window->hwnd;
}


bool WG_WindowHasFocus(WG_Window *window){
    if(!window) return false;
    return (GetFocus() == window->hwnd) ? true : false;
}

void WG_WindowSetFocus(WG_Window *window){
    if(window){
        SetFocus(window->hwnd);
    }
    else{
        SetFocus(NULL);
    }
}


bool WG_WindowIsEnabled(WG_Window *window){
    if(!window) return false;
    return (IsWindowEnabled(window->hwnd)) ? true : false;
}

void WG_EnableWindow(WG_Window *window,bool enable){
    if(!window) return;
    EnableWindow(window->hwnd,enable);
}


void WG_SetWindowPos(WG_Window *window,HWND hWndInsertAfter,int x,int y,int w,int h,uint32_t flags){

    bool result = SetWindowPos(window->hwnd,hWndInsertAfter,x,y,w,h,flags);

    if(result == false){
        WG_PrintWindowError();
    }
}


void WG_SetWindowClientSize(WG_Window *window,int width,int height){
    RECT rect = {0,0,width,height};
    bool result;
    
    result = AdjustWindowRect(&rect,WG_GetWindowStyle(window->flags),false);

    if(result == false){
        WG_PrintWindowError();
        return;
    }

    result = SetWindowPos(
        window->hwnd,
        NULL,
        0,
        0,
        rect.right - rect.left,
        rect.bottom - rect.top,
        SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER
    );

    if(result == false){
        WG_PrintWindowError();
        return;
    }
}

void WG_GetWindowClientSize(WG_Window *window,int *width,int *height){
    RECT rect;
    bool result = GetClientRect(window->hwnd,&rect);
    if(result == true){
        *width = rect.right - rect.left;
        *height = rect.bottom - rect.top;
    }
    else{
        WG_PrintWindowError();
    }
}


void WG_SetWindowSize(WG_Window *window,int width,int height){

    bool result = SetWindowPos(
        window->hwnd,
        NULL,
        0,
        0,
        width,
        height,
        SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER
    );

    if(result == false){
        WG_PrintWindowError();
        return;
    }
}

void WG_GetWindowSize(WG_Window *window,int *width,int *height){
    RECT rect;
    bool result = GetWindowRect(window->hwnd,&rect);
    if(result == true){
        *width = rect.right - rect.left;
        *height = rect.bottom - rect.top;
    }
    else{
        WG_PrintWindowError();
    }
}


void WG_SetWindowCoordinates(WG_Window *window,int x,int y){
    
    bool result = SetWindowPos(
        window->hwnd,
        NULL,
        x,
        y,
        0,
        0,
        SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER
    );

    if(result == false){
        WG_PrintWindowError();
    }
}

void WG_GetWindowCoordinates(WG_Window *window,int *x,int *y){
    RECT rect;
    bool result = GetWindowRect(window->hwnd,&rect);
    if(result == true){
        *x = rect.left;
        *y = rect.top;
    }
    else{
        WG_PrintWindowError();
    }
}


void WG_SetWindowMinimumSize(WG_Window *window,int min_width,int min_height){
    RECT rect = {0,0,min_width,min_height};
    uint32_t style = WG_GetWindowStyle(window->flags);
    AdjustWindowRect(&rect,style,false);
    window->min_width = rect.right - rect.left;
    window->min_height = rect.bottom - rect.top;
}

void WG_SetWindowMaxSize(WG_Window *window,int max_width,int max_height){
    RECT rect = {0,0,max_width,max_height};
    uint32_t style = WG_GetWindowStyle(window->flags);
    AdjustWindowRect(&rect,style,false);
    window->max_width = rect.right - rect.left;
    window->max_height = rect.bottom - rect.top;
}


void WG_SetWindowFullScreen(WG_Window *window,bool fullscreen){
    
    if(window->flags & WG_WINDOW_POPUP) return;

    if(fullscreen == true && window->fullscreen == false){

        MONITORINFO monitorInfo = {sizeof(MONITORINFO)};
        HMONITOR hMonitor = MonitorFromWindow(window->hwnd,MONITOR_DEFAULTTOPRIMARY);

        if(GetWindowPlacement(window->hwnd,&window->wpPrev) && GetMonitorInfo(hMonitor,&monitorInfo)){
            
            SetWindowLongPtr(window->hwnd,GWL_STYLE,WS_POPUP);
            
            SetWindowPos(
                window->hwnd,
                HWND_TOP,
                monitorInfo.rcMonitor.left,
                monitorInfo.rcMonitor.top,
                monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
                monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
                SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW
            );

            window->fullscreen = true;
        }
    }
    else if(fullscreen == false && window->fullscreen == true){

        if(window->flags & WG_WINDOW_FULLSCREEN){

            window->flags = WG_WINDOW_DEFAULT;

            SetWindowLongPtr(window->hwnd,GWL_STYLE,WG_GetWindowStyle(window->flags));

            RECT rect;
            SystemParametersInfo(SPI_GETWORKAREA,0,&rect,0);

            uint32_t uFlags = SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW;

            SetWindowPos(window->hwnd,NULL,rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,uFlags);
        }
        else{

            SetWindowLongPtr(window->hwnd,GWL_STYLE,WG_GetWindowStyle(window->flags));

            SetWindowPlacement(window->hwnd,&window->wpPrev);

            uint32_t uFlags = SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED;

            SetWindowPos(window->hwnd,NULL,0,0,0,0,uFlags);
        }

        window->fullscreen = false;
    }
}


void WG_ShowWindow(WG_Window *window){
    if(window->flags & WG_WINDOW_SHOWN) return;
    ShowWindow(window->hwnd,SW_SHOW);
    window->flags &= ~WG_WINDOW_HIDDEN;
    window->flags |= WG_WINDOW_SHOWN;
}

void WG_HideWindow(WG_Window *window){
    if(window->flags & WG_WINDOW_HIDDEN) return;
    ShowWindow(window->hwnd,SW_HIDE);
    window->flags &= ~WG_WINDOW_SHOWN;
    window->flags |= WG_WINDOW_HIDDEN;
}


void WG_PollEvent(WG_Window *window,WG_Event *event){
    
    MSG msg = {0};

    if(PeekMessage(&msg,window->hwnd,0,0,PM_REMOVE) != 0){
        
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        window->processed_events = true;
    }
    else if(window->processed_events){

        memset(window->event,0,sizeof(WG_Event));
        window->processed_events = false;

    }

    memcpy(event,window->event,sizeof(WG_Event));
}

void WG_SetEventCallback(WG_Window *window,WG_EventCallback callback,void *userdata){
    window->event_callback = callback;
    window->event_userdata = userdata;
}


uint32_t WG_GetMouseState(void){
    uint32_t state = 0;
    if(GetKeyState(VK_LBUTTON) & 0x8000){
        state |= WG_MOUSEBUTTON_LEFT;
    }
    if(GetKeyState(VK_MBUTTON) & 0x8000){
        state |= WG_MOUSEBUTTON_MIDDLE;
    }
    if(GetKeyState(VK_RBUTTON) & 0x8000){
        state |= WG_MOUSEBUTTON_RIGHT;
    }
    return state;
}

void WG_GetMousePos(int *x,int *y){
    POINT point;
    GetCursorPos(&point);
    if(x) *x = point.x;
    if(y) *y = point.y;
}

void WG_GetMousePosInWindow(WG_Window *window,int *x,int *y){
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(window->hwnd,&point);
    if(x) *x = point.x;
    if(y) *y = point.y;
}

void WG_ClientToScreen(WG_Window *window,int client_x,int client_y,int *screen_x,int *screen_y){
    POINT point = {
        .x = client_x,
        .y = client_y
    };
    ClientToScreen(window->hwnd,&point);
    if(screen_x) *screen_x = point.x;
    if(screen_y) *screen_y = point.y;
}


const uint8_t* WG_GetKeyBoardState(WG_Window *window,uint32_t *numKeys){
    if(numKeys != NULL){
        *numKeys = WG_NUM_SCANCODES;
    }
    return window->keyboardstate;
}

WG_KeyMod WG_GetKeyModState(void){
    WG_KeyMod keyMod = WG_KEYMOD_NONE;

    if(GetKeyState(VK_LCONTROL) & 0x8000){
        keyMod |= WG_KEYMOD_LCTRL;
    }
    if(GetKeyState(VK_RCONTROL) & 0x8000){
        keyMod |= WG_KEYMOD_RCTRL;
    }

    if(GetKeyState(VK_LSHIFT) & 0x8000){
        keyMod |= WG_KEYMOD_LSHIFT;
    }
    if(GetKeyState(VK_RSHIFT) & 0x8000){
        keyMod |= WG_KEYMOD_RSHIFT;
    }

    if(GetKeyState(VK_LMENU) & 0x8000){
        keyMod |= WG_KEYMOD_LALT;
    }
    if(GetKeyState(VK_RMENU) & 0x8000){
        keyMod |= WG_KEYMOD_RALT;
    }

    if(GetKeyState(VK_LWIN) & 0x8000){
        keyMod |= WG_KEYMOD_LGUI;
    }
    if(GetKeyState(VK_RWIN) & 0x8000){
        keyMod |= WG_KEYMOD_RGUI;
    }

    if(GetKeyState(VK_NUMLOCK) & 0x01){
        keyMod |= WG_KEYMOD_NUM;
    }
    if(GetKeyState(VK_CAPITAL) & 0x01){
        keyMod |= WG_KEYMOD_CAPS;
    }
    if(GetKeyState(VK_SCROLL) & 0x01){
        keyMod |= WG_KEYMOD_SCROLL;
    }

    return keyMod;
}


uint64_t WG_GetPerformanceFrequency(void){
    LARGE_INTEGER frequency;
    bool result = QueryPerformanceFrequency(&frequency);
    if(result == false){
        WG_PrintWindowError();
        return 0;
    }
    return frequency.QuadPart;
}

uint64_t WG_GetPerformanceCounter(void){
    LARGE_INTEGER ticks;
    bool result = QueryPerformanceCounter(&ticks);
    if(result == false){
        WG_PrintWindowError();
        return 0;
    }
    return ticks.QuadPart;
}


bool WG_SetClipboardText(uint16_t *text){
    if(!OpenClipboard(NULL) || !text) return false;
    EmptyClipboard();
    HANDLE *handle_memory = GlobalAlloc(GHND,sizeof(uint16_t) * (wcslen(text) + 1));
    uint16_t *data = GlobalLock(handle_memory);
    if(!data) return false;
    wcscpy(data,text);
    GlobalUnlock(handle_memory);
    SetClipboardData(CF_UNICODETEXT,handle_memory);
    CloseClipboard();
    return true;
}

uint16_t* WG_GetClipboardText(void){
    if(!OpenClipboard(NULL)) return NULL;
    HANDLE *handle_clipboard = GetClipboardData(CF_UNICODETEXT);
    uint16_t *data = GlobalLock(handle_clipboard);
    if(!data) return NULL;
    uint16_t *text = wcsdup(data);
    GlobalUnlock(handle_clipboard);
    CloseClipboard();
    return text;
}