#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T; 
    cin >> T;
    while(T--){
        int n;
        cin >> n;

        vector<ll> p(n), s(n);
        for(int i = 0; i < n; i++) cin >> p[i];
        for(int i = 0; i < n; i++) cin >> s[i];

        bool ok = true;

        // 1) p must be non‑increasing divisibility: p[i-1] % p[i] == 0
        for(int i = 1; i < n && ok; i++){
            if(p[i-1] % p[i] != 0) 
                ok = false;
        }

        // 2) s must be non‑decreasing divisibility: s[i] % s[i-1] == 0
        for(int i = 1; i < n && ok; i++){
            if(s[i] % s[i-1] != 0)
                ok = false;
        }

        // 3) global‐gcd match: last prefix == first suffix
        if(ok && p[n-1] != s[0]) 
            ok = false;

        // 4) cross‐split GCD must equal the global GCD = p[n-1]
        for(int i = 0; i+1 < n && ok; i++){
            if(std::gcd(p[i], s[i+1]) != p[n-1]){
                ok = false;
            }
        }

        cout << (ok ? "YES\n" : "NO\n");
    }
    return 0;
}
