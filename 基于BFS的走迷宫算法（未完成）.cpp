#include <iostream>
#include <algorithm>
using namespace std;

#define LEFT	1
#define RIGHT	2
#define UP		3
#define DOWN	4

struct position
{
	short x;
	short y;
};

int decisions[4] = {LEFT, RIGHT, UP, DOWN};
int width, height;
position target;
char **map;

// 边界保护
bool overside(position& thispos)
{
	if (thispos.x >= width || thispos.y >= height)
		return true;
	else if (thispos.x < 0 || thispos.y < 0)
		return true;
		
	return false;
}

void printDecision(int p)
{
	switch (p)
	{
		case LEFT:
			cout << "-> LEFT ";
			break;
		case RIGHT:
			cout << "-> RIGHT ";
			break;
		case UP:
			cout << "-> UP ";
			break;
		case DOWN:
			cout << "-> DOWN ";
			break;
	}
}

bool start(position pos, int lastDecision)
{
	if (pos.x == target.x && pos.y == target.y)
		return true;
	if (map[pos.y][pos.x] == '#')
		return false;
		
	position lastpos;
	
	for (int i = 0; i < 4; i++)
	{
		lastpos = pos;
		
		if (decisions[i] == lastDecision)
			continue;
		switch (decisions[i])
		{
			case LEFT:
				pos.x--;
				break;
			case RIGHT:
				pos.x++;
				break;
			case UP:
				pos.y--;
				break;
			case DOWN:
				pos.y++;
				break;
		}
		
		if (overside(pos))
		{
			// 边界保护触发，坐标回滚
			pos = lastpos;
			continue;
		}
		
		printDecision(decisions[i]);
		
		if (start(pos, decisions[i]))
			return true;
	}
	
	return false;
}

int main()
{
	cin >> height >> width;
		
	position initaddr;
	initaddr.x = 0;
	initaddr.y = 0;
	target.x = width - 1;
	target.y = height - 1;
	
	map = new char* [height];
	for (int i = 0; i < height; i++)
	{
		map[i] = new char[width];
		for (int j = 0; j < width; j++)
			cin >> map[i][j];
	}
	
	start(initaddr, -1);
	return 0;
}
