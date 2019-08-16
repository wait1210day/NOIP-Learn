#include <iostream>
#include <queue>
#include <vector>
using namespace std;

/* 队列中的可用方向 */
#define UP		1
#define DOWN	2
#define LEFT	3
#define RIGHT	4

#define PASS	0
#define WALL	1

struct Addr
{
	int x;
	int y;
};
vector<Addr> tred;

/* 输出一个决策 */
void dump_selection(int selection)
{
	if (tred.empty())
		cout << "Backtrace: ";
	else
		cout << " -> ";
	
	switch (selection)
	{
		case UP:
			cout << "UP";
			break;
		case DOWN:
			cout << "DOWN ";
			break;
		case LEFT:
			cout << "LEFT ";
			break;
		case RIGHT:
			cout << "RIGHT ";
			break;
	}
}

/* 数组边界保护 */
bool array_edge_protector(int plen, int len, int x, int y)
{
	if (x < 0 || y < 0)
		return false;
	else if (plen <= x || len <= y)
		return false;
	else
		return true;
}

/* 路径死循环保护（依赖全局变量 tred），或称 DFS 访问标记 */ 
bool deadloop_protector(int x, int y)
{
	Addr add;
	for (int i = 0; i < tred.size(); i++)
	{
		add = tred[i];
		if (add.x == x && add.y == y)
		{
			return true;
		}
	}
	return false;
}

bool resolver(int *data[], int plen, int len, int ax, int ay, int bx, int by, int lastsec)
{
	if (ax == bx && ay == by) return true;
	int lastax = ax, lastay = ay;
	
	Addr add;
	
	/* 创建决策队列，每次循环都尝试队首的决策，然后使其出队 */
	queue<int> ql;
	/* 基于方向决策的死循环保护，同时可以减少决策尝试的次数 */
	if (lastsec != DOWN) ql.push(UP);
	if (lastsec != UP) ql.push(DOWN);
	if (lastsec != RIGHT) ql.push(LEFT);
	if (lastsec != LEFT) ql.push(RIGHT);
	
	while (!ql.empty())
	{
		/* 按照队列执行决策，并不是所有case都会执行 */
		switch (ql.front())
		{
			case UP:
				if (array_edge_protector(plen, len, ax, ay)) ax--;
				break;
			case DOWN:
				if (array_edge_protector(plen, len, ax, ay)) ax++;
				break;
			case LEFT:
				if (array_edge_protector(plen, len, ax, ay)) ay--;
				break;
			case RIGHT:
				if (array_edge_protector(plen, len, ax, ay)) ay++;
				break;
		}
		
		/* 数组边界保护，触发 array_edge_protector 表示已越出迷宫 */
		if (!array_edge_protector(plen, len, ax, ay))
		{
			/* 回滚决策并重新开始循环 */
  			ql.pop();
			ax = lastax;
			ay = lastay;
			continue;
		}

		/* DFS 标记测试（路径死循环保护） */
		if (deadloop_protector(ax, ay))
			return false;
		
		/* 当前决策是否正确 */
		if (data[ax][ay] == PASS)
		{
			/* 继续以当前位置搜索 */
			dump_selection(ql.front());
			if (resolver(data, plen, len, ax, ay, bx, by, ql.front()))
				return true;
		}
		else
		{
			/* 决策错误，回滚 */
			ax = lastax;
			ay = lastay;
		}
		
		ql.pop();
		lastax = ax;
		lastay = ay;
		add.x = ax;
		add.y = ay;
		/* 标记本次搜寻到的位置 */
		tred.push_back(add);
	}
	
	/* 所有可能的决策已枚举完，但是没有成功的尝试 */ 
	return false;
}

int main()
{
	int n, k, startx, starty, tgx, tgy;
	char in;
	
	int **data;
	
	cin >> k;
	for (int i = 0; i < k; i++)
	{
		cin >> n;
		data = new int* [n];
		for (int a = 0; a < n; a++)
		{
			data[a] = new int[n];
			for (int b = 0; b < n; b++)
			{
				cin >> in;
				if (in == '.') data[a][b] = PASS;
				else data[a][b] = WALL;
			}
		}
		
		cin >> startx >> starty >> tgx >> tgy;
	
		if (resolver(data, n, n, startx, starty, tgx, tgy, -1))
		{
			cout << "YES" << endl;
		}
		else
		{
			cout << "NO" << endl;
		}
		tred.clear();
	}
	return 0;
}

