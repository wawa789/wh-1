#include <iostream>
#include <cmath>   // 包含abs()函數
using namespace std;

// 結構體 Term 表示多項式中的每一項
struct Term {
    float coef;   // 系數
    int exp;      // 指數
    Term* next;   // 指向下一個項的指標，實現環狀鏈結串列
};

// Polynomial 類別用來表示多項式
class Polynomial {
    // 輸出流運算子重載，支援直接輸出多項式
    friend ostream& operator<<(ostream& os, const Polynomial& p);
    // 輸入流運算子重載，支援直接從標準輸入讀入多項式
    friend istream& operator>>(istream& is, Polynomial& p);
private:
    Term* first;  // 指向多項式第一項的指標（環狀鏈結串列）
public:
    Polynomial() : first(nullptr) {}  // 預設構造函數，將first初始化為空指標
    Polynomial(const Polynomial& B);  // 複製構造函數
    ~Polynomial();  // 解構函數
    Polynomial& operator=(const Polynomial& B);  // 賦值運算子重載
    Polynomial operator+(const Polynomial& B) const;  // 加法運算子重載
    Polynomial operator-(const Polynomial& B) const;  // 減法運算子重載
    Polynomial operator*(const Polynomial& B) const;  // 乘法運算子重載
    float Evaluate(float x) const;  // 計算多項式在某一點的值
    void newTerm(float coef, int exp);  // 新增一個新項到多項式中
};

// 新增一個新項
void Polynomial::newTerm(float coef, int exp) {
    if (coef == 0) return;  // 如果係數為零，則不加入該項

    // 創建新項並初始化
    Term* newTerm = new Term{ coef, exp, nullptr };
    if (first == nullptr) {  // 如果是第一個加入的項
        first = newTerm;
        first->next = first;  // 自己指向自己，形成環狀鏈結串列
    }
    else {
        Term* current = first;
        Term* previous = nullptr;
        // 找到應插入的位置
        do {
            previous = current;
            current = current->next;
        } while (current != first && current->exp > exp);

        if (current->exp == exp) {  // 如果有相同指數的項，則合併
            current->coef += coef;
            if (current->coef == 0) {  // 如果合併後係數為零，則刪除該項
                if (current == first) {  // 如果是第一個項，則更新first指標
                    first = (first == first->next) ? nullptr : first->next;
                }
                previous->next = current->next;
                delete current;  // 釋放記憶體
            }
            return;
        }

        previous->next = newTerm;  // 插入新項
        newTerm->next = current;   // 確保鏈結串列連接正確
        if (current == first) {  // 如果插入的是第一個位置，則更新first指標
            first = newTerm;
        }
    }
}

// 重載 << 運算子來輸出多項式
ostream& operator<<(ostream& os, const Polynomial& p) {
    if (p.first == nullptr) {  // 如果多項式為空，則輸出0
        os << "0";
        return os;
    }
    Term* current = p.first;
    bool firstTerm = true;  // 標記是否為第一項
    do {
        if (firstTerm) {
            if (current->coef < 0) {
                os << "- ";  // 如果第一項係數為負，則輸出負號
            }
        }
        else {
            if (current->coef < 0) {
                os << " - ";  // 如果係數為負，則輸出 - 號
            }
            else {
                os << " + ";  // 如果係數為正，則輸出 + 號
            }
        }

        os << abs(current->coef);  // 輸出係數絕對值
        if (current->exp > 0) {  // 如果指數大於0，則輸出x
            os << "x";
            if (current->exp > 1) {  // 如果指數大於1，則輸出x^exp
                os << "^" << current->exp;
            }
        }

        firstTerm = false;  // 標記已經輸出第一項
        current = current->next;
    } while (current != p.first);
    return os;
}

// 重載 >> 運算子來讀入多項式
istream& operator>>(istream& is, Polynomial& p) {
    float coef;
    int exp;
    while (is >> coef >> exp) {  // 讀入係數與指數
        p.newTerm(coef, exp);  // 新增該項
    }
    return is;
}

