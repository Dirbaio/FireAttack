#include "MenuScene.h"
#include<sstream>
#include "GameScene.h"
#include "Input.h"

#define MIN_PLAYERS 1

MenuScene::MenuScene(int objecScore)
{

    hasKeyboard = false;
    menu = true;
    credits = false;

    playMusic("wiideo2");

    changeScoreTime = 0.0;
    changeScoreTimeWindow = 0.2;

    closeTime = 0.0;

    objScore = objecScore;
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
        changeScoreTime += dt;
        closeTime += dt;
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
            else if (wInput.wiiControl[i][W_HOME] && closeTime >= 2.0)
                closeNow = true;
            else if (wInput.wiiControl[i][W_PLUS] && changeScoreTime >= changeScoreTimeWindow)
                objScore = min(1000, objScore+1), changeScoreTime = 0;
            else if (wInput.wiiControl[i][W_MINUS] && changeScoreTime >= changeScoreTimeWindow)
                objScore = max(1, objScore-1), changeScoreTime = 0;

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
            nextScene = new GameScene(FREEMODE, ct, objScore);
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

void MenuScene::writeText(const string text, const int offset = 0, const int fontSize = 40)
{
    Text t;
    t.setFont(font);
    t.setCharacterSize(fontSize);

    t.setString(String(text));
    t.setPosition(app->getView().getCenter().x - t.getLocalBounds().width/2, app->getView().getCenter().y + offset);
    app->draw(t);
}

void MenuScene::renderHud()
{
    if (menu)
    {
        Image img;
        img.loadFromFile("img/logo.png");
        int logoWidth = img.getSize().x;
        int logoHeight = img.getSize().y;
        Texture texture;
        texture.loadFromImage(img, Rect<int>(0,0, logoWidth,logoHeight));
        RectangleShape logo(vec2(logoWidth,logoHeight));
        logo.setTexture(&texture, true);
        logo.setPosition(app->getView().getCenter().x - logo.getLocalBounds().width/2, -80);
        app->draw(logo);

        writeText("Conecta los Wiimotes (pulsa 1+2)", -110);

        int ct = wInput.connectedCount;
        if(hasKeyboard) ct++;

        if(ct)
        {
            if(ct == 1)
                writeText("1 wiimote conectado (max 4)", -60);
            else
                writeText(toString(ct)+" wiimotes conectados (max 4)", -60);

            writeText("Objetivo: "+toString(objScore)+" puntos (puedes cambiarlo con +/-)", 30);

            if (ct < MIN_PLAYERS)
                writeText("Hacen falta minimo "+toString(MIN_PLAYERS)+" jugadores para jugar", 100);
            else
                writeText("Pulsa A para iniciar la partida", 100);

            writeText("Pulsa 1 para ver los creditos", 200);
            writeText("Pulsa HOME para salir del juego", 300);
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

        writeText("FIRE SMASH", -280, 50);
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
        writeText("Ableton Live 8", 290);
    }
}
