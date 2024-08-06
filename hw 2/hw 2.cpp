#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

// �w�q��ܦh�������ت� Term ���O
class Polynomial; // �e�V�ŧi

class Term {
    friend class Polynomial;
public:
    float coef; // �Y��
    int exp;    // ����
};

// �w�q��ܦh������ Polynomial ���O
class Polynomial {
private:
    Term* termArray; // �D�s�����Ʋ�
    int capacity;    // termArray ���j�p
    int terms;       // �D�s�����ƶq

public:
    // �c�y���
    Polynomial(int cap = 10) : capacity(cap), terms(0) {
        termArray = new Term[capacity];
    }

    // �Ѻc���
    ~Polynomial() {
        delete[] termArray;
    }

    // ���� >> �B��l�A�Ω��J�h����
    friend istream& operator>>(istream& input, Polynomial& p);

    // ���� << �B��l�A�Ω��X�h����
    friend ostream& operator<<(ostream& output, const Polynomial& p);

    // �h�����[�k
    Polynomial operator+(const Polynomial& other) const;

    // �W�[�@�Ӷ�
    void NewTerm(float theCoeff, int theExp);
};

// ���� >> �B��l
istream& operator>>(istream& input, Polynomial& p) {
    int numTerms;
    cout << "��J����: ";
    input >> numTerms;

    for (int i = 0; i < numTerms; ++i) {
        float coef;
        int exp;
        cout << "��J�Y�ƩM����: ";
        input >> coef >> exp;
        p.NewTerm(coef, exp);
    }

    // ���ӫ��ƭ��ǱƧ�
    sort(p.termArray, p.termArray + p.terms, [](const Term& a, const Term& b) {
        return a.exp > b.exp;
        });

    return input;
}

// ���� << �B��l
ostream& operator<<(ostream& output, const Polynomial& p) {
    for (int i = 0; i < p.terms; ++i) {
        output << p.termArray[i].coef << "x^" << p.termArray[i].exp;
        if (i != p.terms - 1) output << " + ";
    }
    return output;
}

// �W�[�@�Ӷ��A�æX�֦P���ƶ�
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

// �h�����[�k
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

    cout << "��J�Ĥ@�Ӧh����:\n";
    cin >> p1;
    cout << "��J�ĤG�Ӧh����:\n";
    cin >> p2;

    cout << "�h����1: " << p1 << endl;
    cout << "�h����2: " << p2 << endl;

    Polynomial p3 = p1 + p2;

    cout << "(" << p1 << ") + (" << p2 << ") = " << p3 << endl;

    return 0;
}
