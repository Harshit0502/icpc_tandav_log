#include <bits/stdc++.h>
using namespace std;

#define int long long

int count_good_up_to(int val, const vector<int>& pre, int mod_val, int block_count) {
    if (val <= 0) return 0;
    int full_blocks = val / mod_val;
    int partial = val % mod_val;
    return full_blocks * block_count + pre[partial];
}

int32_t main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    const int base = 210;
    vector<int> coprime_prefix(base + 1, 0);
    for (int i = 1; i <= base; ++i) {
        coprime_prefix[i] = coprime_prefix[i - 1] + (__gcd(i, base) == 1);
    }
    int coprime_per_block = coprime_prefix[base];

    int queries;
    cin >> queries;
    while (queries--) {
        int left, right;
        cin >> left >> right;
        int res = count_good_up_to(right, coprime_prefix, base, coprime_per_block)
                - count_good_up_to(left - 1, coprime_prefix, base, coprime_per_block);
        cout << res << '\n';
    }

    return 0;
}
