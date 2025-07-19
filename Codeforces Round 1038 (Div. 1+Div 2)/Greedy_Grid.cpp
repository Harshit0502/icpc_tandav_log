#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        int n, m;
        cin >> n >> m;
        // NO if there's only one row/col, or if it's exactly 2×2
        if (n == 1 || m == 1 || (n == 2 && m == 2))
            cout << "NO\n";
        else
            cout << "YES\n";
    }
    return 0;
}
