#include <iostream>
#include <string>

using namespace std;

// 阿克曼函數函数
int a(int m,int n) {
    if (m == 0) {
        return n + 1;
    }
    else if (m > 0 && n == 0) {
        return a(m - 1, 1);
    }
    else if (m > 0 && n > 0) {
        return a(m - 1, a(m, n - 1));
    }
    return 0; 
}

int main() {
    int m, n;
    cout << "輸入m和n的值: ";
    cin >> m >> n;

    cout << "阿克曼函數(" << m << ", " << n << ") = " << a(m, n) << endl;

    return 0;
}
