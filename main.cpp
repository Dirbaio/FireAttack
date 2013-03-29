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

using namespace std;
using namespace sf;

float rotx = 0;
float roty = 0;

//Defined as extern in util.h
float dt;
float tim = 0;
Scene* sc;
Window* theApp;

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
    sh->bind();

    sh->setParameter("aspectRatio", float(theApp->getSize().x)/float(theApp->getSize().y));

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

void makeHexagon()
{
    float DIS = 1.154700538;
    float COS = 0.577350269;
    float SIN = 0.866025404;
    float x[] = {-1.0, 0.0, 1.0, 1.0, 0.0, -1.0, -1.0};
    float y[] = {1.0, -1.0};
    float z[] = {COS, DIS, COS, -COS, -DIS, -COS, COS};
    float nx[] = {-0.5, 0.5, 1.0, 0.5, -0.5, -1.0};
    float nz[] = {SIN, SIN, 0.0, -SIN, -SIN, 0.0};
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            if (i == 0)
            {
                hexVert.push_back(vec3(x[j], y[i], z[j]));
                hexVert.push_back(vec3(x[j+1], y[i], z[j+1]));
                hexVert.push_back(vec3(0.0, y[i], 0.0));
                for (int k = 0; k < 3; k++) hexNorm.push_back(vec3(0.0, 1.0, 0.0));
                hexTexCoord.push_back(vec2((6.0-j)/6.0, 1.0/3.0));
                hexTexCoord.push_back(vec2((5.0-j)/6.0, 1.0/3.0));
                hexTexCoord.push_back(vec2((5.5-j)/6.0, 0.0));

                hexVert.push_back(vec3(x[j], y[0], z[j]));
                hexVert.push_back(vec3(x[j+1], y[1], z[j+1]));
                hexVert.push_back(vec3(x[j+1], y[0], z[j+1]));
                for (int k = 0; k < 3; k++) hexNorm.push_back(vec3(nx[j], 0.0, nz[j]));
                hexTexCoord.push_back(vec2((6.0-j)/6.0, 1.0/3.0));
                hexTexCoord.push_back(vec2((5.0-j)/6.0, 2.0/3.0));
                hexTexCoord.push_back(vec2((5.0-j)/6.0, 1.0/3.0));
            }
            else
            {
                hexVert.push_back(vec3(x[j+1], y[i], z[j+1]));
                hexVert.push_back(vec3(x[j], y[i], z[j]));
                hexVert.push_back(vec3(0.0, y[i], 0.0));
                for (int k = 0; k < 3; k++) hexNorm.push_back(vec3(0.0, -1.0, 0.0));
                hexTexCoord.push_back(vec2((5.0-j)/6.0, 2.0/3.0));
                hexTexCoord.push_back(vec2((6.0-j)/6.0, 2.0/3.0));
                hexTexCoord.push_back(vec2((5.5-j)/6.0, 1.0));

                hexVert.push_back(vec3(x[j+1], y[1], z[j+1]));
                hexVert.push_back(vec3(x[j], y[0], z[j]));
                hexVert.push_back(vec3(x[j], y[1], z[j]));
                for (int k = 0; k < 3; k++) hexNorm.push_back(vec3(nx[j], 0.0, nz[j]));
                hexTexCoord.push_back(vec2((5.0-j)/6.0, 2.0/3.0));
                hexTexCoord.push_back(vec2((6.0-j)/6.0, 1.0/3.0));
                hexTexCoord.push_back(vec2((6.0-j)/6.0, 2.0/3.0));
            }
        }
    }
}

int main(int argc, char** argv)
{
    // Create the main window
    Window app (sf::VideoMode(window_width, window_height, 32), "SFML OpenGL");
    theApp = &app;

    //	app.setVerticalSyncEnabled(true);

    // Create a clock for measuring time elapsed
    sf::Clock clock;
    sf::Clock profiler;

    makeHexagon(); // Create the hexagon pattern

    // Set color and depth clear value
    glClearDepth(1.0f);
    glClearColor(0.f, 0.f, 0.f, 0.f);

    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);

    initFrameBuffer();

    glDisable(GL_CULL_FACE);

    Shader* lightShader = loadShader("vertex2.glsl", "fragment-light.glsl");
    Shader* particleShader = loadShader("vertex2.glsl", "fragment-particle.glsl");
    Shader* glowShader = loadShader("vertex-null.glsl", "fragment-glow.glsl");

    srand(time(NULL));

    float frameTime = 0;
    int frameCount = 0;

    sc = new GameScene();

    float updateTime = 0;
    float renderTime = 0;
    float renderPartTime = 0;

    // Start game loop
    while (app.isOpen())
    {
        // Process events
        sf::Event event;
        while (app.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
            {
                app.close();
                return 0;
            }

            // Escape key : exit
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == Keyboard::Escape))
            {
                app.close();
                return 0;
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

        app.setActive();

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
        glEnable(GL_LIGHTING);
        glDisable(GL_BLEND);
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
        glPushAttrib(GL_VIEWPORT_BIT | GL_ENABLE_BIT);
        glViewport(0, 0, window_width, window_height);

        glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
/*
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
*/
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
        sc->renderParticles(true);
        setupShader(particleShader);
        sc->renderParticles(false);

        if(sc->nextScene)
        {
            Scene* next = sc->nextScene;
            delete sc;
            sc = next;
        }
        app.display();
        countTime(renderPartTime, profiler.getElapsedTime().asMilliseconds());
    }

    return EXIT_SUCCESS;
}




