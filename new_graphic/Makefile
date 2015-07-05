SRCDIR         = src/
INCDIR         = include/
INCDIRLOGIC    = ../logic/include/
INCDIRBOX2D    = libs/Box2D_v2.3.0/Box2D/
INCUSR         = /usr/local/include/
BINDIR         = bin/

BOX2D    = libs/Box2D_v2.3.0/Box2D/
BOX2Dlib = $(BOX2D)Build/Box2D/libBox2D.a
GLUIlib  = $(BOX2D)Build/glui/libglui.a
#FREEGLUTlib = $(BOX2D)Build/freeglut/libfreeglut_static.a

LDFLAGS = -lX11 -lGL -lGLU -lXxf86vm ${BOX2Dlib} ${GLUIlib} -lglut #${FREEGLUTlib} #-lglut -lGLU #-lopengl32 -lglu32 -lfreeglut

CXX = g++

#only in debug version
#CXXDEBUGFLAGS = -ggdb

#Use c++11
CXXFLAGS = -std=c++11

#all warnings
#CXXFLAGS  += -Wall
#CXXFLAGS  += -Wextra

#maximum optimization
CXXFLAGS  += -O3 -g
CXXFLAGS  += -march=native

CXXFLAGS  += -I$(INCDIR) -I$(INCDIRBOX2D) -I$(INCUSR) -I$(INCDIRLOGIC)


SRCS = gui.cpp \
     main.cpp \
     neuralWorld.cpp \
     render.cpp \
     test.cpp \
     ../logic/src/script.cpp \
     ../logic/src/agent.cpp \
     ../logic/src/evolutionary.cpp \
     ../logic/src/feedForwardNet.cpp \
     ../logic/src/ffNeuron.cpp \
     ../logic/src/net.cpp \
     ../logic/src/neuron.cpp

OBJS   = ${SRCS:%.cpp=%.o} 

TARGET = new_graphic

%.o:$(SRCDIR)%.cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(CXXDEBUGFLAGS) $<

.PHONY: libs auto clean default

default: $(TARGET)

$(TARGET): $(OBJS) libs
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)
	mkdir -p $(BINDIR); mv *.o $(BINDIR); mv ../logic/src/*.o $(BINDIR)

libs:
	cd $(BOX2D)/Build/; cmake ..; make
#	cmake -DBOX2D_INSTALL=ON -DBOX2D_BUILD_SHARED=ON ..
#	make
#	make install	


auto: $(TARGET)

clean: 
	rm -r -f $(TARGET)* $(BINDIR) core *~ *.o
	clear
