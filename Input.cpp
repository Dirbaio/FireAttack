#include "Input.h"
#include <cstdio>

Input::Input(int wiiMoteNum)
{
    mapSize = MAPPINGSIZE;
    valueMapSize = VALUESIZE;
    keys = vector<sf::Keyboard::Key> (mapSize);
    keysPressed = vector<bool> (mapSize);
    keyValues = vector<float> (valueMapSize, 128);
    wiiMote = true;
    this->wiiMoteNum = wiiMoteNum;
    pt = new Pointer(true, wiiMoteNum);
}

Input::Input(vector<Keyboard::Key> v)
{
    mapSize = MAPPINGSIZE;
    valueMapSize = VALUESIZE;
    keys = vector<sf::Keyboard::Key> (mapSize);
    keysPressed = vector<bool> (mapSize);
    keyValues = vector<float> (valueMapSize, 128);
    wiiMote = false;

    pt = new Pointer(false, 0);

    for (int i = 0; i < v.size(); i++)
        keys[i] = v[i];
}

bool Input::getKeyPressed(int n)
{
    return keysPressed[n];
}

float Input::getValue(int n)
{
    return keyValues[n];
}

float Input::getPointerX()
{
    return pt->getX();
}

float Input::getPointerY()
{
    return pt->getY();
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
    pt->update();

    if (!wiiMote)
    {
        for (int i = 0; i < mapSize; i++)
            keysPressed[i] = Keyboard::isKeyPressed(keys[i]);
    }
    else
    {
        int i = wiiMoteNum;
        keysPressed[JUMP] = wInput.wiiControl[i][W_A];
        keysPressed[SHOOT] = wInput.wiiControl[i][W_B];
        keysPressed[SPAWN] = wInput.wiiControl[i][W_Z];
        keysPressed[MOVERIGHT] = wInput.wiiValues[i][WV_ANG] >= 44.0 && wInput.wiiValues[i][WV_ANG] <= 136.0 && wInput.wiiValues[i][WV_MAG] >= 0.5;
        keysPressed[MOVELEFT] = wInput.wiiValues[i][WV_ANG] >= 224.0 && wInput.wiiValues[i][WV_ANG] <= 316.0 && wInput.wiiValues[i][WV_MAG] >= 0.5;

        keyValues[DASHX] = wInput.wiiValues[i][NUN_ACC_X];
        keyValues[DASHY] = wInput.wiiValues[i][NUN_ACC_Y];
        keyValues[DASHZ] = wInput.wiiValues[i][NUN_ACC_Z];
    }

}