// 複製構造函數
Polynomial::Polynomial(const Polynomial& B) : first(nullptr) {
    Term* current = B.first;
    if (current != nullptr) {
        do {
            newTerm(current->coef, current->exp);  // 使用newTerm新增項
            current = current->next;
        } while (current != B.first);
    }
}

// 解構函數
Polynomial::~Polynomial() {
    if (first != nullptr) {  // 如果多項式不為空
        Term* current = first;
        do {
            Term* nextTerm = current->next;
            delete current;  // 釋放記憶體
            current = nextTerm;
        } while (current != first);
    }
}

// 賦值運算子
Polynomial& Polynomial::operator=(const Polynomial& B) {
    if (this != &B) {  
        Polynomial temp(B);  // 使用複製構造函數創建臨時物件
        swap(first, temp.first);  // 交換資料
    }
    return *this;
}

// 多項式加法
Polynomial Polynomial::operator+(const Polynomial& B) const {
    Polynomial result;
    Term* i = this->first, * j = B.first;
    do {
        result.newTerm(i->coef, i->exp);  // 將this中的每一項加入結果
        i = i->next;
    } while (i != this->first);
    do {
        result.newTerm(j->coef, j->exp);  // 將B中的每一項加入結果
        j = j->next;
    } while (j != B.first);
    return result;
}

// 多項式減法
Polynomial Polynomial::operator-(const Polynomial& B) const {
    Polynomial negB;  // 創建B的負多項式
    Term* j = B.first;
    do {
        negB.newTerm(-j->coef, j->exp);  // 將B中的每一項的係數取負後加入negB
        j = j->next;
    } while (j != B.first);
    return *this + negB;  // 將this與negB相加
}

// 多項式乘法
Polynomial Polynomial::operator*(const Polynomial& B) const {
    Polynomial result;
    Term* i = this->first;
    do {
        Term* j = B.first;
        do {
            result.newTerm(i->coef * j->coef, i->exp + j->exp);  // 計算每一項的乘積
            j = j->next;
        } while (j != B.first);
        i = i->next;
    } while (i != this->first);
    return result;
}

// 計算多項式的值
float Polynomial::Evaluate(float x) const {
    float result = 0;
    Term* current = this->first;
    do {
        result += current->coef * pow(x, current->exp);  // 計算當前項在x處的值並加總
        current = current->next;
    } while (current != this->first);
    return result;  // 返回結果
}

// 主函數來測試
#include <iostream>
using namespace std;

int main() {
    Polynomial p1, p2;

    // 設定第一個多項式 P1(x) = 4x^3 + 3x^2 + 2x + 1
    p1.newTerm(4, 3);
    p1.newTerm(3, 2);
    p1.newTerm(2, 1);
    p1.newTerm(1, 0);

    // 設定第二個多項式 P2(x) = -2x^2 - x + 5
    p2.newTerm(-2, 2);
    p2.newTerm(-1, 1);
    p2.newTerm(5, 0);

    // 輸出多項式
    cout << "P1(x) = " << p1 << endl;
    cout << "P2(x) = " << p2 << endl;

    // 計算加法、減法和乘法
    Polynomial sum = p1 + p2;
    Polynomial diff = p1 - p2;
    Polynomial prod = p1 * p2;

    cout << "P1(x) + P2(x) = " << sum << endl;
    cout << "P1(x) - P2(x) = " << diff << endl;
    cout << "P1(x) * P2(x) = " << prod << endl;

    // 計算多項式在某一點的值
    float x = 2.0; // 設定 x 的值為 2.0
    cout << "P1 在 x = " << x << " 處的值: " << p1.Evaluate(x) << endl;
    cout << "P2 在 x = " << x << " 處的值: " << p2.Evaluate(x) << endl;
    cout << "和在 x = " << x << " 處的值: " << sum.Evaluate(x) << endl;
    cout << "差在 x = " << x << " 處的值: " << diff.Evaluate(x) << endl;
    cout << "積在 x = " << x << " 處的值: " << prod.Evaluate(x) << endl;


    return 0;
}
