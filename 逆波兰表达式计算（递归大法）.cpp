#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;

float expression()
{
	char str[10];
	float expr1, expr2, res;
	memset(str, '\0', 10);
	cin >> str;
	
	switch (str[0])
	{
		case '+':
			expr1 = expression();
			expr2 = expression();
			res = expr1 + expr2;
			break;
		case '-':
			expr1 = expression();
			expr2 = expression();
			res = expr1 - expr2;
			break;
		case '*':
			expr1 = expression();
			expr2 = expression();
			res = expr1 * expr2;
			break;
		case '/':
			expr1 = expression();
			expr2 = expression();
			res = expr1 / expr2;
			break;
		default:
			res = atof(str);
			break;
	}
	return res;
}

int main()
{
	cout.setf(std::ios_base::fixed);
	cout.precision(6);
	cout << expression() << endl;
	
	return 0;
}
