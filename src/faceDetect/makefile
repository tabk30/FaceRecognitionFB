CC = g++
CFLAGS 	= -c -Wall -O3  -I -lmysqlcppconn 

#include Libs
#include OpenCV
CFLAGS 		+= $(shell pkg-config opencv --cflags)
LDFLAGS 	+= $(shell pkg-config opencv --libs)

SOURCES		= $(wildcard ./src/faceDetectInImage.cpp)
OBJECTS		= $(SOURCES:.cpp=.o)

EXECUTABLE = face_detect

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -lmysqlcppconn $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(CURDIR)/$< -o $@

clean:
	rm -rf ./src/*.o $(EXECUTABLE)
