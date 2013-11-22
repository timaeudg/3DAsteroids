#ifndef __MAZE
#define __MAZE

#include <stdlib.h>
#include <vector>

#define BOTTOM_BLOCK 0x01
#define TOP_BLOCK 0x02
#define LEFT_BLOCK 0x04
#define RIGHT_BLOCK 0x08

#define NEW_CELL 0xff
#define UNVISITED_CELL 0x10

class Cell
{
public:
	Cell()
	{ c=NEW_CELL; }
	
	bool bottomBlocked() const
	{ return (c&BOTTOM_BLOCK)==BOTTOM_BLOCK; }
	bool topBlocked() const
	{ return (c&TOP_BLOCK)==TOP_BLOCK; }
	bool leftBlocked() const
	{ return (c&LEFT_BLOCK)==LEFT_BLOCK; }
	bool rightBlocked() const
	{ return (c&RIGHT_BLOCK)==RIGHT_BLOCK; }
	
	void clearBottom()
	{ c = c&~BOTTOM_BLOCK; }
	void clearTop()
	{ c = c&~TOP_BLOCK; }
	void clearLeft()
	{ c = c&~LEFT_BLOCK; }
	void clearRight()
	{ c = c&~RIGHT_BLOCK; }
	
	void visit()
	{ c = c&~UNVISITED_CELL; }
	
	bool unvisited() const
	{ return (c&UNVISITED_CELL)==UNVISITED_CELL; }
	
private:
	unsigned char c;
};

class Maze
{
public:
	Maze()
	{
		this->width = 2;
		this->height = 2;
		clearMaze();
		buildMazeStack(0,0);
		makeEntraceExit();
	}
	
	Maze(unsigned int const & width, unsigned int const & height, unsigned int const & seed = 1)
	{
		srand(seed);
		this->width = width;
		this->height = height;
		clearMaze();
		buildMazeStack(0,0);
		makeEntraceExit();
	}
	
	unsigned int getWidth() const
	{ return width; }
	unsigned int getHeight() const
	{ return height; }
	
	bool bottomBlocked(unsigned int const & x, unsigned int const & y) const
	{
		if( x >= width || x < 0 || y >= height || y < 0) return false;
		return getCell(x,y).bottomBlocked(); }
	bool topBlocked(unsigned int const & x, unsigned int const & y) const
	{
		if( x >= width || x < 0 || y >= height || y < 0) return false;
		return getCell(x,y).topBlocked(); 
	}
	bool leftBlocked(unsigned int const & x, unsigned int const & y) const
	{
		if( x >= width || x < 0 || y >= height || y < 0) return false;
		return getCell(x,y).leftBlocked(); 
	}
	bool rightBlocked(unsigned int const & x, unsigned int const & y) const
	{
		if( x >= width || x < 0 || y >= height || y < 0) return false;
		return getCell(x,y).rightBlocked(); 
	}
	
private:
	unsigned int width;
	unsigned int height;
	std::vector<Cell> cells;
	
	void clearMaze()
	{
		cells.resize(width*height);
		for(size_t i=0; i<cells.size(); i++)
			cells[i] = Cell();
	}
	
	bool hasUnvisitedNeighbors(unsigned int const & x, unsigned int const & y) const
	{	
		if( y!=0 && getCell(x, y-1).unvisited() )
			return true;
		
		if( (y+1)!=height && getCell(x, y+1).unvisited() )
			return true;
		
		if( x!=0 && getCell(x-1, y).unvisited() )
			return true;
		
		if( (x+1)!=width && getCell(x+1, y).unvisited() )
			return true;
		
		return false;
	}
	
	void makeEntraceExit()
	{
		size_t i;
		i = rand()%height;
		getCell(0,i).clearLeft();
		i = rand()%height;
		getCell(width-1,i).clearRight();
	}
	
