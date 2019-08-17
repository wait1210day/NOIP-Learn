#include <iostream>
using namespace std;

int main(int argc, char const **argv)
{
	int n, k, res = 0;
	cin >> n;
	int coins[n];
	
	for (int i = 0; i < n; i++)
		cin >> coins[i];
	cin >> k;
	
	/* Get the value of paying */
	int need = coins[n-1] - k;
	int count;
	
	for (int i = n-1; i >= 0; i--)
	{
		count = need / coins[i];
		need -= coins[i] * count;
		res += count;
	}
	
	cout << res << endl;
	return 0;
}
