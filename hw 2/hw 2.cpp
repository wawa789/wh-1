#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

// 定義表示多項式項目的 Term 類別
class Polynomial; // 前向宣告

class Term {
    friend class Polynomial;
public:
    float coef; // 係數
    int exp;    // 指數
};

// 定義表示多項式的 Polynomial 類別
class Polynomial {
private:
    Term* termArray; // 非零項的數組
    int capacity;    // termArray 的大小
    int terms;       // 非零項的數量

public:
    // 構造函數
    Polynomial(int cap = 10) : capacity(cap), terms(0) {
        termArray = new Term[capacity];
    }

    // 解構函數
    ~Polynomial() {
        delete[] termArray;
    }

    // 重載 >> 運算子，用於輸入多項式
    friend istream& operator>>(istream& input, Polynomial& p);

    // 重載 << 運算子，用於輸出多項式
    friend ostream& operator<<(ostream& output, const Polynomial& p);

    // 多項式加法
    Polynomial operator+(const Polynomial& other) const;

    // 增加一個項
    void NewTerm(float theCoeff, int theExp);
};

// 重載 >> 運算子
istream& operator>>(istream& input, Polynomial& p) {
    int numTerms;
    cout << "輸入項數: ";
    input >> numTerms;

    for (int i = 0; i < numTerms; ++i) {
        float coef;
        int exp;
        cout << "輸入係數和指數: ";
        input >> coef >> exp;
        p.NewTerm(coef, exp);
    }

    // 按照指數降序排序
    sort(p.termArray, p.termArray + p.terms, [](const Term& a, const Term& b) {
        return a.exp > b.exp;
        });

    return input;
}

// 重載 << 運算子
ostream& operator<<(ostream& output, const Polynomial& p) {
    for (int i = 0; i < p.terms; ++i) {
        output << p.termArray[i].coef << "x^" << p.termArray[i].exp;
        if (i != p.terms - 1) output << " + ";
    }
    return output;
}

// 增加一個項，並合併同指數項
void Polynomial::NewTerm(float theCoeff, int theExp) {
    for (int i = 0; i < terms; ++i) {
        if (termArray[i].exp == theExp) {
            termArray[i].coef += theCoeff;
            return;
        }
    }
    if (terms == capacity) {
        capacity *= 2;
        Term* newArray = new Term[capacity];
        copy(termArray, termArray + terms, newArray);
        delete[] termArray;
        termArray = newArray;
    }
    termArray[terms].coef = theCoeff;
    termArray[terms++].exp = theExp;
}

// 多項式加法
Polynomial Polynomial::operator+(const Polynomial& other) const {
    Polynomial c;
    int aPos = 0, bPos = 0;
    while (aPos < terms && bPos < other.terms) {
        if (termArray[aPos].exp == other.termArray[bPos].exp) {
            float t = termArray[aPos].coef + other.termArray[bPos].coef;
            if (t) c.NewTerm(t, termArray[aPos].exp);
            aPos++;
            bPos++;
        }
        else if (termArray[aPos].exp < other.termArray[bPos].exp) {
            c.NewTerm(other.termArray[bPos].coef, other.termArray[bPos].exp);
            bPos++;
        }
        else {
            c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
            aPos++;
        }
    }
    for (; aPos < terms; aPos++)
        c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
    for (; bPos < other.terms; bPos++)
        c.NewTerm(other.termArray[bPos].coef, other.termArray[bPos].exp);
    return c;
}

int main() {
    Polynomial p1, p2;

    cout << "輸入第一個多項式:\n";
    cin >> p1;
    cout << "輸入第二個多項式:\n";
    cin >> p2;

    cout << "多項式1: " << p1 << endl;
    cout << "多項式2: " << p2 << endl;

    Polynomial p3 = p1 + p2;

    cout << "(" << p1 << ") + (" << p2 << ") = " << p3 << endl;

    return 0;
}
