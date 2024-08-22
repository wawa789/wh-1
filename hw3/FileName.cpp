#include <iostream>
#include <cmath>   // �]�tabs()���
using namespace std;

// ���c�� Term ��ܦh���������C�@��
struct Term {
    float coef;   // �t��
    int exp;      // ����
    Term* next;   // ���V�U�@�Ӷ������СA��{�����쵲��C
};

// Polynomial ���O�ΨӪ�ܦh����
class Polynomial {
    // ��X�y�B��l�����A�䴩������X�h����
    friend ostream& operator<<(ostream& os, const Polynomial& p);
    // ��J�y�B��l�����A�䴩�����q�зǿ�JŪ�J�h����
    friend istream& operator>>(istream& is, Polynomial& p);
private:
    Term* first;  // ���V�h�����Ĥ@�������С]�����쵲��C�^
public:
    Polynomial() : first(nullptr) {}  // �w�]�c�y��ơA�Nfirst��l�Ƭ��ū���
    Polynomial(const Polynomial& B);  // �ƻs�c�y���
    ~Polynomial();  // �Ѻc���
    Polynomial& operator=(const Polynomial& B);  // ��ȹB��l����
    Polynomial operator+(const Polynomial& B) const;  // �[�k�B��l����
    Polynomial operator-(const Polynomial& B) const;  // ��k�B��l����
    Polynomial operator*(const Polynomial& B) const;  // ���k�B��l����
    float Evaluate(float x) const;  // �p��h�����b�Y�@�I����
    void newTerm(float coef, int exp);  // �s�W�@�ӷs����h������
};

// �s�W�@�ӷs��
void Polynomial::newTerm(float coef, int exp) {
    if (coef == 0) return;  // �p�G�Y�Ƭ��s�A�h���[�J�Ӷ�

    // �Ыطs���ê�l��
    Term* newTerm = new Term{ coef, exp, nullptr };
    if (first == nullptr) {  // �p�G�O�Ĥ@�ӥ[�J����
        first = newTerm;
        first->next = first;  // �ۤv���V�ۤv�A�Φ������쵲��C
    }
    else {
        Term* current = first;
        Term* previous = nullptr;
        // ��������J����m
        do {
            previous = current;
            current = current->next;
        } while (current != first && current->exp > exp);

        if (current->exp == exp) {  // �p�G���ۦP���ƪ����A�h�X��
            current->coef += coef;
            if (current->coef == 0) {  // �p�G�X�֫�Y�Ƭ��s�A�h�R���Ӷ�
                if (current == first) {  // �p�G�O�Ĥ@�Ӷ��A�h��sfirst����
                    first = (first == first->next) ? nullptr : first->next;
                }
                previous->next = current->next;
                delete current;  // ����O����
            }
            return;
        }

        previous->next = newTerm;  // ���J�s��
        newTerm->next = current;   // �T�O�쵲��C�s�����T
        if (current == first) {  // �p�G���J���O�Ĥ@�Ӧ�m�A�h��sfirst����
            first = newTerm;
        }
    }
}

// ���� << �B��l�ӿ�X�h����
ostream& operator<<(ostream& os, const Polynomial& p) {
    if (p.first == nullptr) {  // �p�G�h�������šA�h��X0
        os << "0";
        return os;
    }
    Term* current = p.first;
    bool firstTerm = true;  // �аO�O�_���Ĥ@��
    do {
        if (firstTerm) {
            if (current->coef < 0) {
                os << "- ";  // �p�G�Ĥ@���Y�Ƭ��t�A�h��X�t��
            }
        }
        else {
            if (current->coef < 0) {
                os << " - ";  // �p�G�Y�Ƭ��t�A�h��X - ��
            }
            else {
                os << " + ";  // �p�G�Y�Ƭ����A�h��X + ��
            }
        }

        os << abs(current->coef);  // ��X�Y�Ƶ����
        if (current->exp > 0) {  // �p�G���Ƥj��0�A�h��Xx
            os << "x";
            if (current->exp > 1) {  // �p�G���Ƥj��1�A�h��Xx^exp
                os << "^" << current->exp;
            }
        }

        firstTerm = false;  // �аO�w�g��X�Ĥ@��
        current = current->next;
    } while (current != p.first);
    return os;
}

