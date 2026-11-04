# 41443102

作業二：多項式Polynomial

# 解題說明

這次要寫一個 `Polynomial`多項式類別，可以做出底下的兩個東西：

1. 照老師給的圖 1還有圖 2，做出多項式，然後能做 **加法 `Add`**、**乘法 `Mult`**、**代入求值 `Eval`**。
2. 寫輸入還有輸出的函式，要**多載 `>>` 和 `<<`**，這樣就能直接寫 `cin >> p;` 和 `cout << p;`。

## 我的想法（How to do?）

多項式 `3x² + 2x` 可以拆成兩個東西：`3x²` 和 `2x`。每個東西要記兩個數字：**係數**和**指數**。

- 一個叫 `Term`（一項），裡面放 `coef`（係數）和 `exp`（指數）。
- 整個多項式就是把很多個 `Term` 排成一排，放在陣列 `termArray` 裡。
- 另外用 `terms` 記現在有幾項，用 `capacity` 記陣列最多放幾項。
- 老師說只存**非零項**，所以係數是 0 的項不用存。

我有另外多用一個函式 `NewTerm(c, e)`，他會處理三件事情：

1. 係數是 0，就不去存。
2. 已經有同指數的項，就把係數加起來（`3x² + 2x² = 5x²`）。
3. 陣列滿了，就換一個兩倍大的陣列。

有這個的話，其他功能應該變得比較簡單：

- **加法**：先複製自己，再把對方的每一項用 `NewTerm` 丟進去。
- **乘法**：每項乘每項，結果也用 `NewTerm` 丟進去，同指數的會自動合併。
- **求值**：每一項算係數 × x 的指數次方，全部加起來。

## 舉例說明

假設 `a = 3x² + 2x`，`b = x + 5`。

**加法**：先把 `a` 抄過來變成 `3x² + 2x`，再丟 `1x¹` 進去，發現已經有 `2x¹`，所以變成 `3x¹`；再丟 `5x⁰`，沒有同指數，就新增。答案：`3x² + 3x + 5`。

**乘法**：

```text
3x² × x = 3x³
3x² × 5 = 15x²
2x  × x = 2x²      跟 15x² 同指數，合併成 17x²
2x  × 5 = 10x
答案：3x³ + 17x² + 10x
```

**求值**：把 `x = 2` 代進 `a`：`3×2² + 2×2 = 12 + 4 = 16`。

## 輸入格式

先輸入有幾項，再輸入每一項的係數 指數。例如 `3x² + 2x` 要輸入：

```text
2
3 2
2 1
```

# Algorithm Design & Programming

## 程式碼

```cpp
#include <iostream>
#include <cmath>
using namespace std;
class Polynomial;  
class Term 
{
    friend class Polynomial;
    friend ostream& operator<<(ostream& out, const Polynomial& p);
private:
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
private:
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
    if (c == 0) return;                 

    for (int i = 0; i < terms; i++) 
    {  
        if (termArray[i].exp == e)
        {
            termArray[i].coef += c;    
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
    int n;
    in >> n;
    p.terms = 0;                  
    for (int i = 0; i < n; i++) 
    {
        float c;
        int e;
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
```

## 一些重點

- **有 `friend`是因為** `Term` 的資料是私有的（`private`），要讓 `Polynomial` 和 `<<` 能讀，就要在 `Term` 裡寫 `friend`
- **有複製建構子和解構子、`operator=`是因為我們用 `new` 跟電腦借了記憶體。如果不寫，`Add` 回傳答案時，兩個多項式會共用同一塊記憶體，其中一個被還掉之後，另一個就壞了，程式會有錯。
- **`operator>>`、`operator<<` 最後要 `return in` / `return out`是因為這樣才能連著寫：`cout << "a = " << a << endl;`。

# 效能分析（Analysis）

下面的 `n` 是第一個多項式的項數，`m` 是第二個多項式的項數。

## 時間複雜度

先看 `NewTerm`它要從頭找一遍有沒有同指數，所以花的時間跟目前的項數成正比，記作 O(項數)。

