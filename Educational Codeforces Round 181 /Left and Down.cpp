#include <bits/stdc++.h>
using namespace std;

#define int long long

int32_t main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tests;
    cin >> tests;
    while (tests--) {
        int x, y, limit;
        cin >> x >> y >> limit;
        int g = __gcd(x, y);
        int step_x = x / g;
        int step_y = y / g;
        cout << (step_x <= limit && step_y <= limit ? 1 : 2) << '\n';
    }
    return 0;
}