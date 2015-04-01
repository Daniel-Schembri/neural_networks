SRCDIR         = src/
INCDIR         = include/
INCDIRBOX2D    = libs/Box2D_v2.3.0/Box2D/
INCDIRTESTS    = include/Tests/
INCUSR         = /usr/local/include/
BINDIR         = bin/

LDFLAGS = -lX11 -lGL -lGLU -lXxf86vm -L${BOX2Dlib} -L${GLUIlib} -lBox2D -lglui -lglut #-lGLU #-lopengl32 -lglu32 -lfreeglut

BOX2Dlib = libs/Box2D_v2.3.0/Box2D/Build/Box2D/libBox2D.a
BOX2D = libs/Box2D_v2.3.0/Box2D/
GLUIlib = libs/Box2D_v2.3.0/Box2D/Build/glui/libglui.a

CXX = g++

#only in debug version
CXXDEBUGFLAGS = -ggdb

#all warnings
CXXFLAGS  = -Wall
CXXFLAGS  += -Wextra

#maximum optimization
CXXFLAGS  += -O3
CXXFLAGS  += -march=native

CXXFLAGS  += -I$(INCDIR) -I$(INCDIRBOX2D) -I$(INCUSR) -I$(INCDIRTESTS)

SRCS   = Render.cpp \
	 Test.cpp \
	 neuron.cpp \
	 net.cpp \
     Creature.cpp \
     evolutionary.cpp \
     NeuralWorld.cpp \
	 Main.cpp 
OBJS   = ${SRCS:%.cpp=%.o}

TARGET = graphic

%.o:$(SRCDIR)%.cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(CXXDEBUGFLAGS) $<

default: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)
	mkdir -p $(BINDIR); mv *.o $(BINDIR)
	
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
	mkdir -p $(BINDIR); mv *.o $(BINDIR)

.PHONY: clean

clean: 
	rm -r -f $(TARGET)* $(BINDIR) core *~ *.o
	clear
