#include "Input.h"
#include <cstdio>

Input* inpoint;

Input::Input(int wiiMoteNum)
{
    mapSize = MAPPINGSIZE;
    keys = vector<sf::Keyboard::Key> (mapSize);
    keysPressed = vector<bool> (mapSize);
    wiiMote = true;
    this->wiiMoteNum = wiiMoteNum;
    inpoint = this;
}

Input::Input(vector<Keyboard::Key> v)
{
    mapSize = MAPPINGSIZE;
    keys = vector<sf::Keyboard::Key> (mapSize);
    keysPressed = vector<bool> (mapSize);
    wiiMote = false;

    inpoint = this;

    for (int i = 0; i < v.size(); i++)
        keys[i] = v[i];
}

bool Input::getKeyPressed(int n)
{
    return keysPressed[n];
}

void Input::setKey(int n, Keyboard::Key val)
{
    keys[n] = val;
}

Keyboard::Key Input::getKey(int n)
{
    return keys[n];
}

void handle_event(struct wiimote_t* wm) {
    printf("\n\n--- EVENT [id %i] ---\n", wm->unid);

    /* if a button is pressed, report it */
    //inpoint->keysPressed[SHOOT] = false;
    //inpoint->keysPressed[JUMP] = false;
    inpoint->keysPressed[SHOOT] = IS_PRESSED(wm, WIIMOTE_BUTTON_B);
    inpoint->keysPressed[JUMP] = IS_PRESSED(wm, WIIMOTE_BUTTON_A);
    if (IS_PRESSED(wm, WIIMOTE_BUTTON_A))      printf("A pressed\n");
    if (IS_PRESSED(wm, WIIMOTE_BUTTON_B))      printf("B pressed\n");
    if (IS_PRESSED(wm, WIIMOTE_BUTTON_UP))		printf("UP pressed\n");
    if (IS_PRESSED(wm, WIIMOTE_BUTTON_DOWN))	printf("DOWN pressed\n");
    if (IS_PRESSED(wm, WIIMOTE_BUTTON_LEFT))	printf("LEFT pressed\n");
    if (IS_PRESSED(wm, WIIMOTE_BUTTON_RIGHT))	printf("RIGHT pressed\n");
    if (IS_PRESSED(wm, WIIMOTE_BUTTON_MINUS))	printf("MINUS pressed\n");
    if (IS_PRESSED(wm, WIIMOTE_BUTTON_PLUS))	printf("PLUS pressed\n");
    if (IS_PRESSED(wm, WIIMOTE_BUTTON_ONE))		printf("ONE pressed\n");
    if (IS_PRESSED(wm, WIIMOTE_BUTTON_TWO))		printf("TWO pressed\n");
    if (IS_PRESSED(wm, WIIMOTE_BUTTON_HOME))	printf("HOME pressed\n");

    /*
     *	Pressing minus will tell the wiimote we are no longer interested in movement.
     *	This is useful because it saves battery power.
     */
    /*
    if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_MINUS))
        wiiuse_motion_sensing(wm, 0);
*/
    /*
     *	Pressing plus will tell the wiimote we are interested in movement.
     */
    /*
    if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_PLUS))
        wiiuse_motion_sensing(wm, 1);
*/

 /*
    if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_UP))
        wiiuse_set_ir(wm, 1);
    if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_DOWN))
        wiiuse_set_ir(wm, 0);
*/
    /* if the accelerometer is turned on then print angles */
    if (WIIUSE_USING_ACC(wm)) {
        printf("wiimote roll  = %f [%f]\n", wm->orient.roll, wm->orient.a_roll);
        printf("wiimote pitch = %f [%f]\n", wm->orient.pitch, wm->orient.a_pitch);
        printf("wiimote yaw   = %f\n", wm->orient.yaw);
    }

    /*
     *	If IR tracking is enabled then print the coordinates
     *	on the virtual screen that the wiimote is pointing to.
     *
     *	Also make sure that we see at least 1 dot.
     */
    if (WIIUSE_USING_IR(wm)) {
        int i = 0;

        /* go through each of the 4 possible IR sources */
        for (; i < 4; ++i) {
            /* check if the source is visible */
            if (wm->ir.dot[i].visible)
                printf("IR source %i: (%u, %u)\n", i, wm->ir.dot[i].x, wm->ir.dot[i].y);
        }

        printf("IR cursor: (%u, %u)\n", wm->ir.x, wm->ir.y);
        printf("IR z distance: %f\n", wm->ir.z);
    }

    /* show events specific to supported expansions */
    if (wm->exp.type == EXP_NUNCHUK) {
        /* nunchuk */
        struct nunchuk_t* nc = (nunchuk_t*)&wm->exp.nunchuk;

        if (IS_PRESSED(nc, NUNCHUK_BUTTON_C))		printf("Nunchuk: C pressed\n");
        if (IS_PRESSED(nc, NUNCHUK_BUTTON_Z))		printf("Nunchuk: Z pressed\n");

        printf("nunchuk roll  = %f\n", nc->orient.roll);
        printf("nunchuk pitch = %f\n", nc->orient.pitch);
        printf("nunchuk yaw   = %f\n", nc->orient.yaw);

        printf("nunchuk joystick angle:     %f\n", nc->js.ang);
        printf("nunchuk joystick magnitude: %f\n", nc->js.mag);
    }
}

