#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while(T--){
        int n, k;
        cin >> n >> k;
        vector<long long> h(n);
        for(int i = 0; i < n; i++){
            cin >> h[i];
        }
        // your starting tower height:
        long long cur = h[k-1];

        // build a sorted, unique list of all tower heights
        vector<long long> heights = h;
        sort(heights.begin(), heights.end());
        heights.erase(unique(heights.begin(), heights.end()), heights.end());

        long long Hmax = heights.back();
        if(cur == Hmax){
            // already on a max‐height tower
            cout << "YES\n";
            continue;
        }

        long long t = 0;       // elapsed time
        bool ok = false;

        // repeatedly try to hop to the tallest reachable next height
        while(cur < Hmax){
            // find first height strictly greater than cur
            auto it0 = upper_bound(heights.begin(), heights.end(), cur);
            if(it0 == heights.end()) break;

            // the farthest we can reach satisfies:
            //   hh - cur <= cur - t
            // => hh <= 2*cur - t
            long long limit = 2*cur - t;
            // find rightmost height <= limit
            auto it1 = upper_bound(it0, heights.end(), limit);
            if(it1 == it0){
                // no next height in (cur, limit]
                break;
            }
            --it1;
            long long nxt = *it1;

            // perform that jump
            long long dt = nxt - cur;  // travel time
            t += dt;
            cur = nxt;

            if(cur == Hmax){
                ok = true;
                break;
            }
        }

        cout << (ok ? "YES\n" : "NO\n");
    }
    return 0;
}
