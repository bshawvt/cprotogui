#include "ui.h"

void fnCallbackTest(UIH_STATE *state, UIH_CONTROL *control, void *data) {
    UIHDisplayError("Clickered the button!", __FUNCTION__, 1, "Success!");
}

void fnCallbackOpenWindow(UIH_STATE *state, UIH_CONTROL *control, void *data) {
    UIH_STATE *state2 = UIHMakeState();
    UIHInit(state2);
    UIHCreateWindow(state2, "Another window", 10, 10, 300, 150);

    UIH_CONTROL *label2 = UIHAddLabel(state2, "Hello world!", 0, 10, 10, 100, 50);
    UIH_CONTROL *button1 = UIHAddButton(state2, "Click Me", 0, 100, 50, 75, 50, &fnCallbackTest, NULL);
    UIHShowWindow(state2, 1);
}

int main(int argc, char* args[]) {

    UIH_STATE *mainState = UIHMakeState();
    if (mainState != NULL) {
        UIHInit(mainState);
        UIHCreateWindow(mainState, "heck世界你好こんにちは、世界feckC GUI Prototype Sample", 10, 10, 350, 150);

        UIH_CONTROL *label1 = UIHAddLabel(mainState, "heck世界你好こんにちは、世界feck", 0, 10, 10, 200, 50);
        UIH_CONTROL *button1 = UIHAddButton(mainState, "Open Window", 0, 100, 50, 100, 50, &fnCallbackOpenWindow, NULL);
        UIHShowWindow(mainState, 1);

        mainState->isRunning = 1;
        while(mainState->isRunning) {
            Sleep(1);
            UIHEventUpdate(mainState);
        }

    }
    else {
        UIHDisplayError("Feckin' memory didn't allocate yo", __FUNCTION__, 1, "Error");
        return -1;
    }
    return 0;
}
