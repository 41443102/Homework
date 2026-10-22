#include <iostream>   
#include <string>    
using namespace std;  
int main()
{
    string s;         
    cin >> s;         
    int n = s.length(); 
    for (int k = 0; k < (1 << n); k++) //1<<n是2的n次方
    {
        string x = "";   
        for (int i = 0; i < n; i++)
            if (k & (1 << i)) //1 << i左移
                
            {
                if (x != "")      
                    x += ",";     
                x += s[i];      
            }
        cout << "(" << x << ")" << endl;  
    }
}