void handle_read(struct wiimote_t* wm, byte* data, unsigned short len) {
    int i = 0;

    printf("\n\n--- DATA READ [wiimote id %i] ---\n", wm->unid);
    printf("finished read of size %i\n", len);
    for (; i < len; ++i) {
        if (!(i%16))
            printf("\n");
        printf("%x ", data[i]);
    }
    printf("\n\n");
}

void handle_ctrl_status(struct wiimote_t* wm) {
    printf("\n\n--- CONTROLLER STATUS [wiimote id %i] ---\n", wm->unid);

    printf("attachment:      %i\n", wm->exp.type);
    printf("speaker:         %i\n", WIIUSE_USING_SPEAKER(wm));
    printf("ir:              %i\n", WIIUSE_USING_IR(wm));
    printf("leds:            %i %i %i %i\n", WIIUSE_IS_LED_SET(wm, 1), WIIUSE_IS_LED_SET(wm, 2), WIIUSE_IS_LED_SET(wm, 3), WIIUSE_IS_LED_SET(wm, 4));
    printf("battery:         %f %%\n", wm->battery_level);
}

void handle_disconnect(wiimote* wm) {
    printf("\n\n--- DISCONNECTED [wiimote id %i] ---\n", wm->unid);
}


void test(struct wiimote_t* wm, byte* data, unsigned short len) {
    printf("test: %i [%x %x %x %x]\n", len, data[0], data[1], data[2], data[3]);
}


void Input::update()
{
    if (!wiiMote)
    {
        for (int i = 0; i < mapSize; i++)
            keysPressed[i] = Keyboard::isKeyPressed(keys[i]);
    }
    else
    {
        if (wiiuse_poll(wiimotes, MAX_WIIMOTES)) {
            /*
             *	This happens if something happened on any wiimote.
             *	So go through each one and check if anything happened.
             */
            int i = wiiMoteNum;
            switch (wiimotes[i]->event) {
                case WIIUSE_EVENT:
                    /* a generic event occured */
                    handle_event(wiimotes[i]);
                    break;

                case WIIUSE_STATUS:
                    /* a status event occured */
                    handle_ctrl_status(wiimotes[i]);
                    break;

                case WIIUSE_DISCONNECT:
                case WIIUSE_UNEXPECTED_DISCONNECT:
                    /* the wiimote disconnected */
                    handle_disconnect(wiimotes[i]);
                    cerr << "disconnected WiiMote" << endl;
                    break;

                case WIIUSE_READ_DATA:
                    /*
                     *	Data we requested to read was returned.
                     *	Take a look at wiimotes[i]->read_req
                     *	for the data.
                     */
                    break;

                case WIIUSE_NUNCHUK_INSERTED:
                    /*
                     *	a nunchuk was inserted
                     *	This is a good place to set any nunchuk specific
                     *	threshold values.  By default they are the same
                     *	as the wiimote.
                     */
                     //wiiuse_set_nunchuk_orient_threshold((struct nunchuk_t*)&wiimotes[i]->exp.nunchuk, 90.0f);
                     //wiiuse_set_nunchuk_accel_threshold((struct nunchuk_t*)&wiimotes[i]->exp.nunchuk, 100);
                    printf("Nunchuk inserted.\n");
                    break;

                case WIIUSE_NUNCHUK_REMOVED:
                    /* some expansion was removed */
                    handle_ctrl_status(wiimotes[i]);
                    printf("An expansion was removed.\n");
                    break;

                default:
                    break;
            }
        }
    }

}
