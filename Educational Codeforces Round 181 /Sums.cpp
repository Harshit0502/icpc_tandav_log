#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

const int MOD = 998244353;
inline int add(int a,int b){ a+=b; if(a>=MOD) a-=MOD; return a; }

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T; cin >> T;
    while (T--) {
        int n, x;
        cin >> n >> x;
        if (n == 1) {
            cout << x << "\n";
            continue;
        }
        else if (n == 2) {
            cout << (ll)x*(x-1)/2 % MOD << "\n";
            continue;
        }
        else if(n>x){
            cout<<0<<endl;
            continue;
        }
        else {
            unordered_set<ull> seen;

    // iterate all subsets of size n
    ull mask = (1ULL<<n) - 1;
    ull limit = 1ULL << x;

    while (mask < limit) {
        // 1) compute sum of elements in subset
        ll s = 0;
        for (ull m = mask; m; m &= m-1)
            s += __builtin_ctzll(m) + 1;

        // 2) build Q_mask
        ull Q = 0;
        for (ull m = mask; m; m &= m-1) {
            int b = __builtin_ctzll(m);
            int qi = s - (b+1);
            if (qi >= 1 && qi <= x)
                Q |= 1ULL << (qi - 1);
        }

        seen.insert(Q);

        // next combination
        ull c = mask & -mask;
        ull r = mask + c;
        mask = (((r ^ mask) >> 2) / c) | r;
    }

    cout << seen.size() << "\n";
        }
    }
    return 0;

}
