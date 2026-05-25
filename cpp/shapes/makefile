CXX      = g++
CXXFLAGS = -I include
LDFLAGS  = -L ./lib -Wl,-rpath,./lib

LIBS     = -lpoint -lglut_utils -lglut -lGL -lGLU

TARGET   = res

all: demo

demo: src/demo.c
	$(CXX) src/demo.c $(CXXFLAGS) $(LDFLAGS) $(LIBS) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all demo clean
