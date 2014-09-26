CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.cpp basics.cpp quadcode.cpp relation2D.cpp trie.cpp utils.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=memoria

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

