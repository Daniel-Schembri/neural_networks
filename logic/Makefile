SRCDIR         = src/
INCDIR         = include/
BINDIR         = bin/

TRAINDIR       = train/
TRAINTARGET    = training_data
TRAINFILE      = trainingData.txt 
TRAINSRC       = training.cpp

CXX = g++

#only in debug version
CXXDEBUGFLAGS = -ggdb

#all warnings
CXXFLAGS  = -Wall
CXXFLAGS  += -Wextra

#maximum optimization
CXXFLAGS  += -O3
CXXFLAGS  += -march=native

CXXFLAGS  += -I$(INCDIR)

SRCS   = net.cpp \
         neuron.cpp \
	     feedForwardNet.cpp \
	     srn.cpp \
	     competitive.cpp \
		 selfOrganizingMap.cpp \
	     main.cpp 

OBJS   = ${SRCS:%.cpp=%.o}

TARGET = neural_network

%.o:$(SRCDIR)%.cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(CXXDEBUGFLAGS) $<

default: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)
	mkdir -p $(BINDIR); mv *.o $(BINDIR)

#.PHONY: auto
#
#auto: $(TARGET)
#
#$(TARGET): $(OBJS)
#	$(CXX) -o $@ $(OBJS)
#	mkdir -p $(BINDIR); mv *.o $(BINDIR)

.PHONY: train

train: 
	cd $(TRAINDIR); make; ./$(TRAINTARGET) > $(TRAINFILE); mv $(TRAINFILE) ../$(SRCDIR)

.PHONY: clean

clean: 
	rm -r -f $(TARGET)* $(BINDIR) core *~ *.o
	cd $(TRAINDIR); make clean
	clear
