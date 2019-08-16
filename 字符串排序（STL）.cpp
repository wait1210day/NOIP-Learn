#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
	vector<std::string> str;
	string in;
	
	while (cin >> in)
		str.push_back(in);
	
	sort(str.begin(), str.end());
	
	for (int i = 0; i < str.size(); i++)
		cout << str[i] << endl;
	return 0;
}
