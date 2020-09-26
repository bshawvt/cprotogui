
#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#undef __STRICT_ANSI__


#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "wchar.h"

#include "windows.h"
#include "mbstring.h"
#include "Commctrl.h"
#include "locale.h"
#include "richedit.h"
#include "commdlg.h"

#include "ui.h"


UIH_STATE *UIHMakeState() {
    UIH_STATE *state = (UIH_STATE*) malloc(sizeof(UIH_STATE));
    if (state == NULL) {
        UIHDisplayError("Bad things have happened and couldn't allocate memory for state", __FUNCTION__, 1, "Error");
        return NULL;
    }
    *state = (UIH_STATE) {0};
    state->accelTable = NULL;
    return state;
}

wchar_t *UIHCharToWide(char *text) {
    int textSize = MultiByteToWideChar(CP_UTF8, 0, text, -1, NULL, 0);

    wchar_t *wText = (wchar_t*) malloc(sizeof(wchar_t) * textSize);
    //swprintf(wText, textSize-1, "%s", text);
    //swprintf(wText, "%s", text);

    MultiByteToWideChar(CP_UTF8, 0, text, -1, wText, textSize);
    return wText;
}
void UIHDisplayError(char *body, const char *gcc_function_macro, int showMessageBox, char* title) {
    printf("Error in %s():\n %s\n", gcc_function_macro, body);
    if (showMessageBox) {
        int bodySize = MultiByteToWideChar(CP_UTF8, 0, body, -1, NULL, 0);
        int titleSize = MultiByteToWideChar(CP_UTF8, 0, title, -1, NULL, 0);

        wchar_t tmpBodyText[bodySize];
        wchar_t tmpTitleText[titleSize];

        //sprintf(tmpBodyText, "%s", body);
        //sprintf(tmpTitleText, "%s", title);

        MultiByteToWideChar(CP_UTF8, 0, body, -1, tmpBodyText, bodySize);
        MultiByteToWideChar(CP_UTF8, 0, title, -1, tmpTitleText, titleSize);

        MessageBoxW(NULL, tmpBodyText, tmpTitleText, 0);
    }
};
void UIHDoMenuCallbackFromId(UIH_STATE *state, int id) {
    printf("id = %i\n", id);
    if (state->fnMenuCallback != NULL) {
        HANDLE fHandle = GetPropW(state->hwnd, UIH_PROPNAME_MENUCALLBACK);
        void (*fnMenuCallback)(UIH_STATE *, int, void *) = (void(*)(UIH_STATE*, int, void*))state->fnMenuCallback;
        fnMenuCallback(state, id, fHandle);
    }
}
void UIHDoControlCallbackFromId(UIH_STATE *state, int id) {
    printf("id = %i\n", id);
    for(int i = 0; i < state->numberOfControls; i++) {
        UIH_CONTROL *control = &state->controls[i];
        if (id == control->uuid) {
            if (control->fnControlCallback != NULL) {
                HANDLE tHandle = GetPropW(control->hwnd, UIH_PROPNAME_CALLBACK);
                void (*fnControlCallback)(UIH_STATE*, UIH_CONTROL*, void*) = (void(*)(UIH_STATE*,UIH_CONTROL*, void*))control->fnControlCallback;
                fnControlCallback(state, control, tHandle);
            }
            break;
        }
    }
}
void UIHDoControlResizeCallback(UIH_STATE *state) {
    UIH_CONTROL_RECT rect = {0};
    RECT *tmpRect = (RECT*)malloc(sizeof(RECT));
    GetWindowRect(state->hwnd, tmpRect);
    rect.width = tmpRect->right - tmpRect->left;
    rect.height = tmpRect->bottom - tmpRect->top;
    for(int i = 0; i < UIH_NUM_CONTROLS; i++) {
        UIH_CONTROL *control = &state->controls[i];
        if (control->fnResizeCallback != NULL) {
            printf("UIHDoControlResizeCallback control addr = %p\n", control);
            void (*fnResizeCallback)(UIH_STATE*, UIH_CONTROL*, UIH_CONTROL_RECT*) = (void(*)(UIH_STATE*, UIH_CONTROL*, UIH_CONTROL_RECT*))control->fnResizeCallback;
            fnResizeCallback(state, control, &rect);
        }
    }
    free(tmpRect);
}
LRESULT CALLBACK windowProcCallback(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
    int id = (unsigned short) wparam;
    //int event = ((unsigned short)(((unsigned long) wparam >> 16) & 0xFFFF));
    HANDLE handle = GetPropW(hwnd, UIH_PROPNAME_STATE);
    UIH_STATE *state = NULL;
    if (handle != NULL) {
        state = (UIH_STATE *) handle;
        switch (umsg) {
            case WM_COMMAND: {
                if (id < UIH_CONTROL_UUID_RANGE + UIH_NUM_CONTROLS) {
                    UIHDoControlCallbackFromId(state, id);
                }
                else {
                    UIHDoMenuCallbackFromId(state, id);
                }
                break;
            }
            case WM_SIZE: {
                printf("size\n");
                UIHDoControlResizeCallback(state);
                break;
            }
            case WM_SIZING: {
                printf("size2\n");
                break;
            }
            case WM_CLOSE: {
                printf("closeasdasdasd");
                UIHClean(state);
                break;
            }

            case WM_RBUTTONDOWN: {
                if (state->keys[VK_RBUTTON].isActive == 0) {
                    state->keys[VK_RBUTTON].isActive = 1;
                    //state->keys[wparam].time =
                }
                break;
            }
            case WM_LBUTTONDOWN: {
                if (state->keys[VK_LBUTTON].isActive == 0) {
                    state->keys[VK_LBUTTON].isActive = 1;
                    //state->keys[wparam].time =
                }
                break;
            }
            case WM_MBUTTONDOWN: {
                if (state->keys[VK_MBUTTON].isActive == 0) {
                    state->keys[VK_MBUTTON].isActive = 1;
                    //state->keys[wparam].time =
                }
                break;
            }
            case WM_RBUTTONUP: {
                if (state->keys[VK_RBUTTON].isActive == 1) {
                    state->keys[VK_RBUTTON].isActive = 0;
                    //state->keys[wparam].time =
                }
                break;
            }
            case WM_LBUTTONUP: {
                if (state->keys[VK_LBUTTON].isActive == 1) {
                    state->keys[VK_LBUTTON].isActive = 0;
                    //state->keys[wparam].time =
                }
                break;
            }
            case WM_MBUTTONUP: {
                if (state->keys[VK_MBUTTON].isActive == 1) {
                    state->keys[VK_MBUTTON].isActive = 0;
                    //state->keys[wparam].time =
                }
                break;
            }
            case WM_KEYDOWN: {
                if (wparam < 256 && wparam > 0) {
                    if (state->keys[wparam].isActive == 0) {
                        state->keys[wparam].isActive = 1;
                        //state->keys[wparam].time =
                    }
                }
                break;
            }
            case WM_KEYUP: {
                if (wparam < 256 && wparam > 0) {
                    if (state->keys[wparam].isActive == 1) {
                        state->keys[wparam].isActive = 0;
                        //state->keys[wparam].time =
                    }
                }
                break;
            }


            default: {
                //printf("???\n");
                //printf("\n** window proc callback:\n id=%i event=%i umsg=%x", id, event, umsg);
                //return DefWindowProcW(hwnd, umsg, wparam, lparam);
                break;
            }
        }
    }
    else {
        UIHDisplayError("handle is null!", __FUNCTION__, 0, "");

    }
    return DefWindowProcW(hwnd, umsg, wparam, lparam);
}
int UIHGetButtonState(UIH_STATE *state, int virtualKey) {
    return state->keys[virtualKey].isActive;
}
char *UIHWideToChar(wchar_t *text) {
    int textSize = WideCharToMultiByte(CP_UTF8, 0, text, -1, NULL, 0, NULL, NULL);
    char *buffer = malloc(sizeof(char) * (textSize + 1));
    WideCharToMultiByte(CP_UTF8, 0, text, -1, buffer, textSize, NULL, NULL);
    return buffer;
}
void UIHSetString(UIH_CONTROL *control, char *text) {
    /*int textSize = MultiByteToWideChar(CP_UTF8, 0, text, -1, NULL, 0);
    wchar_t *tmpBuffer = malloc(textSize * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, text, -1, tmpBuffer, textSize);
    printf("hwnd= %i\ncontrol addr = %p\n", control->hwnd, control);*/
    wchar_t* tmpBuffer = UIHCharToWide(text);
    SendMessageW(control->hwnd, WM_SETTEXT, 0, (LPARAM) tmpBuffer);
    free(tmpBuffer);
}
wchar_t *UIHGetString(UIH_CONTROL *control) {
    int editSize = SendMessageW(control->hwnd, WM_GETTEXTLENGTH, 0, 0);
    wchar_t *buffer = malloc(sizeof(wchar_t) * (editSize + sizeof(wchar_t)));
    SendMessageW(control->hwnd, WM_GETTEXT, editSize + sizeof(wchar_t), (LPARAM)buffer);
    return buffer;
}
void UIHCreateWindow(UIH_STATE *state, char *title, int x, int y, int width, int height) {
    int classnameSize = MultiByteToWideChar(CP_UTF8, 0, title, -1, NULL, 0);
    int titleSize = MultiByteToWideChar(CP_UTF8, 0, title, -1, NULL, 0);

    state->windowClassname = malloc(sizeof(wchar_t) * (classnameSize + sizeof(wchar_t)));
    state->hwndTitle = malloc(sizeof(wchar_t) + (titleSize * sizeof(wchar_t)));

    char tmpClassname[classnameSize];
    sprintf(tmpClassname, "%p%s", state, title);
    MultiByteToWideChar(CP_UTF8, 0, tmpClassname, -1, state->windowClassname, classnameSize);
    MultiByteToWideChar(CP_UTF8, 0, title, -1, state->hwndTitle, titleSize);
    //UIHErr();
    HINSTANCE hInstance = GetModuleHandleW(NULL);

    WNDCLASSEXW window;
    window.cbSize = sizeof(WNDCLASSEX);
    window.style = CS_HREDRAW | CS_VREDRAW;
    window.lpfnWndProc = windowProcCallback;
    window.cbClsExtra = 0;
    window.cbWndExtra = 0;
    window.hInstance = hInstance;
    window.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    window.hCursor = LoadCursor(NULL, IDC_ARROW);
    window.hbrBackground = (HBRUSH) COLOR_WINDOW;
    window.lpszMenuName = NULL;
    window.lpszClassName = state->windowClassname;
    window.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassExW(&window);
    //UIHErr();

    state->hwnd = CreateWindowExW(0, state->windowClassname, state->hwndTitle, WS_TILEDWINDOW, x, y, width, height, NULL, NULL, hInstance, NULL);
    SetPropW(state->hwnd, UIH_PROPNAME_STATE, state);

    //UIHErr();
}
void UIHCreateOwnDCWindow(UIH_STATE *state, char *title, int x, int y, int width, int height) {//, HINSTANCE hInstance) {
    int classnameSize = MultiByteToWideChar(CP_UTF8, 0, title, -1, NULL, 0);
    int titleSize = MultiByteToWideChar(CP_UTF8, 0, title, -1, NULL, 0);

    state->windowClassname = malloc(sizeof(wchar_t) * (classnameSize + sizeof(wchar_t)));
    state->hwndTitle = malloc(sizeof(wchar_t) + (titleSize * sizeof(wchar_t)));

    char tmpClassname[classnameSize];
    sprintf(tmpClassname, "%p%s", state, title);
    MultiByteToWideChar(CP_UTF8, 0, tmpClassname, -1, state->windowClassname, classnameSize);
    MultiByteToWideChar(CP_UTF8, 0, title, -1, state->hwndTitle, titleSize);

    HINSTANCE hInstance = GetModuleHandleW(NULL);

    WNDCLASSEXW window;
    window.cbSize = sizeof(WNDCLASSEXW);
    window.style = CS_OWNDC;
    window.lpfnWndProc = windowProcCallback;
    window.cbClsExtra = 0;
    window.cbWndExtra = 0;
    window.hInstance = hInstance;//hInstance;
    window.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    window.hCursor = LoadCursor(NULL, IDC_ARROW);
    window.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    window.lpszMenuName = NULL;
    window.lpszClassName = state->windowClassname; //L"uihowndc";
    window.hIconSm = NULL;//LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassExW(&window);

    state->hwnd = CreateWindowExW(0, state->windowClassname, state->hwndTitle, WS_OVERLAPPEDWINDOW, x, y, width, height, NULL, NULL, hInstance, NULL);
    state->dc = GetDC(state->hwnd);

    SetPropW(state->hwnd, UIH_PROPNAME_STATE, state);

    //UIHErr();
}
void UIHShowWindow(UIH_STATE *state, int hidden) {
    if (hidden) {
        ShowWindow(state->hwnd, SW_SHOWDEFAULT);
    } else {
        ShowWindow(state->hwnd, SW_HIDE);
    }
    UpdateWindow(state->hwnd);
};
int UIHMakeControl(UIH_STATE *state, char *text, int x, int y, int width, int height) {
    if (state->numberOfControls < UIH_NUM_CONTROLS) {
        int index = state->numberOfControls++;
        state->controls[index].uuid = ++state->nextUUID;
        int textSize = MultiByteToWideChar(CP_UTF8, 0, text, -1, NULL, 0);
        state->controls[index].text = malloc(sizeof(wchar_t) * (textSize + sizeof(wchar_t)));
        MultiByteToWideChar(CP_UTF8, 0, text, -1, state->controls[index].text, textSize);

        state->controls[index].rect.x = x;
        state->controls[index].rect.y = y;
        state->controls[index].rect.width = width;
        state->controls[index].rect.height = height;

        return index;
    }
    else {
        UIHDisplayError("exceeded UIH_NUM_CONTROLS", __FUNCTION__, 0, "");
        return -1;
    }
}
void UIHRegisterMenuCallback(UIH_STATE *state, void *callback, void *data) {
    state->fnMenuCallback = callback;
    SetPropW(state->hwnd, UIH_PROPNAME_MENUCALLBACK, data);
}
UIH_CONTROL *UIHAddLabel(UIH_STATE *state, char *text, int fontid, int x, int y, int w, int h) {
    int index  = UIHMakeControl(state, text, x, y, w, h);

    printf("%s: uuid = %li\n", __FUNCTION__, state->controls[index].uuid);
    state->controls[index].hwnd = CreateWindowExW(0, L"STATIC", state->controls[index].text, WS_VISIBLE | WS_CHILD, x, y, w, h, state->hwnd, (HMENU) state->controls[index].uuid, NULL, NULL);
    SendMessageW(state->controls[index].hwnd,
                 WM_SETFONT,
                 MAKEWPARAM(state->fonts[fontid], 0),
                 MAKELPARAM(1, 0));
    return &state->controls[index];
}
UIH_CONTROL *UIHAddButton(UIH_STATE *state, char *text, int fontid, int x, int y, int w, int h, void* callback, void* data) {
    int index  = UIHMakeControl(state, text, x, y, w, h);
    printf("%s: uuid = %li\n", __FUNCTION__, state->controls[index].uuid);
    state->controls[index].hwnd = CreateWindowExW(0, L"BUTTON", state->controls[index].text, WS_VISIBLE | WS_CHILD, x, y, w, h, state->hwnd, (HMENU) state->controls[index].uuid, NULL, NULL);

    if (callback != NULL) {
        state->controls[index].fnControlCallback = callback;
        if (data != NULL) {
            SetPropW(state->controls[index].hwnd, UIH_PROPNAME_CALLBACK, data);
        }
    }

    SendMessageW(state->controls[index].hwnd,
                 WM_SETFONT,
                 MAKEWPARAM(state->fonts[fontid], 0),
                 MAKELPARAM(1, 0));
    return &state->controls[index];
}
UIH_CONTROL *UIHAddEdit(UIH_STATE *state, char *text, int fontid, int x, int y, int w, int h) {
    int index  = UIHMakeControl(state, text, x, y, w, h);

    printf("%s: uuid = %li\n", __FUNCTION__, state->controls[index].uuid);
    state->controls[index].hwnd = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", state->controls[index].text, ES_MULTILINE | ES_AUTOVSCROLL | WS_VISIBLE | WS_CHILD, x, y, w, h, state->hwnd, (HMENU) state->controls[index].uuid, NULL, NULL);
    SendMessageW(state->controls[index].hwnd,
                 WM_SETFONT,
                 MAKEWPARAM(state->fonts[fontid], 0),
                 MAKELPARAM(1, 0));
    return &state->controls[index];
}
int UIHLoadFont(UIH_STATE *state, char *fontName) {
    if (state->numberOfFonts < UIH_NUM_FONTS) {
        int index = state->numberOfFonts++;
        int fontNameSize = MultiByteToWideChar(CP_UTF8, 0, fontName, -1, NULL, 0);
        wchar_t tmpFontName[fontNameSize];
        MultiByteToWideChar(CP_UTF8, 0, fontName, -1,tmpFontName, fontNameSize);
        state->fonts[index] = CreateFontW(14, 0, 0, 0, FW_REGULAR, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, tmpFontName);//TEXT("Microsoft Sans Serif"));
        return index;
    }
    else {
        UIHDisplayError("exceeded UIH_NUM_FONTS", __FUNCTION__, 0, "");
        return -1;
    }
}
void UIHInit(UIH_STATE *state) {
    setlocale(LC_ALL, "");

    UIHLoadFont(state, "Microsoft Sans Serif"); // make state->fonts[0] the default font
    state->nextUUID = UIH_CONTROL_UUID_RANGE;

    INITCOMMONCONTROLSEX c;
    c.dwICC = ICC_TAB_CLASSES;
    c.dwSize = sizeof(INITCOMMONCONTROLSEX);
    InitCommonControlsEx(&c); // force load comctl32 even though it's linked because sometimes it just dies to death without this
}
void UIHClean(UIH_STATE *state) {
    printf("numberOfFonts = %i\n", state->numberOfFonts);
    int i = 0;
    while(i < state->numberOfFonts) {
        DeleteObject(state->fonts[i++]);
    }
    i = 0;
    while (i < state->numberOfControls) {
        RemovePropW(state->controls[i++].hwnd, UIH_PROPNAME_CALLBACK);
        free(state->controls[i++].text);
    }
    i = 0;
    while (i < UIH_NUM_STATE_DATUMS) {
        free(state->datums[i++]);
    }
    RemovePropW(state->hwnd, UIH_PROPNAME_STATE);
    RemovePropW(state->hwnd, UIH_PROPNAME_MENUCALLBACK);

    free(state->hwndTitle);
    free(state->windowClassname);
    DestroyAcceleratorTable(state->accelTable);
    state->isRunning = 0;

}
void UIHEventUpdate(UIH_STATE *state) {
    MSG msg;
    if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (state->accelTable != NULL)
            TranslateAcceleratorW(state->hwnd, state->accelTable, &msg);
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}
