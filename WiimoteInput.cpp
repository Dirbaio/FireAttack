#include "WiimoteInput.h"
#include <unistd.h>
WiimoteInput wInput;

bool wiimoteInputRunning;

void wiimoteSearch()
{
    while(wInput.searchMode && wiimoteInputRunning)
        wInput.search();
}

void wiimoteUpdate()
{

    while (wiimoteInputRunning)
    {
        bool finish = wInput.update();
        if(finish) break;
    }
    wInput.disconnect();
}

sf::Thread wiimoteSearchThread(&wiimoteSearch);
sf::Thread wiimoteUpdateThread(&wiimoteUpdate);

void startWiimoteInput()
{
    wInput.init();

    wiimoteInputRunning = true;
    wiimoteSearchThread.launch();
}

void stopWiimoteInput()
{
    wiimoteInputRunning = false;
    wiimoteSearchThread.wait();
    wiimoteUpdateThread.wait();
}

void WiimoteInput::stopSearch()
{
    if(!searchMode) return;
    searchMode = false;
}

void WiimoteInput::init()
{
    wiimotes =  wiiuse_init(MAX_WIIMOTES);

    wiiuse_set_leds(wiimotes[0], WIIMOTE_LED_1);
    wiiuse_set_leds(wiimotes[1], WIIMOTE_LED_2);
    wiiuse_set_leds(wiimotes[2], WIIMOTE_LED_3);
    wiiuse_set_leds(wiimotes[3], WIIMOTE_LED_4);

    for(int i = 0; i < MAX_WIIMOTES; i++)
    {
        wiiuse_set_ir(wiimotes[i], 1);
        wiiuse_set_ir_position(wiimotes[i], WIIUSE_IR_ABOVE);
        wiiuse_set_ir_sensitivity(wiimotes[i], 5); //1..5
        wiiuse_set_aspect_ratio(wiimotes[i], WIIUSE_ASPECT_16_9);
        wiiuse_set_ir_vres(wiimotes[i], 1024, 1024);
    }

    searchMode = true;
    connectedCount = 0;
}


void WiimoteInput::disconnect()
{
    for (int i = 0; i < MAX_WIIMOTES; i++)
        wiiuse_disconnect(wiimotes[i]);
}

void WiimoteInput::search()
{
    int found = wiiuse_find(wiimotes+connectedCount, MAX_WIIMOTES-connectedCount, 1);
    if (found)
        cout << found << " wiimotes found."<<endl;

    int connected = wiiuse_connect(wiimotes+connectedCount, MAX_WIIMOTES-connectedCount);
    if (connected)
    {
        wiiuse_set_leds(wiimotes[0], WIIMOTE_LED_1);
        wiiuse_set_leds(wiimotes[1], WIIMOTE_LED_2);
        wiiuse_set_leds(wiimotes[2], WIIMOTE_LED_3);
        wiiuse_set_leds(wiimotes[3], WIIMOTE_LED_4);

        for(int i = 0; i < MAX_WIIMOTES; i++)
        {
            wiiuse_set_ir(wiimotes[i], 1);
            wiiuse_set_ir_position(wiimotes[i], WIIUSE_IR_ABOVE);
            wiiuse_set_ir_sensitivity(wiimotes[i], 5); //1..5
            wiiuse_set_aspect_ratio(wiimotes[i], WIIUSE_ASPECT_16_9);
            wiiuse_set_ir_vres(wiimotes[i], 1024, 1024);
        }

        cerr << "Connected to " << connected << " wiimotes (of " << found << " found).\n";
    }

    connectedCount += connected;

    if(connectedCount == connected && connectedCount != 0)
    {
        usleep(100000);
        wiiuse_set_leds(wiimotes[0], WIIMOTE_LED_1);
        wiiuse_set_leds(wiimotes[1], WIIMOTE_LED_2);
        wiiuse_set_leds(wiimotes[2], WIIMOTE_LED_3);
        wiiuse_set_leds(wiimotes[3], WIIMOTE_LED_4);

        for(int i = 0; i < MAX_WIIMOTES; i++)
        {
            wiiuse_set_ir(wiimotes[i], 1);
            wiiuse_set_ir_position(wiimotes[i], WIIUSE_IR_ABOVE);
            wiiuse_set_ir_sensitivity(wiimotes[i], 5); //1..5
            wiiuse_set_aspect_ratio(wiimotes[i], WIIUSE_ASPECT_16_9);
            wiiuse_set_ir_vres(wiimotes[i], 1024, 1024);
        }

        wiimoteUpdateThread.launch();
    }

//    if(connected)
//        stopSearch();
}

