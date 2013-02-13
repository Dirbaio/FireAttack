#############################################################################
# Makefile for building: main
# Generated by qmake (2.01a) (Qt 4.8.3) on: Wed Feb 13 18:45:47 2013
# Project:  main.pro
# Template: app
# Command: /usr/bin/qmake -spec /usr/share/qt4/mkspecs/linux-g++ -o Makefile main.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_WEBKIT
CFLAGS        = -pipe -g -O3 -O2 -Wall -W $(DEFINES)
CXXFLAGS      = -pipe -g -O3 -Wall -W $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I../Downloads/Box2D_v2.2.1
LINK          = g++
LFLAGS        = -O3 -Wl,-O1 -O3
LIBS          = $(SUBLIBS)   -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lGL -lGLU -L/home/dirbaio/Fire\ Attack/../Downloads/Box2D_v2.2.1/Box2D/ -lBox2D 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.cpp \
		Particle.cpp \
		Actor.cpp \
		Scene.cpp \
		ParticleEmitter.cpp \
		FireActor.cpp \
		util.cpp \
		BoxActor.cpp \
		GameScene.cpp \
		GroundActor.cpp \
		PlayerActor.cpp \
		WallActor.cpp 
OBJECTS       = main.o \
		Particle.o \
		Actor.o \
		Scene.o \
		ParticleEmitter.o \
		FireActor.o \
		util.o \
		BoxActor.o \
		GameScene.o \
		GroundActor.o \
		PlayerActor.o \
		WallActor.o
DIST          = /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		main.pro
QMAKE_TARGET  = main
DESTDIR       = 
TARGET        = main

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET): /home/dirbaio/Fire\ Attack/../Downloads/Box2D_v2.2.1/Box2D/libBox2D.a $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: main.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf
	$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++ -o Makefile main.pro
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/common/gcc-base.conf:
/usr/share/qt4/mkspecs/common/gcc-base-unix.conf:
/usr/share/qt4/mkspecs/common/g++-base.conf:
/usr/share/qt4/mkspecs/common/g++-unix.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/release.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
qmake:  FORCE
	@$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++ -o Makefile main.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/main1.0.0 || $(MKDIR) .tmp/main1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/main1.0.0/ && (cd `dirname .tmp/main1.0.0` && $(TAR) main1.0.0.tar main1.0.0 && $(COMPRESS) main1.0.0.tar) && $(MOVE) `dirname .tmp/main1.0.0`/main1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/main1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: 

####### Compile

main.o: main.cpp Particle.h \
		util.h \
		Actor.h \
		ParticleEmitter.h \
		Scene.h \
		GameScene.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

Particle.o: Particle.cpp Particle.h \
		util.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Particle.o Particle.cpp

Actor.o: Actor.cpp Actor.h \
		ParticleEmitter.h \
		Particle.h \
		util.h \
		Scene.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Actor.o Actor.cpp

Scene.o: Scene.cpp Scene.h \
		Particle.h \
		util.h \
		Actor.h \
		ParticleEmitter.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Scene.o Scene.cpp

ParticleEmitter.o: ParticleEmitter.cpp ParticleEmitter.h \
		Particle.h \
		util.h \
		Actor.h \
		Scene.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ParticleEmitter.o ParticleEmitter.cpp

FireActor.o: FireActor.cpp FireActor.h \
		Actor.h \
		ParticleEmitter.h \
		Particle.h \
		util.h \
		Scene.h \
		BoxActor.h \
		GameScene.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o FireActor.o FireActor.cpp

util.o: util.cpp util.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o util.o util.cpp

BoxActor.o: BoxActor.cpp BoxActor.h \
		Actor.h \
		ParticleEmitter.h \
		Particle.h \
		util.h \
		GameScene.h \
		Scene.h \
		PlayerActor.h \
		FireActor.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o BoxActor.o BoxActor.cpp

GameScene.o: GameScene.cpp GameScene.h \
		Scene.h \
		Particle.h \
		util.h \
		BoxActor.h \
		Actor.h \
		ParticleEmitter.h \
		GroundActor.h \
		PlayerActor.h \
		FireActor.h \
		WallActor.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o GameScene.o GameScene.cpp

GroundActor.o: GroundActor.cpp GroundActor.h \
		Actor.h \
		ParticleEmitter.h \
		Particle.h \
		util.h \
		Scene.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o GroundActor.o GroundActor.cpp

PlayerActor.o: PlayerActor.cpp PlayerActor.h \
		Actor.h \
		ParticleEmitter.h \
		Particle.h \
		util.h \
		FireActor.h \
		GameScene.h \
		Scene.h \
		BoxActor.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o PlayerActor.o PlayerActor.cpp

WallActor.o: WallActor.cpp WallActor.h \
		Actor.h \
		ParticleEmitter.h \
		Particle.h \
		util.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o WallActor.o WallActor.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

