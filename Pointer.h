#ifndef POINTER_H
#define POINTER_H

#include "util.h"

class Pointer
{
private:
    bool useWiimote;
    int numWiimote;
    float x, y;

public:
    Pointer(bool useWiimote, int numWiimote);

    float getX();
    float getY();

    void update();
};

#endif // POINTER_H
