#include "MenuScene.h"
#include<sstream>
#include "GameScene.h"
#include "Input.h"

MenuScene::MenuScene()
{

    hasKeyboard = false;
    menu = true;
    credits = false;

}

string toString(int n)
{
    stringstream ss;
    ss<<n;
    return ss.str();
}

void MenuScene::update()
{
    if (menu)
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            hasKeyboard = true;

        int ct = wInput.connectedCount;
        if(hasKeyboard) ct++;

        bool start = false;

        for(int i = 0; i < MAX_WIIMOTES; i++)
        {
            if(wInput.wiiControl[i][W_A] && ct >= MIN_PLAYERS)
                start = true;
            else if(wInput.wiiControl[i][W_ONE])
                menu = false, credits = true;
        }

        if (hasKeyboard)
            start |= (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && ct >= MIN_PLAYERS);

        if(start)
        {
            wInput.stopSearch();

            ct = wInput.connectedCount;
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
    else if (credits)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            credits = false, menu = true;

        for (int i = 0; i < MAX_WIIMOTES; i++)
            if (wInput.wiiControl[i][W_B])
                credits = false, menu = true;

    }
}

void MenuScene::writeText(const string text, const int offset = 0)
{
    Text t;
    t.setFont(font);
    t.setCharacterSize(40);

    t.setString(String(text));
    t.setPosition(app->getView().getCenter().x - t.getLocalBounds().width/2, app->getView().getCenter().y + offset);
    app->draw(t);
}

void MenuScene::renderHud()
{
    if (menu)
    {
        writeText("Conecta los Wiimotes (pulsa 1+2)", -200);

        int ct = wInput.connectedCount;
        if(hasKeyboard) ct++;

        if(ct)
        {
            if(ct == 1)
                writeText("1 wiimote conectado");
            else
                writeText(toString(ct)+" wiimotes conectados");

            if (ct < MIN_PLAYERS)
                writeText("hacen falta minimo "+toString(MIN_PLAYERS)+" jugadores para jugar", 60);
            else
                writeText("Pulsa A para iniciar la partida", 60);

            writeText("Pulsa 1 para ver los creditos", 160);
            writeText("Pulsa HOME para salir del juego", 260);
        }
    }
    else if (credits)
    {
        Text t;
        t.setFont(font);
        t.setCharacterSize(40);

        t.setString(String("Pulsa B para volver al menu"));
        t.setPosition(20, 20);
        app->draw(t);

        writeText("FIRE SMASH", -280);
        writeText("Desarrollado para Wiideojuegos 2012 por:", -200);
        writeText("David Balaghi Buil", -150);
        writeText("Dario Nieuwenhuis Nivela", -110);
        writeText("Librerias utilizadas:", -30);
        writeText("SFML 2.0", 20);
        writeText("BOX2D", 60);
        writeText("WiiUse", 100);
        writeText("Software utilizado:", 160);
        writeText("Qt Creator", 210);
        writeText("GIMP", 250);
        writeText("Ableton", 290);
    }
}
