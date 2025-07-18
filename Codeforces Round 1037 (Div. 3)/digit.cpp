#include<bits/stdc++.h>
using namespace std;

int main() {
    int t;
    cin >> t;
    while (t--) {
        int x;
        cin >> x;
        if (x < 10) {
            cout << x << endl;
        } else {
            vector<int> d;
            int i = 1;
            while (x > 0) {
                int mod_val = static_cast<int>(pow(10, i));
                d.push_back(x % mod_val);
                x = x / 10;
                i++;
            }
            cout << *min_element(d.begin(), d.end()) << endl;
        }
    }
    return 0;
}