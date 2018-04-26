
class board
{
	private:
		int blocks[20][10];
	public:
		int numPoints;
		board();
		~board();
		void reset();
		bool collision(bool tileBlocks[4][4], int steps, int boundCheck);
		bool reachedTop(bool tileBlocks[4][4], int steps);
		void add(bool tileBlocks[4][4], int steps, int boundCheck, int color_id);
		void writeBuffer();
};
