CONFIG -= qt
QMAKE_CFLAGS += -g -O3
QMAKE_CXXFLAGS += -g -O3

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_LFLAGS -= -O1
QMAKE_LFLAGS += -O3
QMAKE_LFLAGS_RELEASE -= -O1
QMAKE_LFLAGS_RELEASE += -O3
QMAKE_LDFLAGS -= -O1
QMAKE_LDFLAGS += -O3

LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lGL -lGLU -lBox2D
SOURCES += main.cpp Particle.cpp Actor.cpp Scene.cpp ParticleEmitter.cpp FireActor.cpp util.cpp \
    BoxActor.cpp \
    GameScene.cpp \
    GroundActor.cpp \
    PlayerActor.cpp \
    WallActor.cpp \
    Model.cpp \
    Enemy.cpp \
    Hexagon.cpp \
    SolidHexagon.cpp \
    BouncyHexagon.cpp \
    StandardEnemy.cpp \
    ShooterEnemy.cpp \
    WaterHexagon.cpp \
    ModelActor.cpp \
    FloatingHexagon.cpp \
    WaterPlane.cpp
HEADERS += Particle.h util.h Actor.h Scene.h ParticleEmitter.h FireActor.h \
    BoxActor.h \
    GameScene.h \
    GroundActor.h \
    PlayerActor.h \
    WallActor.h \
    Model.h \
    Enemy.h \
    Hexagon.h \
    SolidHexagon.h \
    BouncyHexagon.h \
    StandardEnemy.h \
    ShooterEnemy.h \
    WaterHexagon.h \
    ModelActor.h \
    FloatingHexagon.h \
    WaterPlane.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Downloads/Box2D_v2.2.1/Box2D/release/ -lBox2D
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Downloads/Box2D_v2.2.1/Box2D/debug/ -lBox2D
else:symbian: LIBS += -lBox2D
else:unix: LIBS += -L$$PWD/../Downloads/Box2D_v2.2.1/Box2D/ -lBox2D

INCLUDEPATH += $$PWD/../Downloads/Box2D_v2.2.1/
DEPENDPATH += $$PWD/../Downloads/Box2D_v2.2.1/s

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../Downloads/Box2D_v2.2.1/Box2D/release/Box2D.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../Downloads/Box2D_v2.2.1/Box2D/debug/Box2D.lib
else:unix:!symbian: PRE_TARGETDEPS += $$PWD/../Downloads/Box2D_v2.2.1/Box2D/libBox2D.a

OTHER_FILES += \
    vertex.glsl \
    fragment-particle.glsl \
    fragment-light.glsl \
    fragment-model.glsl \
    fragment-glow.glsl \
    vertex-null.glsl \
    fragment-hexagons.glsl \
    vertex-light.glsl \
    vertex-particle.glsl \
    fragment-water.glsl
