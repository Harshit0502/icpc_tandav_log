#include <bits/stdc++.h>
using namespace std;

#define int long long
const int modval = 998244353;

int binpow(int a, int b) {
    int res = 1;
    a %= modval;
    while (b > 0) {
        if (b & 1) res = res * a % modval;
        a = a * a % modval;
        b >>= 1;
    }
    return res;
}

int32_t main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int segs, size;
    cin >> segs >> size;

    vector<vector<pair<int, int>>> links(size + 1);
    int total = 1;

    for (int i = 0; i < segs; ++i) {
        int x, y, a, b;
        cin >> x >> y >> a >> b;

        int val = a * binpow(b, modval - 2) % modval;
        int rem = (1 - val + modval) % modval;
        total = total * rem % modval;

        int use = val * binpow(rem, modval - 2) % modval;
        links[y].emplace_back(x, use);
    }

    vector<int> dp(size + 1);
    dp[0] = 1;

    for (int j = 1; j <= size; ++j) {
        int collect = 0;
        for (auto &[start, weight] : links[j]) {
            collect = (collect + dp[start - 1] * weight) % modval;
        }
        dp[j] = collect;
    }

    cout << dp[size] * total % modval << '\n';
    return 0;
}
