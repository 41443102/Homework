41443102

作業二：多項式（Polynomial）

# 解題說明

## 題目在問什麼

這次要我們寫一個 `Polynomial`（多項式）類別，要做到兩件事：

1. 照老師給的圖 1、圖 2，做出多項式，並且能做 **加法 `Add`**、**乘法 `Mult`**、**代入求值 `Eval`**。
2. 寫輸入和輸出的函式，而且要**多載 `>>` 和 `<<`**，這樣就能直接寫 `cin >> p;` 和 `cout << p;`。

## 我的想法（How to do?）

多項式 `3x² + 2x` 可以拆成兩個小東西：`3x²` 和 `2x`。每個小東西只要記兩個數字：**係數**和**指數**。所以：

- 一個小東西叫 `Term`（一項），裡面放 `coef`（係數）和 `exp`（指數）。
- 整個多項式就是把很多個 `Term` 排成一排，放在陣列 `termArray` 裡。
- 另外用 `terms` 記現在有幾項，用 `capacity` 記陣列最多放幾項。
- 老師說只存**非零項**，所以係數是 0 的項不用存。

最重要的一個想法：我多寫了一個小幫手函式 `NewTerm(c, e)`，意思是「幫我加一項」。它會自己處理三件麻煩事：

1. 係數是 0，就不存。
2. 已經有同指數的項，就把係數加起來（`3x² + 2x² = 5x²`）。
3. 陣列滿了，就換一個兩倍大的陣列。

有了它，其他功能都變得很簡單：

- **加法**：先複製自己，再把對方的每一項用 `NewTerm` 丟進去。
- **乘法**：每一項乘每一項（國中的分配律），結果也用 `NewTerm` 丟進去，同指數的會自動合併。
- **求值**：每一項算「係數 × x 的指數次方」，全部加起來。

## 舉例說明

假設 `a = 3x² + 2x`，`b = x + 5`。

**加法**：先把 `a` 抄過來變成 `3x² + 2x`，再丟 `1x¹` 進去，發現已經有 `2x¹`，所以變成 `3x¹`；再丟 `5x⁰`，沒有同指數，就新增。答案：`3x² + 3x + 5`。

**乘法**：

```text
3x² × x = 3x³
3x² × 5 = 15x²
2x  × x = 2x²      ← 跟 15x² 同指數，合併成 17x²
2x  × 5 = 10x
答案：3x³ + 17x² + 10x
```

**求值**：把 `x = 2` 代進 `a`：`3×2² + 2×2 = 12 + 4 = 16`。

## 輸入格式

先輸入「有幾項」，再輸入每一項的「係數 指數」。例如 `3x² + 2x` 要輸入：

```text
2
3 2
2 1
```

# Algorithm Design & Programming

## 程式碼（含註解）

