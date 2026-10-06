#include <iostream>
using namespace std;

int A(int m, int n)
{
    if (m == 0) 
    {
        return n + 1; //安m单0肚n+1
    }
    else if (m == 1)
    {
        return n + 2; //安m单1肚n+2
    }
    else if (m == 2)
    {
        return 2 * n + 3; //安m单2肚2n+3
    }
    else if (m == 3)
    {
        int r = 1;

        for (int i = 0; i < n + 3; i++)
        {
            r = r * 2;
        }

        return r - 3;
    }

    return -1;
}

int main()
{
    int m, n;

    cin >> m >> n; //块ㄢ计

    cout << A(m, n) << endl; //㊣ㄧΑ A(m, n) 璸衡

    return 0;
}