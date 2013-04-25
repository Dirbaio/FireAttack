#ifndef MENUSCENE_H
#define MENUSCENE_H

#include "Scene.h"

class MenuScene : public Scene
{
public:
    MenuScene();

    virtual void update();
    virtual void renderHud();
    bool hasKeyboard;
};

#endif // MENUSCENE_H
