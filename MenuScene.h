#ifndef MENUSCENE_H
#define MENUSCENE_H

#include "Scene.h"

class MenuScene : public Scene
{
private:
    bool menu, credits;
    int objScore;
    bool plusClicked, minusClicked;
    float changeScoreTime, changeScoreTimeWindow, closeTime;

public:
    MenuScene(int objecScore);

    virtual void update();
    virtual void renderHud();
    static void writeText(const string text, const int offset, const int fontSize);
    bool hasKeyboard;
};

#endif // MENUSCENE_H
