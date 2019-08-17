#include <iostream>
#include <algorithm>
#include <queue>
using namespace std;

struct Person
{
	int number;
	int time;
};

static bool cmpfunc(Person ps1, Person ps2)
{
	if (ps1.time >= ps2.time)
		return true;
	else
		return false;
}

int main()
{
	int n, val;
	float avg = 0;
	cin >> n;
	Person ps[n];
	
	for (int i = 0; i < n; i++)
	{
		cin >> ps[i].time;
		ps[i].number = i + 1;
	}
	
	sort(ps, ps + n, cmpfunc);
	
	for (int i = n-1; i >= 0; i--)
		cout << ps[i].number << " ";
	cout << endl;
	
	/* calculating Tavg */
	for (int i = 0; i < n; i++)
		avg += i * ps[i].time;
	avg /= (float) n;
	
	cout.setf(ios_base::fixed);
	cout.precision(2);
	cout << avg << endl;
	return 0;
}