	void buildMazeStack(unsigned int const & startX, unsigned int const & startY)
	{
		//not as pretty as recusion, but only grows mem usage by 2 ints each cell
		//so, fast and efficient
		std::vector<unsigned int> stack;
		
		unsigned int x = startX;
		unsigned int y = startY;
		unsigned int nextSpot = 5;
		
		//basic idea:
		
		//set current x,y
		//while true
			//mark visit
			//if has unvisited
				//pick next target
				//push x,y
				//set current to target
				//continue (start processing target)
			//else
				//if more stack
					//pop stack
					//set current to popped
					//continue loop (implied)
				//else
					//return (all done)
		
		while(true)
		{
			getCell(x,y).visit();
			
			if(hasUnvisitedNeighbors(x,y))
			{
				nextSpot = pickUnvisitedDirection(x,y);
				switch(nextSpot)
				{
						/* bottom */
					case 0:
						getCell(x,y).clearBottom();
						getCell(x,y-1).clearTop();
						stack.push_back(x); stack.push_back(y);
						x=x; y=y-1;
						continue;
						/* top */
					case 1:
						getCell(x,y).clearTop();
						getCell(x,y+1).clearBottom();
						stack.push_back(x); stack.push_back(y);
						x=x; y=y+1;
						break;
						/* left */
					case 2:
						getCell(x,y).clearLeft();
						getCell(x-1,y).clearRight();
						stack.push_back(x); stack.push_back(y);
						x=x-1; y=y;
						break; 
						/* right */
					case 3:
						getCell(x,y).clearRight();
						getCell(x+1,y).clearLeft();
						stack.push_back(x); stack.push_back(y);
						x=x+1; y=y;
						break;
				}
			}
			else
			{
				if(stack.size() > 0)
				{
					y=stack.back();
					stack.pop_back();
					x=stack.back();
					stack.pop_back();
				}
				else
					return;
			}
		}
	}
	
	void buildMazeRecursive(unsigned int const & x, unsigned int const & y)
	{
		//recursion is elegant, but requires a stack frame each recusion
		//slow and stack grows too fast to handle huge mazes
		
		getCell(x,y).visit();
		
		while(hasUnvisitedNeighbors(x,y))
		{
			unsigned int nextSpot = 5;
			nextSpot = pickUnvisitedDirection(x,y);
			switch(nextSpot)
			{
					/* bottom */
				case 0:
					getCell(x,y).clearBottom();
					getCell(x,y-1).clearTop();
					buildMazeRecursive(x, y-1);
					break;
					/* top */
				case 1:
					getCell(x,y).clearTop();
					getCell(x,y+1).clearBottom();
					buildMazeRecursive(x, y+1);
					break;
					/* left */
				case 2:
					getCell(x,y).clearLeft();
					getCell(x-1,y).clearRight();
					buildMazeRecursive(x-1, y);
					break; 
					/* right */
				case 3:
					getCell(x,y).clearRight();
					getCell(x+1,y).clearLeft();
					buildMazeRecursive(x+1, y);
					break;
			}
		}
	}
	
	int pickUnvisitedDirection(unsigned int const & x, unsigned int const & y) const
	{
		bool spots[4]={false,false,false,false};
		int i;
		Cell c = getCell(x,y);
		
		if( y!=0 && getCell(x, y-1).unvisited() )
			spots[0]=true;
		
		if( (y+1)!=height && getCell(x, y+1).unvisited() )
			spots[1]=true;
		
		if( x!=0 && getCell(x-1, y).unvisited() )
			spots[2]=true;
		
		if( (x+1)!=width && getCell(x+1, y).unvisited() )
			spots[3]=true;
		
		/* if no places, exit */
		if(spots[0]==0 && spots[1]==0 && spots[2]==0 && spots[3]==0)
			return -1;
		
		/* pick a spot to move to */
		i = rand()%4;
		while( spots[i]==false )
			i = rand()%4;
		
		return i;
	}
	
	unsigned int getId(unsigned int const & x, unsigned int const & y) const
	{ return y*width + x; }
	
	Cell & getCell(unsigned int const & x, unsigned int const & y)
	{ return cells.at( getId(x, y) ); }
	
	Cell const & getCell(unsigned int const & x, unsigned int const & y) const
	{ return cells.at( getId(x, y) ); }
};

#endif
