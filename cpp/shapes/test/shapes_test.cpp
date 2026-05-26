#include <iostream>
#include "shape.hpp"
#include "circle.hpp"
#include "line.hpp"
#include "square.hpp"

#include "point.hpp"
#include "glut_utils.h"
#include "point.hpp"
#include <unistd.h>

using namespace hrd42;
using namespace std;
using namespace ilrd;

enum
{
    SCREEN_WIDTH = 1000,
    SCREEN_HEIGHT = 1000,
    COLORS_NUMBER = 8
};

COLORS colors_fsm[] = 
{
COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_YELLOW,
 COLOR_MAGENTA, COLOR_CYAN, COLOR_WHITE,
 COLOR_FILL
};

static void DrawFunction();
static int TimerFunction();

Point circle1_center(300, 500);
Point circle2_center(300,300);
Point line1_center(300, 500);
Point square1_center(400,500);

int current_color_idx = 0;

Circle c1(circle1_center, 120, 30, COLOR_WHITE);
Circle c2(circle2_center);

Line l1(line1_center, 200, 70, COLOR_WHITE);

Square s1(square1_center, 200, 60,COLOR_WHITE );
int main(int argc, char** argv)
{
    DrawInit(argc, argv, SCREEN_WIDTH, SCREEN_HEIGHT, DrawFunction);
    //user modifies color
    //user modifies placement (move)
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
    c1.Draw();
    c2.Draw();

    l1.Draw();

    s1.Draw();
}

//called each 100 msec
static int TimerFunction()
{
    //user modify color
    current_color_idx = (current_color_idx + 1 ) % COLORS_NUMBER;

   c1.SetColor(colors_fsm[current_color_idx]);
    l1.SetColor(colors_fsm[current_color_idx]);
    s1.SetColor(colors_fsm[current_color_idx]);
    c2.SetColor(colors_fsm[current_color_idx]);

    c1.Move(1,1);

   // s1.Rotate(20);
    s1.Move(1,1);
    s1.Revolve(Point(500,400), 50);

    //l1.Move(30, 30);
    l1.Rotate(20);
    c2.Revolve(Point(400,400), 20);

    //s1.Move(10,20);



    return 1;  
}
