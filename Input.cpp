#include "Input.h"

Input::Input(int wiiMoteNum)
{
    mapSize = MAPPINGSIZE;
    wiiMote = true;
    this->wiiMoteNum = wiiMoteNum;
    keys = vector<sf::Keyboard::Key> (mapSize);
    keysPressed = vector<bool> (mapSize);
    if (wiiMote)
    {
        wiimote** wiimotes;
        int found, connected;

        wiimotes =  wiiuse_init(4);

        found = wiiuse_find(wiimotes, 4, 5);
        if (!found) {
            cerr << "No wiimotes found." << endl;
        }

        connected = wiiuse_connect(wiimotes, 4);
        if (connected)
            printf("Connected to %i wiimotes (of %i found).\n", connected, found);
        else {
            printf("Failed to connect to any wiimote.\n");
            return 0;
        }

        /*
         *	Now set the LEDs and rumble for a second so it's easy
         *	to tell which wiimotes are connected (just like the wii does).
         */
        wiiuse_set_leds(wiimotes[0], WIIMOTE_LED_1);
        wiiuse_set_leds(wiimotes[1], WIIMOTE_LED_2);
        wiiuse_set_leds(wiimotes[2], WIIMOTE_LED_3);
        wiiuse_set_leds(wiimotes[3], WIIMOTE_LED_4);
    }
}

Input::Input(vector<Keyboard::Key> v)
{
    mapSize = MAPPINGSIZE;
    keys = vector<sf::Keyboard::Key> (mapSize);
    keysPressed = vector<bool> (mapSize);

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
            switch (wiimotes[wiiMoteNum]->event) {
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
