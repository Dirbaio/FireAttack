#include "Input.h"

Input::Input()
{
    mapSize = MAPPINGSIZE;
    keys = vector<sf::Keyboard::Key> (mapSize);
    keysPressed = vector<bool> (mapSize);
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
    for (int i = 0; i < mapSize; i++)
        keysPressed[i] = Keyboard::isKeyPressed(keys[i]);

}