```cpp
// Homework 2：Polynomial（多項式）
#include <iostream>   // 要用 cin、cout
#include <cmath>      // 要用 pow（算次方）
using namespace std;

class Polynomial;     // 前向宣告：先跟電腦說「等等會有 Polynomial 喔」

// ---------- 一項（例如 3x^2） ----------
class Term {
    friend class Polynomial;                                         // 讓 Polynomial 可以直接用我的私有資料
    friend ostream& operator<<(ostream& out, const Polynomial& p);   // 讓輸出函式也可以用
private:
    float coef;   // 係數（3x^2 的 3）
    int exp;      // 指數（3x^2 的 2）
};

// ---------- 整個多項式（很多項排成一排） ----------
class Polynomial {
    friend istream& operator>>(istream& in, Polynomial& p);          // 輸入 >>
    friend ostream& operator<<(ostream& out, const Polynomial& p);   // 輸出 <<
public:
    Polynomial();                                // 建構子：做出 p(x) = 0
    Polynomial(const Polynomial& b);             // 複製建構子：複製一份新的
    ~Polynomial();                               // 解構子：把借的記憶體還回去
    Polynomial& operator=(const Polynomial& b);  // 指定：a = b

    Polynomial Add(Polynomial q);                // 加法：自己 + q
    Polynomial Mult(Polynomial q);               // 乘法：自己 * q
    float Eval(float x);                         // 代入 x，算出值

    void NewTerm(float c, int e);                // 新增一項（係數 c、指數 e）
private:
    Term *termArray;   // 放所有項的陣列
    int capacity;      // 陣列最多放幾項
    int terms;         // 現在實際有幾項
};

// 建構子：一開始是 0 多項式
Polynomial::Polynomial() {
    capacity = 2;                    // 先準備 2 格
    terms = 0;                       // 現在 0 項
    termArray = new Term[capacity];  // 跟電腦借一塊記憶體
}

// 解構子：用完把記憶體還回去
Polynomial::~Polynomial() {
    delete [] termArray;
}

// 複製建構子：b 是被複製的那個
Polynomial::Polynomial(const Polynomial& b) {
    capacity = b.capacity;               // 容量照抄
    terms = b.terms;                     // 項數照抄
    termArray = new Term[capacity];      // 自己借一塊新的（不跟 b 共用）
    for (int i = 0; i < terms; i++)
        termArray[i] = b.termArray[i];   // 一項一項抄過來
}

// 指定運算子：a = b
Polynomial& Polynomial::operator=(const Polynomial& b) {
    if (this == &b) return *this;        // 自己等於自己就不用做事
    delete [] termArray;                 // 先把舊的還掉
    capacity = b.capacity;
    terms = b.terms;
    termArray = new Term[capacity];      // 借新的
    for (int i = 0; i < terms; i++)
        termArray[i] = b.termArray[i];   // 抄資料
    return *this;
}

// 新增一項：c 是係數，e 是指數
void Polynomial::NewTerm(float c, int e) {
    if (c == 0) return;                  // 係數是 0 就不存

    for (int i = 0; i < terms; i++) {    // 先找有沒有「同指數」的項
        if (termArray[i].exp == e) {
            termArray[i].coef += c;      // 有 → 係數加起來
            if (termArray[i].coef == 0) {            // 加完變 0 → 這項刪掉
                for (int j = i; j < terms - 1; j++)
                    termArray[j] = termArray[j + 1]; // 後面的往前補位
                terms--;                             // 項數少一個
            }
            return;                      // 處理完了
        }
    }

    if (terms == capacity) {             // 陣列滿了 → 換一個兩倍大的箱子
        capacity = capacity * 2;
        Term *big = new Term[capacity];
        for (int i = 0; i < terms; i++)
            big[i] = termArray[i];       // 舊資料搬過去
        delete [] termArray;             // 丟掉舊箱子
        termArray = big;                 // 改用新箱子
    }

    termArray[terms].coef = c;           // 放在最後面
    termArray[terms].exp = e;
    terms++;                             // 項數加一
}

// 加法
Polynomial Polynomial::Add(Polynomial q) {
    Polynomial ans = *this;              // 答案先從「自己」開始
    for (int i = 0; i < q.terms; i++)    // 把 q 的每一項加進答案
        ans.NewTerm(q.termArray[i].coef, q.termArray[i].exp);
    return ans;
}

// 乘法：每一項乘每一項（分配律）
Polynomial Polynomial::Mult(Polynomial q) {
    Polynomial ans;                      // 空的（0）多項式
    for (int i = 0; i < terms; i++) {
        for (int j = 0; j < q.terms; j++) {
            float c = termArray[i].coef * q.termArray[j].coef;  // 係數相乘
            int e = termArray[i].exp + q.termArray[j].exp;      // 指數相加
            ans.NewTerm(c, e);           // 加進答案（同指數自動合併）
        }
    }
    return ans;
}

// 代入 x 算值
float Polynomial::Eval(float x) {
    float s = 0;                         // s 是總和，從 0 開始
    for (int i = 0; i < terms; i++)
        s += termArray[i].coef * pow(x, termArray[i].exp);  // 係數 × x 的指數次方
    return s;
}

// 輸入：先輸入項數 n，再輸入 n 組「係數 指數」
istream& operator>>(istream& in, Polynomial& p) {
    int n;
    in >> n;                             // 讀項數
    p.terms = 0;                         // 先清空
    for (int i = 0; i < n; i++) {
        float c;
        int e;
        in >> c >> e;                    // 讀「係數 指數」
        p.NewTerm(c, e);                 // 加進去
    }
    return in;                           // 回傳 in，才能連續寫 cin >> a >> b
}

// 輸出：印成 3x^2 + 2x^1
ostream& operator<<(ostream& out, const Polynomial& p) {
    if (p.terms == 0) {                  // 沒有任何項就印 0
        out << "0";
        return out;
    }
    for (int i = 0; i < p.terms; i++) {
        if (i > 0) out << " + ";         // 第二項開始前面加 +
        out << p.termArray[i].coef << "x^" << p.termArray[i].exp;
    }
    return out;                          // 回傳 out，才能連續寫 cout << a << b
}

int main() {
    Polynomial a, b;

    cout << "請輸入第一個多項式（項數，然後每項 係數 指數）：" << endl;
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

## 幾個重點說明

- **為什麼要 `friend`？** `Term` 的資料是私有的（`private`），外人不能碰。要讓 `Polynomial` 和 `<<` 能讀它，就要在 `Term` 裡寫 `friend`。順便一提，「朋友的朋友」不算朋友，所以 `<<` 要自己也被 `Term` 指定成朋友。
- **為什麼要寫複製建構子、解構子、`operator=`？** 因為我們用 `new` 跟電腦借了記憶體。如果不寫，`Add` 回傳答案時，兩個多項式會共用同一塊記憶體，其中一個被還掉之後，另一個就壞了，程式會當掉。
- **為什麼 `operator>>`、`operator<<` 最後要 `return in` / `return out`？** 這樣才能連著寫：`cout << "a = " << a << endl;`。

# 效能分析（Analysis）

下面的 `n` 是第一個多項式的項數，`m` 是第二個多項式的項數。

## 時間複雜度

先看小幫手 `NewTerm`：它要從頭找一遍有沒有同指數，所以花的時間跟目前的項數成正比，記作 O(項數)。

| 功能 | 時間複雜度 | 白話原因 |
|------|-----------|----------|
| `NewTerm` | O(t)，t 是目前項數 | 要從頭找一遍有沒有同指數 |
| `Add` | O(m × (n+m))，n = m 時是 **O(n²)** | 對方的每一項（m 次）都要呼叫一次 `NewTerm`，每次最多找 n+m 項 |
| `Mult` | O(n × m × (n+m))，n = m 時是 **O(n³)** | 每一項乘每一項共 n×m 次，每次都要呼叫 `NewTerm`，答案最多有 n+m−1 項要找 |
| `Eval` | **O(n)** | 每一項算一次，總共 n 項 |
| 輸入 `>>` | O(k²)，k 是輸入的項數 | 每輸入一項都呼叫 `NewTerm` |
| 輸出 `<<` | O(n) | 每一項印一次 |

陣列滿了要換大箱子，這個動作要把舊資料搬一遍，但因為每次都是容量變兩倍，所以平均下來每次新增只多花一點點，不會影響上面的結果。

## 空間複雜度

| 功能 | 空間複雜度 | 白話原因 |
|------|-----------|----------|
| 一個多項式 | O(n) | 要放 n 項 |
| `Add` | O(n + m) | 答案最多 n+m 項 |
| `Mult` | O(n + m) | 答案最多 n+m−1 項（同指數會合併） |
| `Eval` | O(1) | 只用一個變數 `s` 存總和 |

因為 `Add(Polynomial q)` 和 `Mult(Polynomial q)` 是用「傳值」，所以呼叫時會多複製一份 `q`，也是 O(m)，不會改變整體的大小。

## 可以改進的地方

`NewTerm` 每次都從頭找同指數，所以慢。如果讓陣列永遠**照指數大小排好**，加法和乘法就可以用「雙指標」一次掃過去，加法變成 O(n+m)。不過這次的作業重點是照圖 1、圖 2 把類別做出來，所以我選擇比較好懂的寫法。

# 測試與驗證（Testing and Proving）

## 測試案例

| 測試 | 輸入 a | 輸入 b | 預期結果 | 實際結果 |
|------|--------|--------|----------|----------|
| 1 一般情況 | `3x²+2x` | `x+5` | 和 `3x²+3x+5`、積 `3x³+17x²+10x`、a(2)=16 | 完全一樣 |
| 2 相消 | `3x²+2x` | `-3x²-2x` | 和 `0` | `0` |
| 3 輸入同指數 | `3x²` 和 `4x²` 分兩項輸入 | `1` | 自動合併成 `7x²`，a(2)=28 | 完全一樣 |
| 4 其中一個是 0 | `0`（0 項） | `x+5` | 和 `x+5`、積 `0`、a(2)=0 | 完全一樣 |
| 5 超過初始容量 | 5 項（`1+x+x²+x³+x⁴`） | `x` | 和 `1+2x+x²+x³+x⁴`、積 `x+x²+x³+x⁴+x⁵`、a(2)=31 | 完全一樣 |
| 6 兩數和差的乘積 | `x+1` | `x-1` | 積 `x²-1`（中間的 x 要消掉） | `1x^2 + -1x^0` |
| 7 小數係數 | `2.5x²+1.5` | `2x` | 積 `5x³+3x`、a(2)=11.5 | 完全一樣 |

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

- 加法：`2x + x = 3x`，所以 `3x² + 3x + 5`。✔
- 乘法：`(3x²+2x)(x+5) = 3x³ + 15x² + 2x² + 10x = 3x³ + 17x² + 10x`。✔
- 求值：`3×2² + 2×2 = 12 + 4 = 16`。✔

## 驗證小結

- 乘法有再用另一個方法確認：`(x+1)(x-1)` 用公式是 `x²-1`，程式的結果一樣，代表「同指數合併」和「變 0 刪掉」都正常。
- 加法和乘法的結果，跟手算的結果一致。

# 效能量測（Measuring）

## 怎麼量

用 `<chrono>` 計時。每次把兩個 n 項的多項式（`1 + x + x² + ... + x^(n-1)`）拿來做 `Add`、`Mult`、`Eval`，每個量 5 次取平均。

把 `main` 換成下面這段就可以量（記得最上面加 `#include <chrono>`）：

