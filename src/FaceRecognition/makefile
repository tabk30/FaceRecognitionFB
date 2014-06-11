CC = g++
CFLAGS 	= -c -Wall -O3  -I ./include/

#include Libs
#include OpenCV
CFLAGS 		+= $(shell pkg-config opencv --cflags)
LDFLAGS 	+= $(shell pkg-config opencv --libs)

SOURCES		= $(wildcard ./src/Main.cpp ./src/FaceRecogniontPCA.cpp ./src/detect/FaceDetect.cpp)
OBJECTS		= $(SOURCES:.cpp=.o)

EXECUTABLE = recognition

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -lopencv_core -lopencv_imgproc -lopencv_highgui $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(CURDIR)/$< -o $@

clean:
	rm -rf ./src/*.o $(EXECUTABLE)
