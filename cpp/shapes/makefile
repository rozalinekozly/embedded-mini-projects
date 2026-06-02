# SHAPES MAKEFILE
# RELEASE VERSION, MAY ADD FLAG -g TO CXX FOR DEBUG VERSION (OR JUST ADD ANOTHER VARIABLE OR WHATEVER)
CXX      = g++ 
CXXFLAGS = -I include
LDFLAGS  = -L ./lib -Wl,-rpath,./lib

LIBS     = -lpoint -lglut_utils -lglut -lGL -lGLU

TARGET   = res

CPPS = src/shape.cpp src/circle.cpp src/line.cpp src/square.cpp src/rectangle.cpp src/group.cpp

all: shapes_test

demo: src/demo.c
	$(CXX) src/demo.c $(CXXFLAGS) $(LDFLAGS) $(LIBS) -o $(TARGET)

shapes_test: test/shapes_test.cpp
			@echo "---- SHAPES TEST ----"
			$(CXX) test/shapes_test.cpp $(CPPS) $(CXXFLAGS) $(LDFLAGS) $(LIBS) -o $(TARGET)
			@echo "---- BUILD SUCCESSFULLY ----"
			@echo "---- RUNING res ----"
			./res
clean:
	rm -f $(TARGET)

.PHONY: all demo clean