// ���� >> �B��l��Ū�J�h����
istream& operator>>(istream& is, Polynomial& p) {
    float coef;
    int exp;
    while (is >> coef >> exp) {  // Ū�J�Y�ƻP����
        p.newTerm(coef, exp);  // �s�W�Ӷ�
    }
    return is;
}

// �ƻs�c�y���
Polynomial::Polynomial(const Polynomial& B) : first(nullptr) {
    Term* current = B.first;
    if (current != nullptr) {
        do {
            newTerm(current->coef, current->exp);  // �ϥ�newTerm�s�W��
            current = current->next;
        } while (current != B.first);
    }
}

// �Ѻc���
Polynomial::~Polynomial() {
    if (first != nullptr) {  // �p�G�h����������
        Term* current = first;
        do {
            Term* nextTerm = current->next;
            delete current;  // ����O����
            current = nextTerm;
        } while (current != first);
    }
}

// ��ȹB��l
Polynomial& Polynomial::operator=(const Polynomial& B) {
    if (this != &B) {  
        Polynomial temp(B);  // �ϥνƻs�c�y��ƳЫ��{�ɪ���
        swap(first, temp.first);  // �洫���
    }
    return *this;
}

// �h�����[�k
Polynomial Polynomial::operator+(const Polynomial& B) const {
    Polynomial result;
    Term* i = this->first, * j = B.first;
    do {
        result.newTerm(i->coef, i->exp);  // �Nthis�����C�@���[�J���G
        i = i->next;
    } while (i != this->first);
    do {
        result.newTerm(j->coef, j->exp);  // �NB�����C�@���[�J���G
        j = j->next;
    } while (j != B.first);
    return result;
}

// �h������k
Polynomial Polynomial::operator-(const Polynomial& B) const {
    Polynomial negB;  // �Ы�B���t�h����
    Term* j = B.first;
    do {
        negB.newTerm(-j->coef, j->exp);  // �NB�����C�@�����Y�ƨ��t��[�JnegB
        j = j->next;
    } while (j != B.first);
    return *this + negB;  // �Nthis�PnegB�ۥ[
}

// �h�������k
Polynomial Polynomial::operator*(const Polynomial& B) const {
    Polynomial result;
    Term* i = this->first;
    do {
        Term* j = B.first;
        do {
            result.newTerm(i->coef * j->coef, i->exp + j->exp);  // �p��C�@�������n
            j = j->next;
        } while (j != B.first);
        i = i->next;
    } while (i != this->first);
    return result;
}

// �p��h��������
float Polynomial::Evaluate(float x) const {
    float result = 0;
    Term* current = this->first;
    do {
        result += current->coef * pow(x, current->exp);  // �p���e���bx�B���Ȩå[�`
        current = current->next;
    } while (current != this->first);
    return result;  // ��^���G
}

// �D��ƨӴ���
#include <iostream>
using namespace std;

int main() {
    Polynomial p1, p2;

    // �]�w�Ĥ@�Ӧh���� P1(x) = 4x^3 + 3x^2 + 2x + 1
    p1.newTerm(4, 3);
    p1.newTerm(3, 2);
    p1.newTerm(2, 1);
    p1.newTerm(1, 0);

    // �]�w�ĤG�Ӧh���� P2(x) = -2x^2 - x + 5
    p2.newTerm(-2, 2);
    p2.newTerm(-1, 1);
    p2.newTerm(5, 0);

    // ��X�h����
    cout << "P1(x) = " << p1 << endl;
    cout << "P2(x) = " << p2 << endl;

    // �p��[�k�B��k�M���k
    Polynomial sum = p1 + p2;
    Polynomial diff = p1 - p2;
    Polynomial prod = p1 * p2;

    cout << "P1(x) + P2(x) = " << sum << endl;
    cout << "P1(x) - P2(x) = " << diff << endl;
    cout << "P1(x) * P2(x) = " << prod << endl;

    // �p��h�����b�Y�@�I����
    float x = 2.0; // �]�w x ���Ȭ� 2.0
    cout << "P1 �b x = " << x << " �B����: " << p1.Evaluate(x) << endl;
    cout << "P2 �b x = " << x << " �B����: " << p2.Evaluate(x) << endl;
    cout << "�M�b x = " << x << " �B����: " << sum.Evaluate(x) << endl;
    cout << "�t�b x = " << x << " �B����: " << diff.Evaluate(x) << endl;
    cout << "�n�b x = " << x << " �B����: " << prod.Evaluate(x) << endl;


    return 0;
}
