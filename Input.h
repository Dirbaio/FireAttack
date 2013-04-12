#ifndef INPUT_H
#define INPUT_H

#include "util.h"
#include <vector>
#include <SFML/System.hpp>

#define JUMP 0
#define MOVERIGHT 1
#define MOVELEFT 2
#define SHOOT 3
#define SPAWN 4

#define MAPPINGSIZE 5

class Input
{
private:
    vector<sf::Keyboard::Key> keys;
    vector<bool> keysPressed;

    int mapSize;
    bool wiiMote;
    int wiiMoteNum;

    wiimote** wiimotes;

public:
    Input(int wiiMoteNum);
    Input(vector<sf::Keyboard::Key> v);

    bool getKeyPressed(int n);

    void setKey(int n, sf::Keyboard::Key val);

    sf::Keyboard::Key getKey(int n);

    void update();
};

#endif // INPUT_H
