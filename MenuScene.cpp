#include "MenuScene.h"
#include<sstream>
#include "GameScene.h"
#include "Input.h"

MenuScene::MenuScene()
{

    hasKeyboard = false;

}

string toString(int n)
{
    stringstream ss;
    ss<<n;
    return ss.str();
}

void MenuScene::update()
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        hasKeyboard = true;

    bool start = false;

    for(int i = 0; i < MAX_WIIMOTES; i++)
        if(wInput.wiiControl[i][W_A])
            start = true;

    if (hasKeyboard)
        start |= sf::Keyboard::isKeyPressed(sf::Keyboard::A);

    if(start)
    {
        wInput.stopSearch();

        int ct = wInput.connectedCount;
        if(hasKeyboard) ct++;

        if(hasKeyboard)
            playerConfigs[0].useWiimote = false;

        for(int i = 0; i < ct; i++)
        {
            playerConfigs[i+(hasKeyboard?1:0)].useWiimote = true;
            playerConfigs[i+(hasKeyboard?1:0)].numWiimote = i;
        }
        nextScene = new GameScene(FREEMODE, ct);
    }
}

void MenuScene::renderHud()
{
    Text t;
    t.setFont(font);
    t.setCharacterSize(40);

    t.setString(String("Connecta els jugadors!"));
    t.setPosition(app->getView().getCenter().x - t.getLocalBounds().width/2, app->getView().getCenter().y-200);
    app->draw(t);
    t.setString(String("Wiimotes: Pulsa 1+2!"));
    t.setPosition(app->getView().getCenter().x - t.getLocalBounds().width/2, app->getView().getCenter().y-140);
    app->draw(t);
    t.setString(String("Teclat: Pulsa espai!"));
    t.setPosition(app->getView().getCenter().x - t.getLocalBounds().width/2, app->getView().getCenter().y-100);
    app->draw(t);

    int ct = wInput.connectedCount;
    if(hasKeyboard) ct++;

    if(ct)
    {
        if(ct == 1)
            t.setString(String("1 jugador connectat."));
        else
            t.setString(String(toString(ct)+" jugadors connectats."));
        t.setPosition(app->getView().getCenter().x - t.getLocalBounds().width/2, app->getView().getCenter().y+0);
        app->draw(t);

        t.setString(String("Pulsa A per iniciar la partida."));
        t.setPosition(app->getView().getCenter().x - t.getLocalBounds().width/2, app->getView().getCenter().y+60);
        app->draw(t);
    }

}
