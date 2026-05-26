#include <iostream>
#include "shape.hpp"
#include "circle.hpp"
#include "line.hpp"
#include "square.hpp"
#include "group.hpp"

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
    COLORS_NUMBER = 7
};

COLORS colors_fsm[] = 
{
COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_YELLOW,
 COLOR_MAGENTA, COLOR_CYAN, COLOR_WHITE
};

static void DrawFunction();
static int TimerFunction();
static void InitTest();
static COLORS FillColor(COLORS color);

Point circle1_center(300, 500);
Point circle2_center(300,300);
Point circle3_center(300,200);
Point circle4_center(600, 500);

Point line1_center(300, 500);

Point square1_center(400,500);
Point square2_center(300,600);

int current_color_idx = 0;

Circle c1(circle1_center, 120, 30, FillColor(COLOR_WHITE));
Circle c2(circle2_center);

Line l1(line1_center, 200, 70, FillColor(COLOR_WHITE));

Square s1(square1_center, 200, 60, FillColor(COLOR_CYAN));


Circle c3(circle3_center, 250, 0, FillColor(COLOR_YELLOW));
Circle c4(circle4_center, 270, 0 , FillColor(COLOR_BLUE));
Square s2(square2_center, 100, 40, FillColor(COLOR_CYAN));


Group group1(c3);

int main(int argc, char** argv)
{
    DrawInit(argc, argv, SCREEN_WIDTH, SCREEN_HEIGHT, DrawFunction);
    //declare on elements
    InitTest();
    //user modifies color
    //user modifies placement (move)
    DrawSetTimerFunc(TimerFunction, 100);
    DrawMainLoop();

    return 0;
}

static COLORS FillColor(COLORS color)
{
    return(static_cast<COLORS>(color | COLOR_FILL));
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

    group1.Draw();

}

//called each 100 msec
static int TimerFunction()
{
    //user modify color
    current_color_idx = (current_color_idx + 1 ) % COLORS_NUMBER;

    c1.SetColor(FillColor(colors_fsm[current_color_idx]));
    l1.SetColor(FillColor(colors_fsm[current_color_idx]));
    s1.SetColor(FillColor(colors_fsm[current_color_idx]));
    c2.SetColor(FillColor(colors_fsm[current_color_idx]));

    c1.Move(1,1);

   s1.Rotate(20);
    s1.Move(1,1);
    s1.Revolve(Point(500,400), 50);

    l1.Move(30, 30);
    l1.Rotate(20);
    c2.Revolve(Point(400,400), 20);

    s1.Move(10,20);

    group1.Revolve(Point(SCREEN_WIDTH/2,SCREEN_HEIGHT/2),50);

    return 1;  
}

static void InitTest()
{
    group1.AddShape(&c4);
    group1.AddShape(&s2);
}