#include<iostream>
#include<string>
#include<algorithm>
using namespace std;
void powerset(string a, int index ,string current) 
{
	if (index == a.length())
	{
		cout << current << endl;
		return;
	}
	powerset(a, index + 1, current);;
	powerset(a, index + 1, current + a[index]);
}
int main()
{
	string n; 
	cin >> n; 
	powerset(n, 0, "");
}
