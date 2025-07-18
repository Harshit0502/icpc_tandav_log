#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Casino {
    int l, r;
    long long real;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        long long k;
        cin >> n >> k;

        vector<Casino> a(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i].l >> a[i].r >> a[i].real;
        }

        // To track if a casino has been visited
        vector<bool> used(n, false);

        // Always try the best reachable casino next
        bool found = true;
        while (found) {
            found = false;
            long long best_gain = k;
            int best_idx = -1;

            for (int i = 0; i < n; ++i) {
                if (!used[i] && a[i].l <= k && k <= a[i].r) {
                    if (a[i].real > best_gain) {
                        best_gain = a[i].real;
                        best_idx = i;
                        found = true;
                    }
                }
            }

            if (found) {
                used[best_idx] = true;
                k = best_gain;
            }
        }

        cout << k << '\n';
    }

    return 0;
}

