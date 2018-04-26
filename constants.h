#include "include/Angel.h"
#include <string>
#include <iostream>

using namespace std;

// init
const string GG_str = "GAME OVER";

const vec4 ColorsVec[7] = {
    vec4(0.86, 0.39, 0.35, 1.0), // red
    vec4(1.00, 0.59, 0.24, 1.0), // orange
    vec4(0.37, 0.51, 1.00, 1.0), // blue
    vec4(0.85, 0.81, 0.05, 1.0), // yellow
    vec4(0.69, 0.41, 0.81, 1.0), // purple
    vec4(0.45, 0.71, 0.41, 1.0), // green
    vec4(0.99, 0.55, 0.65, 1.0) // pink
};

const bool shapes[28][4] = {
	{0,0,0,0},
	{0,1,1,0},
	{0,1,1,0},
	{0,0,0,0},

	{0,0,0,0},
	{1,1,1,1},
	{0,0,0,0},
	{0,0,0,0},

	{0,0,0,0},
	{0,0,1,1},
	{0,1,1,0},
	{0,0,0,0},

	{0,0,0,0},
	{0,1,1,0},
	{0,0,1,1},
	{0,0,0,0},

	{0,0,0,0},
	{0,1,1,1},
	{0,1,0,0},
	{0,0,0,0},

	{0,0,0,0},
	{0,1,1,1},
	{0,0,0,1},
	{0,0,0,0},

	{0,0,0,0},
	{0,1,1,1},
	{0,0,1,0},
	{0,0,0,0},
};

const int NumHoriBlocks = 20;
const int NumHoriLines = NumHoriBlocks + 1;
const int NumVertBlocks = 10;
const int NumVertLines = NumVertBlocks + 1;
const int NumHoriPoints = NumHoriLines * 2;
const int NumVertPoints = NumVertLines * 2;

const double wide = 0.9;
const double height = 0.9;
const double BlockWide = wide/5;
const double BlockHeight = height/10;

const int BeginTetroPoints = NumHoriPoints + NumVertPoints;
const int NumOfTetroPoints = 16;

const int BeginBoardPoints = BeginTetroPoints + NumOfTetroPoints;
const int MaxBoardPoints = 800;

const int TotalPoints = NumHoriPoints + NumVertPoints + NumOfTetroPoints + MaxBoardPoints;

const int ColorsOffset = TotalPoints * sizeof(vec2);

// keys
const int ESC_33 = 33;
const int ESC_27 = 27;

// time interval
const double DefInterval = 500;
const double IntervalSpeedUp = 1.064;
const double MiniInterval = 350;
