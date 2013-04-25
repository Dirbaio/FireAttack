#define GL_GLEXT_PROTOTYPES 1

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/glext.h"
#include <iostream>
#include <cmath>
#include <list>
#include "Particle.h"
#include "Actor.h"
#include "Scene.h"
#include "GameScene.h"
#include "util.h"
#include "WiimoteInput.h"
#include "GameScene.h"
#include "MenuScene.h"

using namespace std;
using namespace sf;

Scene* sc;

void countTime(float& t, float t2)
{
    t = t*0.95 + t2 * 0.05;
}

unsigned int fbo;
unsigned int fbo_depth;
unsigned int fbo_texture1, fbo_texture2;

int window_width = 1200;
int window_height = 800;

void initFrameBuffer(void)
{
    //Color texture
    glGenTextures(1, &fbo_texture1);
    glBindTexture(GL_TEXTURE_2D, fbo_texture1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window_width, window_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);


    //Normal texture
    glGenTextures(1, &fbo_texture2);
    glBindTexture(GL_TEXTURE_2D, fbo_texture2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window_width, window_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);


    glGenTextures(1, &fbo_depth);
    glBindTexture(GL_TEXTURE_2D, fbo_depth);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, window_width, window_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);

    glBindTexture(GL_TEXTURE_2D, 0);

    glGenFramebuffersEXT(1, &fbo); // Generate one frame buffer and store the ID in fbo
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo); // Bind our frame buffer

    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, fbo_texture1, 0);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, fbo_texture2, 0);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, fbo_depth, 0);

    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT); // Check that status of our generated frame buffer

    if (status != GL_FRAMEBUFFER_COMPLETE_EXT) // If the frame buffer does not report back as complete
    {
        std::cout << "Couldn't create frame buffer" << std::endl; // Output an error to the console
        exit(0); // Exit the application
    }

    GLenum buffers[] = {GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT};
    glDrawBuffers(2, buffers);

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // Unbind our frame buffer
}


void setupShader(Shader* sh)
{
    Shader::bind(sh);

    int program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &program); //Trololo
    GLint tex1Loc = glGetUniformLocation(program, "tex1");
    GLint tex2Loc = glGetUniformLocation(program, "tex2");
    GLint tex3Loc = glGetUniformLocation(program, "tex3");

    glUniform1i(tex1Loc, 0);
    glUniform1i(tex2Loc, 1);
    glUniform1i(tex3Loc, 2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo_texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, fbo_texture2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, fbo_depth);
}



