CXX = g++
BINDIR = bin

#only in debug version
CXXDEBUGFLAGS = -ggdb

#all warnings
CXXFLAGS  = -Wall
CXXFLAGS  += -Wextra

#maximum optimization
CXXFLAGS  += -O3
CXXFLAGS  += -march=native

CXXFLAGS  += -I$(INCDIR)

SRCS      = training.cpp

OBJS      = ${SRCS:%.cpp=%.o}

TARGET    = training_data

%.o:$(SRCDIR)%.cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(CXXDEBUGFLAGS) $<

default: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ -static $(OBJS)
	mkdir -p $(BINDIR); mv *.o $(BINDIR)

.PHONY: clean

clean: 
	rm -r -f $(TARGET)* $(BINDIR) core *~ *.o
	clear
