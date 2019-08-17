#include <iostream>
using namespace std;

int binary(int num)
{
	int count = 0;
	while (num != 0)
	{
		if (num % 2 == 1)
			count++;
		num /= 2;
	}
	
	return count;
}


int main()
{
	int val, i;
	while (1)
	{
		cin >> val;
		if (val == 0) break;
		
		for (i = val+1; binary(i) != binary(val); i++)
			;
		cout << i << endl;
	}
}

