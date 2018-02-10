SOURCE  := $(wildcard *.cpp)
OBJS    := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))

TARGET  := myserver
CC      := g++
LIBS    := -lpthread -lopencv_core -lopencv_imgproc -lopencv_highgui
INCLUDE:= -I./usr/local/include/opencv
CFLAGS  := -std=c++11 -g -Wall -O3 $(INCLUDE)
CXXFLAGS:= $(CFLAGS)

.PHONY : objs clean veryclean rebuild all
all : $(TARGET)
objs : $(OBJS)
rebuild: veryclean all
clean :
	rm -fr *.o
veryclean : clean
	rm -rf $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)