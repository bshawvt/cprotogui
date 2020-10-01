
#ifndef _UIH_GUARD
#define _UIH_GUARD

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


/// utf tests
#define UIH_HelloWorld_Hanzi "世界你好"
#define UIH_HelloWorld_Kanji "こんにちは、世界"
#define UIH_HelloWorld_English "Hello world"

#define UIH_NUM_FONTS               10
#define UIH_NUM_CONTROLS            10
#define UIH_NUM_STATE_DATUMS        10
#define UIH_NUM_CALLBACK_DATUMS     10
#define UIH_CONTROL_UUID_RANGE      1000
#define UIH_MENU_UUID_RANGE         10000
#define UIH_PROPNAME_STATE          L"uihprop1"
#define UIH_PROPNAME_CALLBACK       L"uihcallback1"
#define UIH_PROPNAME_MENUCALLBACK   L"uihcallback2"
#define UIH_MAX_FILENAME_SIZE       1024

typedef struct UIH_INPUT_STATE {
    int isActive;
    long time;
} UIH_INPUT_STATE;

typedef struct UIH_CONTROL_RECT {
    int x;
    int y;
    int width;
    int height;
} UIH_CONTROL_RECT;

typedef struct UIH_CONTROL {
    long uuid;
    HWND hwnd;
    wchar_t *text;
    UIH_CONTROL_RECT rect;
    void *fnControlCallback;
    void *fnResizeCallback;
} UIH_CONTROL;

typedef struct UIH_CALLBACK {
    void *datums[UIH_NUM_CALLBACK_DATUMS];
} UIH_CALLBACK;

/*typedef struct UIH_CONTROL_CALLBACK {
    void *datums[UIH_NUM_CALLBACK_DATUMS];
} UIH_CALLBACK;*/

typedef struct UIH_STATE {
    int isRunning; // if main state then 0 tells main loop to stop
    long nextUUID;
    int numberOfControls;
    int numberOfFonts;
    int numberOfMenuItems;
    wchar_t *hwndTitle;
    wchar_t *windowClassname;
    HACCEL accelTable;
    void *datums[UIH_NUM_STATE_DATUMS]; // holds misc data like current open filenames
    void *fnMenuCallback;
    void *fnWindowResizeCallback;
    HWND hwnd;
    HDC dc;
    HFONT fonts[UIH_NUM_FONTS];
    UIH_CONTROL controls[UIH_NUM_CONTROLS];
    UIH_INPUT_STATE keys[256];

} UIH_STATE;

typedef (*UIH_CALLBACK_CONTROLCALLBACK_FUNC) (UIH_STATE*, UIH_CONTROL*, void*);
typedef (*UIH_CALLBACK_CONTROLRESIZE_FUNC)(UIH_STATE*, UIH_CONTROL*, UIH_CONTROL_RECT*);
typedef (*UIH_CALLBACK_WINDOWRESIZE_FUNC) (UIH_STATE*, UIH_CONTROL_RECT*);

//int UIHErrorCallCount = 1;
//#define UIHErr() (printf("\n*****\n - GetLastError#%i = %i\n - in: %s\n - %s @ %i\n*****\n", UIHErrorCallCount++, (int) GetLastError(), __FUNCTION__, __FILE__, __LINE__));

LRESULT CALLBACK windowProcCallback(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
void UIHDisplayError(char *body, const char *gcc_function_macro, int showMessageBox, char* title);
void UIHClean(UIH_STATE *state);
UIH_STATE *UIHMakeState();
wchar_t *UIHCharToWide(char *text);
void UIHDoMenuCallbackFromId(UIH_STATE *state, int id);
void UIHDoControlCallbackFromId(UIH_STATE *state, int id);
void UIHDoControlResizeCallback(UIH_STATE *state);
int UIHGetButtonState(UIH_STATE *state, int virtualKey);
char *UIHWideToChar(wchar_t *text);
void UIHSetString(UIH_CONTROL *control, char *text);
wchar_t *UIHGetString(UIH_CONTROL *control);
void UIHCreateWindow(UIH_STATE *state, char *title, int x, int y, int width, int height);
HWND UIHCreateOwnDCWindow(UIH_STATE *state, char *title, int x, int y, int width, int height, HWND parent);
void UIHShowWindow(UIH_STATE *state, int hidden);
int UIHMakeControl(UIH_STATE *state, char *text, int x, int y, int width, int height);
void UIHRegisterMenuCallback(UIH_STATE *state, void *callback, void *data);
UIH_CONTROL *UIHAddLabel(UIH_STATE *state, char *text, int fontid, int x, int y, int w, int h);
UIH_CONTROL *UIHAddButton(UIH_STATE *state, char *text, int fontid, int x, int y, int w, int h, void* callback, void* data);
UIH_CONTROL *UIHAddEdit(UIH_STATE *state, char *text, int fontid, int x, int y, int w, int h);
int UIHLoadFont(UIH_STATE *state, char *fontName, int size);
void UIHInit(UIH_STATE *state);
void UIHClean(UIH_STATE *state);
void UIHEventUpdate(UIH_STATE *state);


#endif // _GUARD
