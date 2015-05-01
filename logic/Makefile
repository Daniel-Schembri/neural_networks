SRCDIR         = src/
INCDIR         = include/
BINDIR         = bin/

TRAINDIR       = train/
TRAINFILE      = trainingData.txt 
TRAINSRC_C     = calculatedData.py
TRAINSRC_R     = randomData.py
#default value in case no paramter is given
SAMPLES        = 20000
TOPOLOGY       = 2 8 1

CXX = g++

#only in debug version
CXXDEBUGFLAGS = -ggdb

#use c++11
CXXFLAGS  = -std=c++11

#all warnings
CXXFLAGS  += -Wall
CXXFLAGS  += -Wextra

#maximum optimization
CXXFLAGS  += -O3
CXXFLAGS  += -march=native

CXXFLAGS  += -I$(INCDIR)

SRCS   = script.cpp \
	     net.cpp \
	     feedForwardNet.cpp \
	     srn.cpp \
	     competitive.cpp \
		 selfOrganizingMap.cpp \
         neuron.cpp \
		 ffNeuron.cpp \
		 srnNeuron.cpp \
		 somNeuron.cpp \
		 competitiveNeuron.cpp \
		 agent.cpp \
		 evolutionary.cpp \
	     main.cpp 

OBJS   = ${SRCS:%.cpp=%.o}

TARGET = neural_network

%.o:$(SRCDIR)%.cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $< #$(CXXDEBUGFLAGS) $<

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

.PHONY: training_c

training_c:
	cd $(TRAINDIR); ./$(TRAINSRC_C) $(TOPOLOGY); mv $(TRAINFILE) ../$(SRCDIR)/training

.PHONY: training_r

training_r:
	cd $(TRAINDIR); ./$(TRAINSRC_R) $(TOPOLOGY) $(SAMPLES); mv $(TRAINFILE) ../$(SRCDIR)/training

.PHONY: clean

clean: 
	rm -r -f $(TARGET)* $(BINDIR) core *~ *.o
	clear
