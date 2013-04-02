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
    WaterPlane.cpp \
    Input.cpp \
    ExplosiveHexagon.cpp
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
    WaterPlane.h \
    Input.h \
    ExplosiveHexagon.h

OTHER_FILES += \
    vertex.glsl \
    fragment-particle.glsl \
    fragment-light.glsl \
    fragment-model.glsl \
    vertex-null.glsl \
    fragment-hexagons.glsl \
    vertex-light.glsl \
    vertex-particle.glsl \
    fragment-water.glsl \
    shaders/vertex-particle.glsl \
    shaders/vertex-null.glsl \
    shaders/vertex-light.glsl \
    shaders/vertex.glsl \
    shaders/fragment-water.glsl \
    shaders/fragment-particle.glsl \
    shaders/fragment-model.glsl \
    shaders/fragment-light.glsl \
    shaders/fragment-hexagons.glsl \
    shaders/fragment-glow.glsl
