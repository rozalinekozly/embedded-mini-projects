#include <iostream>
#include "shape.hpp"
#include "circle.hpp"
#include "point.hpp"
#include "glut_utils.h"
#include "point.hpp"
#include <unistd.h>

using namespace hrd42;
using namespace std;
using namespace ilrd;

static void DrawFunction();
static int TimerFunction();


Point current_center(300, 500);
COLORS colors_fsm[] = 
{
COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_YELLOW,
 COLOR_MAGENTA, COLOR_CYAN, COLOR_WHITE,
 COLOR_FILL
};

int colors_number = 8;

int current_color_idx = 0;

int main(int argc, char** argv)
{
    DrawInit(argc, argv, 1000, 1000, DrawFunction);
    DrawSetTimerFunc(TimerFunction, 100);
    DrawMainLoop();

    return 0;
}

// method that declares on shapes, sets them
// and call draw 
//each x msec (100) main calls timer funciton 
//whcih resembels a user changes
static void DrawFunction()
{
    Circle c1(current_center, 120, 30, colors_fsm[current_color_idx]);
    c1.Draw();
}

static int TimerFunction()
{
    current_color_idx = (current_color_idx + 1 ) % colors_number;
    return 1;  
}
