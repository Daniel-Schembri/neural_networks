SRCDIR         = src/
INCDIR         = include/
INCDIRLOGIC    = ../logic/include/
INCDIRBOX2D    = libs/Box2D_v2.3.0/Box2D/
INCUSR         = /usr/local/include/
BINDIR         = bin/
   
LDFLAGS = -lX11 -lGL -lGLU -lXxf86vm -L${BOX2Dlib} -L${GLUIlib} -lBox2D -lglui -lglut #-lGLU #-lopengl32 -lglu32 -lfreeglut

BOX2Dlib = libs/Box2D_v2.3.0/Box2D/Build/Box2D/libBox2D.a
BOX2D    = libs/Box2D_v2.3.0/Box2D/
GLUIlib  = libs/Box2D_v2.3.0/Box2D/Build/glui/libglui.a

CXX = g++

#only in debug version
CXXDEBUGFLAGS = -ggdb

#Use c++11
CXXFLAGS = -std=c++11

#all warnings
CXXFLAGS  += -Wall
CXXFLAGS  += -Wextra

#maximum optimization
CXXFLAGS  += -O3
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
     ../logic/src/competitive.cpp \
     ../logic/src/competitiveNeuron.cpp \
     ../logic/src/feedForwardNet.cpp \
     ../logic/src/ffNeuron.cpp \
     ../logic/src/net.cpp \
     ../logic/src/neuron.cpp \
     ../logic/src/selfOrganizingMap.cpp \
     ../logic/src/somNeuron.cpp \
     ../logic/src/srn.cpp \
     ../logic/src/srnNeuron.cpp 

OBJS   = ${SRCS:%.cpp=%.o} 

TARGET = new_graphic

%.o:$(SRCDIR)%.cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(CXXDEBUGFLAGS) $<

default: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)
	mkdir -p $(BINDIR); mv *.o $(BINDIR); mv ../logic/src/*.o $(BINDIR)
    
	
.PHONY: install

install:
	cd $(BOX2D)/Build/; cmake ..; make
	cmake -DBOX2D_INSTALL=ON -DBOX2D_BUILD_SHARED=ON ..
	make
	make install	

.PHONY: auto

auto: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)
	mkdir -p $(BINDIR); mv *.o $(BINDIR); mv ../logic/src/*.o $(BINDIR)

.PHONY: clean

clean: 
	rm -r -f $(TARGET)* $(BINDIR) core *~ *.o
	clear
