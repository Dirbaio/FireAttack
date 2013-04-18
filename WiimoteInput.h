#ifndef WIIMOTEINPUT_H
#define WIIMOTEINPUT_H

#include "wiiuse.h"
#include "util.h"

#define NUM_WII_BUTTONS 20
#define MAX_WIIMOTES 4

enum WiiButtons
{
    W_A,
    W_B,
    W_UP,
    W_DOWN,
    W_RIGHT,
    W_LEFT,
    W_PLUS,
    W_MINUS,
    W_ONE,
    W_TWO,
    W_HOME,
    W_C,
    W_Z
};

enum WiiValues
{
    WV_ANG,
    WV_MAG,
    WV_ACC_X,
    WV_ACC_Y,
    WV_ACC_Z,
    NUN_ACC_X,
    NUN_ACC_Y,
    NUN_ACC_Z,
    WV_OR_R,
    WV_OR_P,
    WV_OR_Y,
    NUN_OR_R,
    NUN_OR_P,
    NUN_OR_Y
};

class WiimoteInput
{
public:
    wiimote** wiimotes;
    bool wiiControl[MAX_WIIMOTES][NUM_WII_BUTTONS];
    float wiiValues[MAX_WIIMOTES][NUM_WII_BUTTONS];

    void init();
    void handle_event(struct wiimote_t* wm, int i);
    void handle_read(struct wiimote_t* wm, byte* data, unsigned short len);
    void handle_ctrl_status(struct wiimote_t* wm);
    void handle_disconnect(wiimote* wm);
    void test(struct wiimote_t* wm, byte* data, unsigned short len);
    void updateWiimotes();
};

extern WiimoteInput wInput;

#endif // WIIMOTEINPUT_H
