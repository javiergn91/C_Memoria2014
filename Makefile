LIBCDSDIR=libcds-master
LIBCDS=$(LIBCDSDIR)/src/.libs/libcds.a
INCLUDES=-I$(LIBCDSDIR)/include/
CC=g++
CFLAGS=-c 
LDFLAGS=
SOURCES=main.cpp basics.cpp quadcode.cpp relation2D.cpp trie.cpp utils.cpp spbitmap.cpp quadcodestruct.cpp common.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=memoria

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(INCLUDES) $(OBJECTS) -o $@ $(LIBCDS)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@ $(LIBCDS)

