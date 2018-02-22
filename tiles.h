#include "include/Angel.h"
#include "board.h"
#include <string>

using namespace std;

class tiles
{
	private:
		timeval startTime;
		vec4 color; // color
		int color_id;
		char shape; // shape
		int shape_id;
		int boundCheck; // shape outer bound check
		int extraStep; // step check
		int rotationCount; // count rota
		bool blocks[4][4]; // each block has four small squares

		inline double timeElapsed() const; // time recording
    
		int steps();
		void rotateForward();
		void rotateBackward();
		void addBlock();

	public:
		board* board_t;
		double interval;
        string TileShapeString;
    
		tiles();
        int randomNumGen();
		void left();
		void right();
		void down();
		void rotate();
		void reset();
		void writeBuffer();
};