int main(int argc, char** argv)
{
    // Create the main window
    window_width = sf::VideoMode::getFullscreenModes()[0].width;
    window_height = sf::VideoMode::getFullscreenModes()[0].height;
    app = new RenderWindow(sf::VideoMode(window_width, window_height, 32), "Fire Attack", sf::Style::None);
    font.loadFromFile("BitDarling.ttf");
    //	app->setVerticalSyncEnabled(true);
    startWiimoteInput();

    // Create a clock for measuring time elapsed

    sf::Clock clock;
    sf::Clock profiler;

    makeHexagon(); // Create the hexagon pattern

    // Set color and depth clear value
    glClearDepth(1.0f);
    glClearColor(0.f, 0.f, 0.f, 0.f);


    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);
    initFrameBuffer();

    Shader* lightShader = loadShader("vertex-light.glsl", "fragment-light.glsl");
    Shader* particleShader = loadShader("vertex-particle.glsl", "fragment-particle.glsl");
    Shader* glowShader = loadShader("vertex-null.glsl", "fragment-glow.glsl");
    Shader* waterShader = loadShader("vertex-null.glsl", "fragment-water.glsl");

    srand(time(NULL));

    float frameTime = 0;
    int frameCount = 0;

    //sc = new GameScene(FREEMODE);
    sc = new MenuScene();

    float updateTime = 0;
    float renderTime = 0;
    float renderPartTime = 0;

    // Start game loop
    while (app->isOpen())
    {
        // Process events
        bool closed = false;
        sf::Event event;
        while (app->pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
            {
                app->close();
                closed = true;
                break;
            }

            // Escape key : exit
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == Keyboard::Escape))
            {
                app->close();
                closed = true;
                break;
            }

            // Resize event : adjust viewport
            if (event.type == sf::Event::Resized)
            {
                window_width = event.size.width;
                window_height = event.size.height;
                glBindTexture(GL_TEXTURE_2D, fbo_texture1);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window_width, window_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
                glBindTexture(GL_TEXTURE_2D, fbo_texture2);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window_width, window_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
                glBindTexture(GL_TEXTURE_2D, fbo_depth);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, window_width, window_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);

                glViewport(0, 0, window_width, window_height);
            }
        }

        if (closed) break;

        app->setActive();

        dt = clock.getElapsedTime().asSeconds();
        clock.restart();
        tim += dt;

        frameTime += dt;
        frameCount++;

        if(frameTime >= 1)
        {
            cout<<"FPS "<<frameCount/frameTime<<", Particles "<<sc->particles.size()<<", Time "<<updateTime<<" "<<renderTime<<" "<<renderPartTime<<endl;
            frameTime = 0;
            frameCount = 0;
        }


        //UPDATE SCENE
        //============
        profiler.restart();
        sc->update();
        countTime(updateTime, profiler.getElapsedTime().asMilliseconds());



        //FIRST PASS
        //==========

        profiler.restart();
        glDisable(GL_BLEND);
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
        glPushAttrib(GL_VIEWPORT_BIT | GL_ENABLE_BIT);
        glViewport(0, 0, window_width, window_height);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(90.f, float(window_width)/float(window_height), 0.01f, 50.f);

        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        sc->render();

        glPopAttrib(); // Restore our glEnable and glViewport states
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // Unbind our texture

        countTime(renderTime, profiler.getElapsedTime().asMilliseconds());


        //SECOND PASS
        //===========

        profiler.restart();
        glDisable(GL_CULL_FACE);
        glDisable(GL_LIGHTING);

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        glDisable(GL_DEPTH_TEST);
        glClearColor (0.0f, 0.0f, 0.0f, 1.0f); // Set the clear colour

        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the depth and colour buffers


        setupShader(glowShader);
        glBegin(GL_QUADS);
        glTexCoord2f(-1, -1); glVertex2f(-1, -1);
        glTexCoord2f(1, -1); glVertex2f(1, -1);
        glTexCoord2f(1, 1); glVertex2f(1, 1);
         glTexCoord2f(-1, 1); glVertex2f(-1, 1);
        glEnd();

        setupShader(lightShader);
        lightShader->setParameter("aspectRatio", float(app->getSize().x)/float(app->getSize().y));
        sc->renderLights();

        setupShader(particleShader);
        particleShader->setParameter("aspectRatio", float(app->getSize().x)/float(app->getSize().y));
        particleShader->setParameter("isReflection", 1);
        sc->renderParticles(true);

        setupShader(waterShader);

        float modelview[16];
        float inverseLookAt[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
        gluInvertMatrix(modelview, inverseLookAt);
        int program;
        glGetIntegerv(GL_CURRENT_PROGRAM, &program); //Trololo
        int loc = glGetUniformLocation(program, "inverseLookAt");
        if(loc != -1)
            glUniformMatrix4fv(loc, 1, false, inverseLookAt);

        waterShader->setParameter("aspectRatio", float(app->getSize().x)/float(app->getSize().y));
        waterShader->setParameter("time", tim);
        waterShader->setParameter("cameraPos", sc->cameraPos);
        vec3 camVec = sc->cameraLookAt - sc->cameraPos;
        normalize(camVec);
        vec3 up2 (0, 1, 0);
        vec3 right = cross(camVec, up2);
        normalize(right);
        vec3 up = cross(right, camVec);
        normalize(up);
        waterShader->setParameter("cameraDir", camVec);
        waterShader->setParameter("cameraRight", right);
        waterShader->setParameter("cameraUp", up);

        glBegin(GL_QUADS);
        glTexCoord2f(-1, -1); glVertex2f(-1, -1);
        glTexCoord2f(1, -1); glVertex2f(1, -1);
        glTexCoord2f(1, 1); glVertex2f(1, 1);
        glTexCoord2f(-1, 1); glVertex2f(-1, 1);
        glEnd();

        setupShader(particleShader);
        particleShader->setParameter("aspectRatio", float(app->getSize().x)/float(app->getSize().y));
        particleShader->setParameter("isReflection", 0);
        sc->renderParticles(false);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        Shader::bind(NULL);
        app->pushGLStates();
        app->resetGLStates();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        sc->renderHud();

        app->popGLStates();

        if(sc->nextScene)
        {
            Scene* next = sc->nextScene;
            delete sc;
            sc = next;
        }
        app->display();
        countTime(renderPartTime, profiler.getElapsedTime().asMilliseconds());
    }

    stopWiimoteInput();

    return EXIT_SUCCESS;
}





