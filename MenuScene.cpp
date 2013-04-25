#include "MenuScene.h"
#include<sstream>
#include "GameScene.h"

MenuScene::MenuScene()
{
}

string toString(int n)
{
    stringstream ss;
    ss<<n;
    return ss.str();
}

void MenuScene::renderHud()
{
    Text t;
    t.setFont(font);
    t.setCharacterSize(40);

    t.setString(String("Connecta els wiimotes!"));
    t.setPosition(app->getView().getCenter().x - t.getLocalBounds().width/2, app->getView().getCenter().y-200);
    app->draw(t);
    t.setString(String("Pulsa 1+2!"));
    t.setPosition(app->getView().getCenter().x - t.getLocalBounds().width/2, app->getView().getCenter().y-140);
    app->draw(t);
    if(wInput.connectedCount)
    {
        if(wInput.connectedCount == 1)
            t.setString(String("1 jugador connectat."));
        else
            t.setString(String(toString(wInput.connectedCount)+" jugadors connectats."));
        t.setPosition(app->getView().getCenter().x - t.getLocalBounds().width/2, app->getView().getCenter().y+0);
        app->draw(t);

        t.setString(String("Pulsa A per iniciar la partida."));
        t.setPosition(app->getView().getCenter().x - t.getLocalBounds().width/2, app->getView().getCenter().y+60);
        app->draw(t);
    }

    bool start = false;

    for(int i = 0; i < MAX_WIIMOTES; i++)
        if(wInput.wiiControl[i][W_A])
            start = true;

    if(start)
    {
        wInput.stopSearch();
        nextScene = new GameScene(FREEMODE, wInput.connectedCount);
    }
}
