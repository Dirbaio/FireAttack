#ifndef MENUSCENE_H
#define MENUSCENE_H

#include "Scene.h"

class MenuScene : public Scene
{
public:
    MenuScene();

    virtual void renderHud();
};

#endif // MENUSCENE_H
