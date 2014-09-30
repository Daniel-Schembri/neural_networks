SRCDIR         = src/
INCDIR         = include/
INCDIRBOX2D    = libs/Box2D_v2.3.0/Box2D/
INCUSR         = /usr/local/include/
BINDIR         = bin/

LDFLAGS = -lX11 -lGL -lGLU -lXxf86vm -L${BOX2Dlib} -lBox2D #-lopengl32 -lglu32 -lfreeglut

BOX2Dlib = libs/Box2D_v2.3.0/Box2D/Build/Box2D/libBox2D.a
BOX2D = libs/Box2D_v2.3.0/Box2D/

CXX = g++

#only in debug version
CXXDEBUGFLAGS = -ggdb

#all warnings
CXXFLAGS  = -Wall
CXXFLAGS  += -Wextra

#maximum optimization
CXXFLAGS  += -O3
CXXFLAGS  += -march=native

CXXFLAGS  += -I$(INCDIR) -I$(INCDIRBOX2D) -I$(INCUSR)

SRCS   = graphics.cpp \
	physics.cpp \
	 main.cpp 

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
