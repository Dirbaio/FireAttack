#include "Pointer.h"

Pointer::Pointer(bool useWiimote, int numWiimote)
{
    this->useWiimote = useWiimote;
    this->numWiimote = numWiimote;
}

float Pointer::getX()
{
    return x;
}

float Pointer::getY()
{
    return y;
}

void Pointer::update()
{
    if (useWiimote)
    {
        /* HAY QUE CAMBIARLO POR IR DEL WIIMOTE CUANDO LO TENGAMOS*/

        Vector2i mpos = Mouse::getPosition(*theApp);
        x = mpos.x;
        y = mpos.y;
    }
    else
    {
        Vector2i mpos = Mouse::getPosition(*theApp);
        x = mpos.x;
        y = mpos.y;
    }
}
