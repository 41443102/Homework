#include <iostream>
#include <cmath>
using namespace std;
class Polynomial;  
class Term 
{
    friend class Polynomial;
    friend ostream& operator<<(ostream& out, const Polynomial& p);
private:          // 私有
    float coef;   //係數
    int exp;      //指數
};
class Polynomial 
{
    friend istream& operator>>(istream& in, Polynomial& p);
    friend ostream& operator<<(ostream& out, const Polynomial& p);
public:
    Polynomial();
    Polynomial(const Polynomial& b);
    ~Polynomial();
    Polynomial& operator=(const Polynomial& b);
    Polynomial Add(Polynomial q);
    Polynomial Mult(Polynomial q);
    float Eval(float x);
    void NewTerm(float c, int e);
private:               // 私有
    Term* termArray;   // 放所有項的陣列
    int capacity;      // 陣列最多放幾項
    int terms;         // 現在有幾項
};
Polynomial::Polynomial()  // 建構子
{
    capacity = 2;
    terms = 0;
    termArray = new Term[capacity];
}
Polynomial::~Polynomial() //解構子
{
    delete[] termArray;
}
Polynomial::Polynomial(const Polynomial& b) //複製建構子
{
    capacity = b.capacity;
    terms = b.terms;
    termArray = new Term[capacity];
    for (int i = 0; i < terms; i++)
        termArray[i] = b.termArray[i];
}
Polynomial& Polynomial::operator=(const Polynomial& b) 
{
    if (this == &b) return *this;
    delete[] termArray;
    capacity = b.capacity;
    terms = b.terms;
    termArray = new Term[capacity];
    for (int i = 0; i < terms; i++)
        termArray[i] = b.termArray[i];
    return *this;
}
void Polynomial::NewTerm(float c, int e) 
{
    if (c == 0) return;  //係數是0的不存         

    for (int i = 0; i < terms; i++)  //找有沒有同指數的項
    {  
        if (termArray[i].exp == e)
        {
            termArray[i].coef += c; //係數加起來    
            if (termArray[i].coef == 0)
            {  
                for (int j = i; j < terms - 1; j++)
                    termArray[j] = termArray[j + 1];
                terms--;
            }
            return;
        }
    }
    if (terms == capacity) 
    {            
        capacity = capacity * 2;
        Term* big = new Term[capacity];
        for (int i = 0; i < terms; i++)
            big[i] = termArray[i];
        delete[] termArray;
        termArray = big;
    }
    termArray[terms].coef = c;          
    termArray[terms].exp = e;
    terms++;
}
Polynomial Polynomial::Add(Polynomial q)
{
    Polynomial ans = *this;            
    for (int i = 0; i < q.terms; i++)
        ans.NewTerm(q.termArray[i].coef, q.termArray[i].exp);
    return ans;
}
Polynomial Polynomial::Mult(Polynomial q) 
{
    Polynomial ans;
    for (int i = 0; i < terms; i++) 
    {
        for (int j = 0; j < q.terms; j++) 
        {
            float c = termArray[i].coef * q.termArray[j].coef;   //係數相乘
            int e = termArray[i].exp + q.termArray[j].exp;       //指數相加
            ans.NewTerm(c, e);
        }
    }
    return ans;
}
float Polynomial::Eval(float x) 
{
    float s = 0;  //s是總和
    for (int i = 0; i < terms; i++)
        s += termArray[i].coef * pow(x, termArray[i].exp);
    return s;
}
istream& operator>>(istream& in, Polynomial& p) 
{
    int n;  //輸入幾項
    in >> n;
    p.terms = 0;  //先把p清空                  
    for (int i = 0; i < n; i++) 
    {
        float c;  //係數
        int e;    //指數
        in >> c >> e;
        p.NewTerm(c, e);
    }
    return in;
}
ostream& operator<<(ostream& out, const Polynomial& p) 
{
    if (p.terms == 0)
    {
        out << "0";
        return out;
    }
    for (int i = 0; i < p.terms; i++) 
    {
        if (i > 0) out << " + ";
        out << p.termArray[i].coef << "x^" << p.termArray[i].exp;
    }
    return out;
}
int main() {
    Polynomial a, b;    
    cout << "請輸入第一個多項式：" << endl;
    cin >> a;
    cout << "請輸入第二個多項式：" << endl;
    cin >> b;
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    cout << "a + b = " << a.Add(b) << endl;
    cout << "a * b = " << a.Mult(b) << endl;
    cout << "a(2) = " << a.Eval(2) << endl;
    return 0;
}