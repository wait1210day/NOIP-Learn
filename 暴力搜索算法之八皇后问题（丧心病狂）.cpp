#include <iostream>
#include <cstdlib>
using namespace std;

bool side(int a1, int b1, int a2, int b2)
{	
	for (int i = a1, j = b1; i <= 8, j <= 8; i++, j++)
		if (i == a2 && j == b2)
			return false;
	for (int i = a1, j = b1; i >= 1, j >= 1; i--, j--)
		if (i == a2 && j == b2)
			return false;
	for (int i = a1, j = b1; i <= 8, j >= 1; i++, j--)
		if (i == a2 && j == b2)
			return false;
	for (int i = a1, j = b1; i >= 1, j <= 8; i--, j++)
		if (i == a2 && j == b2)
			return false;
	return true;
}

bool invalid(int *queens)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = i+1; j < 8; j++)
		{
			if (queens[i] == queens[j])
				return false;
			else if (!side(i+1, queens[i], j+1, queens[j]))
				return false;
		}
	}
	return true;
}

int emulator(int resolveNum)
{
	int a[8], res = 1;
	for (a[0] = 1; a[0] <= 8; a[0]++)
		for (a[1] = 1; a[1] <= 8; a[1]++)
			for (a[2] = 1; a[2] <= 8; a[2]++)
				for (a[3] = 1; a[3] <= 8; a[3]++)
					for (a[4] = 1; a[4] <= 8; a[4]++)
						for (a[5] = 1; a[5] <= 8; a[5]++)
							for (a[6] = 1; a[6] <= 8; a[6]++)
								for (a[7] = 1; a[7] <= 8; a[7]++)
									if (invalid(a))
									{
										if (res == resolveNum)
										{
											for (int i = 0; i < 8; i++)
												cout << a[i];
											cout << endl;
											return 0;
										}
										res++;
									}
	return 1;
}

int main(int argc, char** argv)
{
	int n, k;
	cin >> n;
	for (int i = 0; i != n; i++)
	{
		cin >> k;
		emulator(k);
	}
	return 0;
}