```cpp
#include <chrono>
using namespace std::chrono;

Polynomial make(int n) {            // 做出 n 項：1x^0 + 1x^1 + ... + 1x^(n-1)
    Polynomial p;
    for (int i = 0; i < n; i++) p.NewTerm(1, i);
    return p;
}

int main() {
    int sizes[] = {100, 200, 400, 800, 1600};
    for (int n : sizes) {
        Polynomial a = make(n), b = make(n);
        const int R = 5;                        // 每個量 5 次取平均
        double tadd = 0, tmul = 0, tev = 0;
        volatile float sink = 0;                // 避免編譯器把 Eval 刪掉
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

> 註：時間會因為電腦不同而不一樣（上面是我這次實際跑出來的數字），但「n 變 2 倍，時間變幾倍」的規律是一樣的。

# 心得討論

## 這次學到什麼

1. **把大問題拆成小問題很有用。** 一開始覺得加法、乘法、輸入都要處理「同指數要合併」「係數是 0 要刪掉」，很麻煩。後來想到把這些事情全部丟給 `NewTerm` 一個函式處理，其他功能就都變得很短，也不容易寫錯。
2. **用 `new` 借記憶體，就要負責還。** 一開始我沒寫複製建構子，程式在 `Add` 回傳的時候當掉了。後來才知道是兩個多項式共用同一塊記憶體，其中一個被還掉之後，另一個就指到不存在的地方。所以才要寫「複製建構子、解構子、`operator=`」這三個。
3. **`friend` 不能傳下去。** `Polynomial` 是 `Term` 的朋友，但 `Polynomial` 的朋友 `<<` 不是 `Term` 的朋友，所以 `<<` 讀 `Term` 的係數時一直編譯失敗，要在 `Term` 裡再多寫一行才行。
4. **多載運算子其實不難。** 它就是一個名字比較特別的函式。`>>` 和 `<<` 要回傳 `in` 和 `out`，這樣才能連著寫。

## 遇到的困難

- 一開始很難理解 `friend`、前向宣告、為什麼要寫「三個一組」的函式。後來用「便當盒」和「借東西要還」的方式去想，就比較懂了。
- 效能量測時發現 `Mult` 在 n = 1600 要跑快 10 秒，比我想的慢很多。回頭看才發現是 `NewTerm` 每次都從頭找同指數，結果變成 O(n³)。

## 還可以更好的地方

1. 讓陣列永遠照指數排好，加法、乘法就不用每次從頭找，速度會快很多。
2. `Add` 和 `Mult` 的參數可以改成 `const Polynomial&`（傳參考），就不用每次都複製一份對方。
3. 輸出可以再漂亮一點，例如省略係數 1 的 `1`，把 `1x^1` 印成 `x`，把 `+ -2x` 印成 `- 2x`。
4. 比較浮點數是不是 0（`c == 0`）有一點危險，因為小數有誤差。這次的測試用整數和簡單的小數所以沒問題，更嚴謹的話應該設一個很小的範圍來比較。
