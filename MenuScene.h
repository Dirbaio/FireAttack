#ifndef MENUSCENE_H
#define MENUSCENE_H

#include "Scene.h"

class MenuScene : public Scene
{
private:
    bool menu, credits;

public:
    MenuScene();

    virtual void update();
    virtual void renderHud();
    static void writeText(const string text, const int offset);
    bool hasKeyboard;
};

#endif // MENUSCENE_H