bool WiimoteInput::update()
{

    if (wiiuse_poll(wiimotes, MAX_WIIMOTES)) {
        /*
          *	This happens if something happened on any wiimote.
          *	So go through each one and check if anything happened.
          */
        for (int i = 0; i < MAX_WIIMOTES; i++)
        {
            switch (wiimotes[i]->event) {
            case WIIUSE_EVENT:
                /* a generic event occured */
                handle_event(wiimotes[i], i);
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
                return true;
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
    return false;
}

void WiimoteInput::handle_event(struct wiimote_t* wm, int i) {
    //printf("\n\n--- EVENT [id %i] ---\n", wm->unid);
    /* if a button is pressed, report it */
    wiiControl[i][W_B] = IS_PRESSED(wm, WIIMOTE_BUTTON_B);
    wiiControl[i][W_A] = IS_PRESSED(wm, WIIMOTE_BUTTON_A);
    wiiControl[i][W_UP] = IS_PRESSED(wm, WIIMOTE_BUTTON_UP);
    wiiControl[i][W_DOWN] = IS_PRESSED(wm, WIIMOTE_BUTTON_DOWN);
    wiiControl[i][W_RIGHT] = IS_PRESSED(wm, WIIMOTE_BUTTON_RIGHT);
    wiiControl[i][W_LEFT] = IS_PRESSED(wm, WIIMOTE_BUTTON_LEFT);
    wiiControl[i][W_PLUS] = IS_PRESSED(wm, WIIMOTE_BUTTON_PLUS);
    wiiControl[i][W_MINUS] = IS_PRESSED(wm, WIIMOTE_BUTTON_MINUS);
    wiiControl[i][W_ONE] = IS_PRESSED(wm, WIIMOTE_BUTTON_ONE);
    wiiControl[i][W_TWO] = IS_PRESSED(wm, WIIMOTE_BUTTON_TWO);
    wiiControl[i][W_HOME] = IS_PRESSED(wm, WIIMOTE_BUTTON_HOME);


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
    /*    if (WIIUSE_USING_ACC(wm)) {
        printf("wiimote roll  = %f [%f]\n", wm->orient.roll, wm->orient.a_roll);
        printf("wiimote pitch = %f [%f]\n", wm->orient.pitch, wm->orient.a_pitch);
        printf("wiimote yaw   = %f\n", wm->orient.yaw);
    }*/

    if (WIIUSE_USING_IR(wm))
    {
        wiiControl[i][W_IR] = wm->ir.num_dots != 0;
        wiiValues[i][IR_X] = wm->ir.x;
        wiiValues[i][IR_Y] = wm->ir.y;
    }

    /* show events specific to supported expansions */
    if (wm->exp.type == EXP_NUNCHUK) {
        /* nunchuk */
        struct nunchuk_t* nc = (nunchuk_t*)&wm->exp.nunchuk;

        wiiControl[i][W_C] = IS_PRESSED(nc, NUNCHUK_BUTTON_C);
        wiiControl[i][W_Z] = IS_PRESSED(nc, NUNCHUK_BUTTON_Z);
        /*
        printf("nunchuk roll  = %f\n", nc->orient.roll);
        printf("nunchuk pitch = %f\n", nc->orient.pitch);
        printf("nunchuk yaw   = %f\n", nc->orient.yaw);
*/

        wiiValues[i][WV_ANG] = nc->js.ang;
        wiiValues[i][WV_MAG] = nc->js.mag;

        wiiValues[i][NUN_ACC_X] = nc->accel.x;
        wiiValues[i][NUN_ACC_Y] = nc->accel.y;
        wiiValues[i][NUN_ACC_Z] = nc->accel.z;
    }
}

void WiimoteInput::handle_read(struct wiimote_t* wm, byte* data, unsigned short len) {
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

void WiimoteInput::handle_ctrl_status(struct wiimote_t* wm) {
    printf("\n\n--- CONTROLLER STATUS [wiimote id %i] ---\n", wm->unid);

    printf("attachment:      %i\n", wm->exp.type);
    printf("speaker:         %i\n", WIIUSE_USING_SPEAKER(wm));
    printf("ir:              %i\n", WIIUSE_USING_IR(wm));
    printf("leds:            %i %i %i %i\n", WIIUSE_IS_LED_SET(wm, 1), WIIUSE_IS_LED_SET(wm, 2), WIIUSE_IS_LED_SET(wm, 3), WIIUSE_IS_LED_SET(wm, 4));
    printf("battery:         %f %%\n", wm->battery_level);
}

void WiimoteInput::handle_disconnect(wiimote* wm) {
    printf("\n\n--- DISCONNECTED [wiimote id %i] ---\n", wm->unid);
}


void WiimoteInput::test(struct wiimote_t* wm, byte* data, unsigned short len) {
    printf("test: %i [%x %x %x %x]\n", len, data[0], data[1], data[2], data[3]);
}
