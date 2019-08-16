#include <iostream>
#include <queue>
#include <vector>
using namespace std;

/* �����еĿ��÷��� */
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

/* ���һ������ */
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

/* ����߽籣�� */
bool array_edge_protector(int plen, int len, int x, int y)
{
	if (x < 0 || y < 0)
		return false;
	else if (plen <= x || len <= y)
		return false;
	else
		return true;
}

/* ·����ѭ������������ȫ�ֱ��� tred������� DFS ���ʱ�� */ 
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
	
	/* �������߶��У�ÿ��ѭ�������Զ��׵ľ��ߣ�Ȼ��ʹ����� */
	queue<int> ql;
	/* ���ڷ�����ߵ���ѭ��������ͬʱ���Լ��پ��߳��ԵĴ��� */
	if (lastsec != DOWN) ql.push(UP);
	if (lastsec != UP) ql.push(DOWN);
	if (lastsec != RIGHT) ql.push(LEFT);
	if (lastsec != LEFT) ql.push(RIGHT);
	
	while (!ql.empty())
	{
		/* ���ն���ִ�о��ߣ�����������case����ִ�� */
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
		
		/* ����߽籣�������� array_edge_protector ��ʾ��Խ���Թ� */
		if (!array_edge_protector(plen, len, ax, ay))
		{
			/* �ع����߲����¿�ʼѭ�� */
  			ql.pop();
			ax = lastax;
			ay = lastay;
			continue;
		}

		/* DFS ��ǲ��ԣ�·����ѭ�������� */
		if (deadloop_protector(ax, ay))
			return false;
		
		/* ��ǰ�����Ƿ���ȷ */
		if (data[ax][ay] == PASS)
		{
			/* �����Ե�ǰλ������ */
			dump_selection(ql.front());
			if (resolver(data, plen, len, ax, ay, bx, by, ql.front()))
				return true;
		}
		else
		{
			/* ���ߴ��󣬻ع� */
			ax = lastax;
			ay = lastay;
		}
		
		ql.pop();
		lastax = ax;
		lastay = ay;
		add.x = ax;
		add.y = ay;
		/* ��Ǳ�����Ѱ����λ�� */
		tred.push_back(add);
	}
	
	/* ���п��ܵľ�����ö���꣬����û�гɹ��ĳ��� */ 
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

