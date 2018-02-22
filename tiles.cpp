#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>
#include "tiles.h"
#include "constants.h"

using namespace std;

extern bool gameOver;

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

tiles::tiles()
{
    TileShapeString = "OISZLJT";
    color_id = randomNumGen();
}

int tiles::randomNumGen()
{
    srand((unsigned)time(0)); // true random number / random color and shape
    int result = rand()%(TileShapeString.length()-1);
    return result;
}

void tiles::reset()
{
	boundCheck = 3;
	rotationCount = 0;
	extraStep = -2;
    
    shape_id = randomNumGen();
	shape = TileShapeString[shape_id];
    color_id = (color_id+1)%7;
    color = ColorsVec[color_id];
    cout << "color id: " << color_id << " color: " << color << " shape id: " << shape_id << " shape: " << shape << endl;

	for(int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			blocks[i][j] = shapes[shape_id*4+i][j];

	gettimeofday(&startTime, NULL);
	if(interval > MiniInterval)
		interval /= IntervalSpeedUp;
}

inline double tiles::timeElapsed() const
{
	timeval T;
	gettimeofday(&T, NULL);
	double elapT;
	elapT = (T.tv_sec - startTime.tv_sec)*1000.0; // calculate elapsed time sec -> ms us -> ms
	elapT += (T.tv_usec - startTime.tv_usec)/1000.0;
	return elapT;
}

int tiles::steps()
{
	return ceil(timeElapsed()/interval+extraStep);
}

void tiles::addBlock()
{
	int rollBack = 1;
	while(board_t->collision(blocks, steps()-rollBack, boundCheck))
		rollBack += 1;
	board_t->add(blocks, steps()-rollBack, boundCheck, color_id);
	reset();
}

void tiles::left()
{
	if(!board_t->collision(blocks, steps(), boundCheck-1))
        boundCheck -= 1;
}

void tiles::right()
{
	if(!board_t->collision(blocks, steps(), boundCheck+1))
        boundCheck += 1;
}

void tiles::down()
{
	extraStep += 1;
	if(board_t->collision(blocks, steps(), boundCheck))
        addBlock();
}

void tiles::rotate()
{
	rotationCount += 1;
	if(shape == 'I' || shape == 'Z' || shape == 'S')
		if(rotationCount%2 == 0)
			rotateBackward();
		else
			rotateForward();
	else if(shape == 'J' || shape == 'L' || shape == 'T')
		rotateForward();
}

void tiles::rotateForward()
{
    bool newBlock[4][4] = {{0}};
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            if(blocks[i][j])
            {
                int newI = 1-(j-2);
                int newJ = 2+(i-1);
                if(0 <= newI && newJ < 4 && 0 <= newJ && newI < 4)
                    newBlock[newI][newJ] = 1;
                
                assert(newI >= 0);
                assert(newI <  4);
                assert(newJ >= 0);
                assert(newJ <  4);
            }
    
    if(!board_t -> collision(newBlock, steps(), boundCheck))
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                blocks[i][j] = newBlock[i][j];
    else
        rotationCount -= 1;
}

void tiles::rotateBackward()
{
    bool newBlock[4][4] = {{0}};
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            newBlock[i][j] = shapes[shape_id*4+i][j];
            assert(shape_id*4+i < 28);
        }
    
    if(!board_t -> collision(newBlock, steps(), boundCheck))
        memcpy(blocks, newBlock, 4*4);
    else
        rotationCount -= 1;
}

void tiles::writeBuffer()
{
	int steps = this->steps();
    
    if (board_t->collision(blocks, steps, boundCheck))
    {
        if (board_t->reachedTop(blocks, steps))
        {
            addBlock();
            gameOver = true;
            return;
        }
        else
        {
            addBlock();
            return;
        }
    }
    
    // write buffer for each block
    int current = 0;
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            if(blocks[i][j])
            {
                vec2 points[4];
                points[0] = vec2(-wide + (j+boundCheck  )*BlockWide, height - (i+1)*BlockHeight - steps*BlockHeight);
                points[1] = vec2(-wide + (j+boundCheck+1)*BlockWide, height - (i+1)*BlockHeight - steps*BlockHeight);
                points[2] = vec2(-wide + (j+boundCheck  )*BlockWide, height - i*BlockHeight - steps*BlockHeight);
                points[3] = vec2(-wide + (j+boundCheck+1)*BlockWide, height - i*BlockHeight - steps*BlockHeight);
                glBufferSubData(GL_ARRAY_BUFFER, (BeginTetroPoints + 4 * current) * sizeof(vec2), sizeof(points), points); // block

                vec4 color = ColorsVec[color_id];
                vec4 colors[4] = {color, color, color, color};
                glBufferSubData(GL_ARRAY_BUFFER, ColorsOffset+(BeginTetroPoints+4*current)*sizeof(vec4), sizeof(colors), colors); // color

                current += 1;
            }
}
