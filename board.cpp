#include "board.h"
#include "constants.h"
#include "include/Angel.h"

board::board()
{
	reset();
}

void board::reset()
{
	for(int i = 0; i < 20; i++)
		for(int j = 0; j < 10; j++)
			blocks[i][j] = -1; // empty
	numPoints = 0;
}

bool board::collision(bool tileBlocks[4][4], int steps, int boundCheck)
{
	int left = 999;
	int right = -999;
	int bottom = -999;
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			if(tileBlocks[i][j])
			{
                if(j < left)
                    left = j;
                if(j > right)
                	right = j;
                if(i > bottom)
                	bottom = i;
            }
    // side
	if(boundCheck+left < 0)
        return true;
    if(boundCheck+right > 9)
    	return true;
    
    // bottom
    if(steps+bottom >= 20)
    	return true;
    
    // collision
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            if(steps+i>=0 && steps+i<20 && tileBlocks[i][j] && (blocks[steps+i][boundCheck+j]!=-1))
                return true;

    return false;
}
bool board::reachedTop(bool tileBlocks[4][4], int steps)
{
	int top = 999;
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            if(tileBlocks[i][j])
                if(i < top)
                    top = i;

    if(steps+top <= 0)
        return true;

    return false;
}

void board::add(bool tileBlocks[4][4], int steps, int boundCheck, int color_id)
{
	for(int y = 0; y < 4; y++)
        for(int x = 0; x < 4; x++)
            if(tileBlocks[y][x] && steps+y<20 && boundCheck+x<10 && steps+y>=0)
            {
                if(blocks[steps+y][boundCheck+x] == -1)
                    numPoints += 4;

                blocks[steps+y][boundCheck+x] = color_id;

                assert(0 <= color_id && color_id < 7);

                assert(steps+y >= 0);
                assert(steps+y < 20);
                assert(boundCheck+x >= 0);
                assert(boundCheck+x < 10);
            }

    // check full row
    for(int y = 0; y < 20; y++)
    {
        bool full = true;
        for(int x = 0; x < 10; x++)
            if(blocks[y][x] == -1)
            {
                full = false;
                break;
            }

        if(full)
        {
            for(int i = y; i > 0; i--)
                for(int x = 0; x < 10; x++)
                    blocks[i][x] = blocks[i-1][x];

            for(int i = 0; i < 10; ++i)
                blocks[0][i] = -1;
            numPoints -= 4*10;
        }
    }
}

void board::writeBuffer()
{
	int current = 0;
    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            if (blocks[i][j] != -1) {
                vec2 points[4];
                points[0] = vec2(-wide+(j    )*BlockWide, height-(i+1)*BlockHeight);
                points[1] = vec2(-wide+(j + 1)*BlockWide, height-(i+1)*BlockHeight);
                points[2] = vec2(-wide+(j    )*BlockWide, height-i*BlockHeight);
                points[3] = vec2(-wide+(j + 1)*BlockWide, height-i*BlockHeight);
                glBufferSubData(GL_ARRAY_BUFFER, (BeginBoardPoints+4*current)*sizeof(vec2), sizeof(points), points); // block

                vec4 color = ColorsVec[blocks[i][j]];
                vec4 colors[4] = {color, color, color, color};
                assert(0 <= blocks[i][j] && blocks[i][j] < 7);
                glBufferSubData(GL_ARRAY_BUFFER, ColorsOffset+(BeginBoardPoints+4*current)*sizeof(vec4), sizeof(colors), colors); // color

                current += 1;
            }
        }
    }
}
