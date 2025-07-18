#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        int n, k;
        cin >> n >> k;
        vector<int> a(n);
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }

        // build prefix sums: p[i] = sum of a[0..i-1]
        vector<int> p(n+1, 0);
        for (int i = 0; i < n; i++) {
            p[i+1] = p[i] + a[i];
        }

        int ans = 0;
        int i = 0;
        // try to start a hike at day i (0-based)
        while (i + k <= n) {
            // if a[i..i+k-1] all zeros
            if (p[i+k] - p[i] == 0) {
                ans++;
                // skip the k days of the hike plus 1 break day
                i += k + 1;
            } else {
                i++;
            }
        }

        cout << ans << "\n";
    }

    return 0;
}
