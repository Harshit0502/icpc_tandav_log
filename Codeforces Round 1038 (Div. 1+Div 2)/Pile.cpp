#include <bits/stdc++.h>
using namespace std;
using int64 = long long;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T; 
    cin >> T;
    while(T--){
        int n;
        cin >> n;

        int64 zeros_excess = 0;
        int64 ones_excess  = 0;
        int64 blocks       = 0;

        for(int i = 0; i < n; i++){
            int64 a,b,c,d;
            cin >> a >> b >> c >> d;
            // how many 0s and 1s we need to pull out once and for all:
            if(a > c) zeros_excess += (a - c);
            if(b > d) {
                ones_excess += (b - d);
                blocks += min(a, c);
            }
        }

        cout << (zeros_excess + ones_excess + blocks) << "\n";
    }
    return 0;
}
