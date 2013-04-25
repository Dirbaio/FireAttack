#ifndef INPUT_H
#define INPUT_H

#include "util.h"
#include "Pointer.h"
#include <vector>
#include <SFML/System.hpp>

#define JUMP 0
#define MOVERIGHT 1
#define MOVELEFT 2
#define SHOOT 3
#define SPAWN 4
#define CONNECTED 5

#define DASHX 0
#define DASHY 1
#define DASHZ 2
#define POINTERX 3
#define POINTERY 4

#define MAPPINGSIZE 6
#define VALUESIZE 5

class Input
{
private:
    vector<sf::Keyboard::Key> keys;

    int mapSize;
    int valueMapSize;
    bool wiiMote;
    int wiiMoteNum;

public:

    vector<bool> keysPressed;
    vector<bool> keysOldPressed;
    vector<float> keyValues;
    Input(int wiiMoteNum);
    Input(vector<sf::Keyboard::Key> v);

    bool getKeyPressed(int n);
    bool getKeyDown(int n);
    float getValue(int n);
    float getPointerX();
    float getPointerY();

//    void setKey(int n, sf::Keyboard::Key val);

//    sf::Keyboard::Key getKey(int n);

    void update();

};

#endif // INPUT_H
