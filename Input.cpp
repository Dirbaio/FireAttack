#include "Input.h"
#include <cstdio>

Input::Input(int wiiMoteNum)
{
    mapSize = MAPPINGSIZE;
    valueMapSize = VALUESIZE;
    keys = vector<sf::Keyboard::Key> (mapSize);
    keysPressed = vector<bool> (mapSize);
    keysOldPressed = vector<bool> (mapSize);
    keyValues = vector<float> (valueMapSize, 128);
    wiiMote = true;
    this->wiiMoteNum = wiiMoteNum;
}

Input::Input(vector<Keyboard::Key> v)
{
    mapSize = MAPPINGSIZE;
    valueMapSize = VALUESIZE;
    keys = vector<sf::Keyboard::Key> (mapSize);
    keysPressed = vector<bool> (mapSize);
    keysOldPressed = vector<bool> (mapSize);
    keyValues = vector<float> (valueMapSize, 128);
    wiiMote = false;

    for (int i = 0; i < v.size(); i++)
        keys[i] = v[i];
}

bool Input::getKeyPressed(int n)
{
    return keysPressed[n];
}

bool Input::getKeyDown(int n)
{
    return keysPressed[n] && !keysOldPressed[n];
}

float Input::getValue(int n)
{
    return keyValues[n];
}


/*
void Input::setKey(int n, Keyboard::Key val)
{
    keys[n] = val;
}

Keyboard::Key Input::getKey(int n)
{
    return keys[n];
}*/

void Input::update()
{
    keysOldPressed  = keysPressed;

    if (!wiiMote)
    {
        for (int i = 0; i < mapSize; i++)
        {
            if(keys[i] == Keyboard::Tab)
                keysPressed[i] = Mouse::isButtonPressed(Mouse::Left);
            else
                keysPressed[i] = Keyboard::isKeyPressed(keys[i]);
        }

        Vector2i mpos = Mouse::getPosition(*app);

        vec2 pos ( float(mpos.x) - float(app->getSize().x) / 2,
                  float(app->getSize().y) / 2 -  float(mpos.y));
        pos /= float(app->getSize().y/2);

        keyValues[POINTERX] = pos.x;
        keyValues[POINTERY] = pos.y;

        keyValues[DASHX] = 128.0 + (keysPressed[DASH])*1560;

        keyValues[CONNECTED] = true;
    }
    else
    {
        int i = wiiMoteNum;
        keysPressed[JUMP] = wInput.wiiControl[i][W_A] || wInput.wiiControl[i][W_C];
        keysPressed[SHOOT] = wInput.wiiControl[i][W_B];
        keysPressed[SPAWN] = wInput.wiiControl[i][W_Z];
        keysPressed[MOVERIGHT] = wInput.wiiValues[i][WV_ANG] >= 44.0 && wInput.wiiValues[i][WV_ANG] <= 136.0 && wInput.wiiValues[i][WV_MAG] >= 0.5;
        keysPressed[MOVELEFT] = wInput.wiiValues[i][WV_ANG] >= 224.0 && wInput.wiiValues[i][WV_ANG] <= 316.0 && wInput.wiiValues[i][WV_MAG] >= 0.5;

        keyValues[DASHX] = wInput.wiiValues[i][NUN_ACC_X];
        keyValues[DASHY] = wInput.wiiValues[i][NUN_ACC_Y];
        keyValues[DASHZ] = wInput.wiiValues[i][NUN_ACC_Z];

        if(wInput.wiiControl[i][W_IR])
        {
            keyValues[POINTERX] = (wInput.wiiValues[i][IR_X]-512.0)/512.0  * float(app->getSize().x) / float(app->getSize().y);
            keyValues[POINTERY] = -(wInput.wiiValues[i][IR_Y]-512.0)/512.0;
        }

    }

}


PlayerConfig playerConfigs[4];

