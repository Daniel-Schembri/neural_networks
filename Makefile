SRCDIR         = src/
INCDIR         = include/
DOCDIR         = doc/
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
#CXXFLAGS  += -O3
#CXXFLAGS  += -march=native

CXXFLAGS  += -I$(INCDIR)

SRCS   = net.cpp \
         neuron.cpp \
	     feedForwardNet.cpp \
	     srn.cpp \
	     main.cpp 

OBJS   = ${SRCS:%.cpp=%.o}

TARGET = neural_network

%.o:$(SRCDIR)%.cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(CXXDEBUGFLAGS) $<

default: $(TARGET)

$(TARGET): $(OBJS)
#	$(CXX) -o $@ -static $(OBJS)
	$(CXX) -o $@ $(OBJS)
	mkdir -p $(BINDIR); mv *.o $(BINDIR)

.PHONY: doc

doc: 
	cd $(DOCDIR); make  

.PHONY: train

train: 
	cd $(TRAINDIR); make; ./$(TRAINTARGET) > $(TRAINFILE); mv $(TRAINFILE) ../$(SRCDIR)

.PHONY: clean

clean: 
	rm -r -f $(TARGET)* $(BINDIR) core *~ *.o
	cd $(DOCDIR); make clean
	cd $(TRAINDIR); make clean
	clear
