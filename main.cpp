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

int window_width = 500;
int window_height = 500;

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

int main(int argc, char** argv)
{
    // Create the main window
    Window app (sf::VideoMode(1024, 768, 32), "SFML OpenGL");
    theApp = &app;

    //	app.setVerticalSyncEnabled(true);

    // Create a clock for measuring time elapsed
    sf::Clock clock;
    sf::Clock profiler;

    // Set color and depth clear value
    glClearDepth(1.0f);
    glClearColor(0.f, 0.f, 0.f, 0.f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);

    initFrameBuffer();

    GLfloat mat_specular[] = { 0.5, 1.0 , 0.8, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat light_position[] = { 200.0, 100.0, 300.0, 0.0 };
    //	glShadeModel (GL_SMOOTH);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);



    //glDisable(GL_LIGHTING);
    //    glEnable(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);

    /*
    //TODO Hacerlo segun la ventana
    int w = 512;
    int h = 512;

    GLuint mFBO;
    GLuint mTexColor, mTexNormal, mTexDepth;

    glGenFramebuffersEXT(1, &mFBO);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFBO);

    glGenTextures(1, &mTexColor);
    glBindTexture(GL_TEXTURE_2D, mTexColor);
    //<texture params>
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, mTexColor, 0);

    glGenTextures(1, &mTexNormal);
    glBindTexture(GL_TEXTURE_2D, mTexNormal);
    //<Texture params>
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, mTexNormal, 0);

    glGenTextures(1, &mTexDepth);
    glBindTexture(GL_TEXTURE_2D, mTexDepth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, mTexDepth, 0);

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT };
    glDrawBuffers(2, buffers);
    */

    Shader sh; sh.loadFromFile("vertex.glsl", "fragment.glsl");
    Shader sh2; sh2.loadFromFile("vertex.glsl", "fragment2.glsl");

    srand(time(NULL));

    float frameTime = 0;
    int frameCount = 0;

    sc = new GameScene();

    float updateTime = 0;
    float renderTime = 0;
    float renderPartTime = 0;

    int windowWidth, windowHeight;
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
                glViewport(0, 0, event.size.width, event.size.height);
                // Setup a perspective projection
                windowWidth = event.size.width;
                windowHeight = event.size.height;
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

        profiler.restart();
        sc->update();
        countTime(updateTime, profiler.getElapsedTime().asMilliseconds());




        profiler.restart();
        glEnable(GL_LIGHTING);
        glDisable(GL_BLEND);
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo); // Bind our frame buffer for rendering
        glPushAttrib(GL_VIEWPORT_BIT | GL_ENABLE_BIT); // Push our glEnable and glViewport states
        glViewport(0, 0, window_width, window_height); // Set the size of the frame buffer view port

        glClearColor (0.0f, 0.0f, 0.0f, 1.0f); // Set the clear colour
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the depth and colour buffers

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(90.f, float(windowWidth)/float(windowHeight), 0.01f, 50.f);

        sh2.bind();
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        sc->render();
        sh2.unbind();

        glPopAttrib(); // Restore our glEnable and glViewport states
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // Unbind our texture

        countTime(renderTime, profiler.getElapsedTime().asMilliseconds());




        profiler.restart();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glDisable(GL_CULL_FACE);
        glDisable(GL_LIGHTING);

        sh.bind();

        float ratio = float(theApp->getSize().x) / float(theApp->getSize().y);
        vec3 camVec = sc->cameraLookAt - sc->cameraPos;
        normalize(camVec);
        vec3 up2 (0, 1, 0);
        vec3 right = cross(camVec, up2);
        normalize(right);
        right *= ratio;
        vec3 up = cross(right, camVec);
        normalize(up);
        sh.setParameter("w", theApp->getSize().x);
        sh.setParameter("h", theApp->getSize().y);
        sh.setParameter("cameraUp", up);
        sh.setParameter("cameraRight", right);
        sh.setParameter("cameraFront", camVec);
        sh.setParameter("cameraPos",  sc->cameraPos);

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        glDisable(GL_DEPTH_TEST);
        glClearColor (0.0f, 0.0f, 0.0f, 1.0f); // Set the clear colour
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the depth and colour buffers

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

        sc->renderParticles();
/*

        glColor3f(1, 1, 1);

        glBegin(GL_QUADS);
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glVertex3f(-1.0f, 1.0f, 0.0f);
        glVertex3f(1.0f, 1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 0.0f);
        glEnd();*/



        glBindTexture(GL_TEXTURE_2D, 0); // Unbind any textures
        sh.unbind();


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




