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

LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lGL -lGLU -lBox2D -lwiiuse
SOURCES += main.cpp Particle.cpp Actor.cpp Scene.cpp ParticleEmitter.cpp FireActor.cpp util.cpp \
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
    ModelActor.cpp \
    FloatingHexagon.cpp \
    WaterPlane.cpp \
    Input.cpp \
    ExplosiveHexagon.cpp \
    ExplosionActor.cpp \
    StickyEnemy.cpp \
    TrapHexagon.cpp \
    MagmaHexagon.cpp \
    Level.cpp \
    LevelIO.cpp \
    StaticHexagon.cpp \
    MenuScene.cpp \
    WiimoteInput.cpp
HEADERS += Particle.h util.h Actor.h Scene.h ParticleEmitter.h FireActor.h \
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
    ModelActor.h \
    FloatingHexagon.h \
    WaterPlane.h \
    Input.h \
    ExplosiveHexagon.h \
    ExplosionActor.h \
    StickyEnemy.h \
    TrapHexagon.h \
    MagmaHexagon.h \
    Level.h \
    LevelIO.h \
    StaticHexagon.h \
    MenuScene.h \
    WiimoteInput.h

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