| 功能 | 時間複雜度 | 白話原因 |
|------|-----------|----------|
| `NewTerm` | O(t)，t 是目前項數 | 要從頭找一遍有沒有同指數 |
| `Add` | O(m × (n+m))，n = m 時是 **O(n²)** | 對方的每一項m 次都要呼叫一次 `NewTerm`，每次最多找 n+m 項 |
| `Mult` | O(n × m × (n+m))，n = m 時是 **O(n³)** | 每一項乘每一項共 n×m 次，每次都要呼叫 `NewTerm`，答案最多有 n+m−1 項要找 |
| `Eval` | **O(n)** | 每一項算一次，總共 n 項 |
| 輸入 `>>` | O(k²)，k 是輸入的項數 | 每輸入一項都呼叫 `NewTerm` |
| 輸出 `<<` | O(n) | 每一項印一次 |


## 空間複雜度

| 功能 | 空間複雜度 | 白話原因 |
|------|-----------|----------|
| 一個多項式 | O(n) | 要放 n 項 |
| `Add` | O(n + m) | 答案最多 n+m 項 |
| `Mult` | O(n + m) | 答案最多 n+m−1 項（同指數會合併） |
| `Eval` | O(1) | 只用一個變數 `s` 存總和 |

因為 `Add(Polynomial q)` 和 `Mult(Polynomial q)` 是用傳值，所以呼叫時會多複製一份 `q`，也是 O(m)，不會改變整體的大小。


# 測試與驗證（Testing and Proving）

## 測試案例

| 測試 | 輸入 a | 輸入 b | 預期結果 | 實際結果 |
|------|--------|--------|----------|----------|
| 1 一般情況 | `3x²+2x` | `x+5` | 和 `3x²+3x+5`、積 `3x³+17x²+10x`、a(2)=16 | 同 |
| 2 相消 | `3x²+2x` | `-3x²-2x` | 和 `0` | `0` |
| 3 輸入同指數 | `3x²` 和 `4x²` 分兩項輸入 | `1` | 自動合併成 `7x²`，a(2)=28 | 同 |
| 4 其中一個是 0 | `0`（0 項） | `x+5` | 和 `x+5`、積 `0`、a(2)=0 | 完全一樣 |
| 5 超過初始容量 | 5 項（`1+x+x²+x³+x⁴`） | `x` | 和 `1+2x+x²+x³+x⁴`、積 `x+x²+x³+x⁴+x⁵`、a(2)=31 | 同 |
| 6 兩數和差的乘積 | `x+1` | `x-1` | 積 `x²-1`（中間的 x 要消掉） | `1x^2 + -1x^0` |
| 7 小數係數 | `2.5x²+1.5` | `2x` | 積 `5x³+3x`、a(2)=11.5 | 同 |

測試 5 是為了確認陣列一開始只有 2 格，但輸入 5 項時，會自動擴大，不會當掉。

## 執行範例（測試 1）

編譯與執行：

```shell
$ g++ -std=c++17 -o poly polynomial.cpp
$ ./poly
```

輸入：

```text
2
3 2
2 1
2
1 1
5 0
```

輸出：

```text
a = 3x^2 + 2x^1
b = 1x^1 + 5x^0
a + b = 3x^2 + 3x^1 + 5x^0
a * b = 3x^3 + 17x^2 + 10x^1
a(2) = 16
```

## 手算驗證

- 加法：`2x + x = 3x`，所以 `3x² + 3x + 5`。
- 乘法：`(3x²+2x)(x+5) = 3x³ + 15x² + 2x² + 10x = 3x³ + 17x² + 10x`。
- 求值：`3×2² + 2×2 = 12 + 4 = 16`。

## 小結論

- 乘法有再用另一個方法確認：`(x+1)(x-1)` 用公式是 `x²-1`，程式的結果一樣，代表同指數合併和變 0 刪掉都正常。
- 加法和乘法的結果，跟手算的結果一致。

# 效能量測（Measuring）

## 怎麼量

用 `<chrono>` 計時。每次把兩個 n 項的多項式（`1 + x + x² + ... + x^(n-1)`）拿來做 `Add`、`Mult`、`Eval`，每個量 5 次取平均。

把 `main` 換成下面這段就可以量，然後記得最上面加 `#include <chrono>`：

