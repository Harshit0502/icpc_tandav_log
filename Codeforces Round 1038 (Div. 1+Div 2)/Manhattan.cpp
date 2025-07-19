#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Point {
    int x, y, idx;
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;
        vector<Point> pts(n);
        for (int i = 0; i < n; i++) {
            cin >> pts[i].x >> pts[i].y;
            pts[i].idx = i + 1;
        }

        // ----- Edge case: only one possible pair -----
        if (n == 2) {
            cout << pts[0].idx << ' ' << pts[1].idx << "\n";
            continue;
        }

        ll bestSum = -1;
        vector<pair<int,int>> bestPairs;

        // The 4 sign‐pairs (sx,sy) to project (x,y) → sx*x + sy*y
        static const array<pair<int,int>,4> signs = {{
            {+1,+1}, {+1,-1}, {-1,+1}, {-1,-1}
        }};

        for (auto [sx, sy] : signs) {
            // Build (key, original_index) array
            vector<pair<ll,int>> proj(n);
            for (int i = 0; i < n; i++) {
                proj[i].first  = ll(sx)*pts[i].x + ll(sy)*pts[i].y;
                proj[i].second = i;
            }
            sort(proj.begin(), proj.end(),
                 [](auto &a, auto &b){ return a.first < b.first; });

            ll sum = 0;
            vector<pair<int,int>> pairs;
            pairs.reserve(n/2);
            // Pair the smallest with the largest, etc.
            for (int i = 0; i < n/2; i++) {
                int L = proj[i].second;
                int R = proj[n-1-i].second;
                sum += llabs(pts[L].x - pts[R].x)
                     + llabs(pts[L].y - pts[R].y);
                pairs.emplace_back(pts[L].idx, pts[R].idx);
            }

            if (sum > bestSum) {
                bestSum   = sum;
                bestPairs = move(pairs);
            }
        }

        // Output the chosen pairing
        for (auto &pr : bestPairs) {
            cout << pr.first << ' ' << pr.second << "\n";
        }
    }

    return 0;
}
