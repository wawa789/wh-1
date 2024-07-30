#include <iostream>
#include <vector>
using namespace std;
void gen(vector<int>& s, int i, vector<int> c, vector<vector<int>>& r) {
    if (i == s.size()) {
        r.push_back(c);
        return;
    }
    // ���]�t��e����
    gen(s, i + 1, c, r);
    // �]�t��e����
    c.push_back(s[i]);
    gen(s, i + 1, c, r);
}
vector<vector<int>> ps(vector<int>& s) {
    vector<vector<int>> r;
    vector<int> c;
    gen(s, 0, c, r);
    return r;
}
int main() {
    vector<int> s = { 1, 2, 3 };
    vector<vector<int>> r = ps(s);

    cout << "Powerset: " << endl;
    for (const auto& subset : r) {
        cout << "{ ";
        for (int elem : subset) {
            cout << elem << " ";
        }
        cout << "}" << endl;
    }
    return 0;
}