```cpp
#include <chrono>
using namespace std::chrono;

Polynomial make(int n)
{           
    Polynomial p;
    for (int i = 0; i < n; i++) p.NewTerm(1, i);
    return p;
}

int main() {
    int sizes[] = {100, 200, 400, 800, 1600};
    for (int n : sizes) {
        Polynomial a = make(n), b = make(n);
        const int R = 5;                        
        double tadd = 0, tmul = 0, tev = 0;
        volatile float sink = 0;              
        for (int r = 0; r < R; r++) {
            auto s = steady_clock::now();
            Polynomial c = a.Add(b);
            tadd += duration<double, milli>(steady_clock::now() - s).count();
            s = steady_clock::now();
            Polynomial d = a.Mult(b);
            tmul += duration<double, milli>(steady_clock::now() - s).count();
            s = steady_clock::now();
            sink = a.Eval(1.0001f);
            tev += duration<double, milli>(steady_clock::now() - s).count();
        }
        printf("n=%d  Add=%.4f ms  Mult=%.3f ms  Eval=%.5f ms\n",
               n, tadd / R, tmul / R, tev / R);
    }
}
```

## 量測結果

（編譯方式：`g++ -std=c++17 -O0`，單位是毫秒 ms）

| n（項數） | Add | Mult | Eval |
|-----------|-----|------|------|
| 100 | 0.0237 | 2.844 | 0.00942 |
| 200 | 0.0583 | 19.637 | 0.01021 |
| 400 | 0.1993 | 157.083 | 0.01505 |
| 800 | 0.6606 | 1120.623 | 0.02555 |
| 1600 | 2.7495 | 9214.835 | 0.05107 |

## 結果怎麼看

n 每變成 2 倍，時間會變幾倍？

| n 變化 | Add 變幾倍 | Mult 變幾倍 | Eval 變幾倍 |
|--------|-----------|------------|------------|
| 100 → 200 | 約 2.5 倍 | 約 6.9 倍 | 約 1.1 倍 |
| 200 → 400 | 約 3.4 倍 | 約 8.0 倍 | 約 1.5 倍 |
| 400 → 800 | 約 3.3 倍 | 約 7.1 倍 | 約 1.7 倍 |
| 800 → 1600 | 約 4.2 倍 | 約 8.2 倍 | 約 2.0 倍 |

- **Eval**：n 變 2 倍，時間大約變 2 倍 → 符合 **O(n)**。
- **Add**：n 變 2 倍，時間大約變 4 倍 → 符合 **O(n²)**。
- **Mult**：n 變 2 倍，時間大約變 8 倍 → 符合 **O(n³)**。

量測結果跟前面算出來的時間複雜度一致。


# 心得討論

## 這次學到什麼

1. **把大問題拆成小問題很有用。** 一開始覺得加法、乘法、輸入都要處理「同指數要合併」「係數是 0 要刪掉」，很躁。後來想到把這些事情全部丟給 `NewTerm` 一個函式處理，其他功能就都變得很短，也不會用錯。
2. **用 `new` 借記憶體，就要負責還。** 一開始我沒寫複製建構子，程式在 `Add` 回傳的時候當掉了。後來才知道是兩個多項式共用同一塊記憶體，其中一個被還掉之後，另一個就指到不存在的地方。所以才要寫「複製建構子、解構子、`operator=`」這三個。
3. **`friend` 不能傳下去。** `Polynomial` 是 `Term` 的朋友，但 `Polynomial` 的朋友 `<<` 不是 `Term` 的朋友，所以 `<<` 讀 `Term` 的係數時一直編譯失敗，要在 `Term` 裡再多寫一行才行。
4. **多載運算子其實不難。** 它就是一個名字比較特別的函式。`>>` 和 `<<` 要回傳 `in` 和 `out`，這樣才能連著寫。

## 遇到的困難

- 剛開始說真的我不太理解 `friend`、前向宣告、為什麼要寫「三個一組」的函式。後來用其他方式想，就比較通了。
- 效能量測時發現 `Mult` 在 n = 1600 要跑快 10 秒，比我想的慢很多。回頭看才發現是 `NewTerm` 每次都從頭找同指數，結果變成 O(n³)。

## 還可以更好的地方

1. 讓陣列永遠照指數排好，加法、乘法就不用每次從頭找，速度會比較快。
2. `Add` 和 `Mult` 的參數可以改成 `const Polynomial&`傳參考，就不用沒次都複製一份對方。
3. 輸出感覺可以再好一點點，像是省略係數 1 的 `1`，把 `1x^1` 印成 `x`，把 `+ -2x` 印成 `- 2x`。

## 最後的總結

透過兩次作業我了解到程式的有趣和無趣，如果搞得出來會有成就感，搞不出來就很無趣和挫敗感，資料結構這種報告還真沒做過，東西很多說實話滿複雜的，但在修改讓它報告完善的過程中，也是有一點好玩的，當然無趣大於有趣。
