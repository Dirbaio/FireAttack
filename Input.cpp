#include "Input.h"
#include <cstdio>

Input::Input(int wiiMoteNum)
{
    mapSize = MAPPINGSIZE;
    keys = vector<sf::Keyboard::Key> (mapSize);
    keysPressed = vector<bool> (mapSize);
    wiiMote = true;
    this->wiiMoteNum = wiiMoteNum;
}

Input::Input(vector<Keyboard::Key> v)
{
    mapSize = MAPPINGSIZE;
    keys = vector<sf::Keyboard::Key> (mapSize);
    keysPressed = vector<bool> (mapSize);
    wiiMote = false;

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
        int i = wiiMoteNum;
        keysPressed[JUMP] = wInput.wiiControl[i][W_A];
        keysPressed[SHOOT] = wInput.wiiControl[i][W_B];
        keysPressed[MOVERIGHT] = wInput.wiiValues[i][WV_ANG] >= 44.0 && wInput.wiiValues[i][WV_ANG] <= 136.0 && wInput.wiiValues[i][WV_MAG] >= 0.5;
        keysPressed[MOVELEFT] = wInput.wiiValues[i][WV_ANG] >= 224.0 && wInput.wiiValues[i][WV_ANG] <= 316.0 && wInput.wiiValues[i][WV_MAG] >= 0.5;
    }

}
