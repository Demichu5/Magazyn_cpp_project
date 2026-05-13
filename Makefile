TARGET = main
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wno-enum-compare
INCLUDES = -I./Raylib/include -I./Settings -I./Magazyn -I./GuiHandler
LDFLAGS = ./Raylib/lib/libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11

SRCS = main.cpp \
       GuiHandler/render.cpp \
       GuiHandler/login.cpp \
       GuiHandler/section.cpp \
       Magazyn/magazyn.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(SRCS) -o $(TARGET) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS)

clean:
	rm -f $(TARGET)
