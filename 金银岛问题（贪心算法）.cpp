#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

struct Object
{
	int total;
	int totalPrice;
	float price;
	bool operator> (const Object& obj) const { return this->price > obj.price; }
	bool operator< (const Object& obj) const { return this->price < obj.price; }
};

int main()
{
	int k, s;
	float res = 0;
	int poket = 0, poketMax;
	
	cin >> k;
	vector<Object> list;
	Object obj;
	
	cout.setf(std::ios_base::fixed);
	cout.precision(2);
	
	for (int i = 0; i < k; i++)
	{
		cin >> poketMax;
		cin >> s;
		for (int j = 0; j < s; j++)
		{
			cin >> obj.total >> obj.totalPrice;
			obj.price = (float)obj.totalPrice / (float)obj.total;
			list.push_back(obj);
		}
		
		sort(list.begin(), list.end(), greater<Object>());
		
		for (int j = 0; j < list.size() && poket < poketMax; j++)
		{
			if (poketMax - poket >= list[j].total)
			{
				poket += list[j].total;
				res += list[j].totalPrice;
			}
			else
			{
				res += (float)(poketMax - poket) * list[j].price;
				poket = poketMax;
			}
		}
		
		cout << res << endl;
		res = 0;
		poket = 0;
		list.clear();
	}
	return 0;
}
