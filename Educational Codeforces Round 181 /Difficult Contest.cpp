#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        string s;
        cin >> s;
        array<int,26> freq{};
        for (char c : s) {
            freq[c - 'A']++;
        }

        string ans;
        ans.reserve(s.size());

        // 1) output all "other" letters (not F, N, T) in lex order
        for (char c = 'A'; c <= 'Z'; c++) {
            if (c=='F' || c=='N' || c=='T') continue;
            int cnt = freq[c - 'A'];
            if (cnt > 0) {
                ans.append(cnt, c);
                freq[c - 'A'] = 0;
            }
        }

        int f = freq['F' - 'A'];
        int n = freq['N' - 'A'];
        int t = freq['T' - 'A'];

        // 2) handle the special F=0 case (only N and T)
        if (f == 0) {
            // place all T first, then all N
            if (t > 0) {
                ans.append(t, 'T');
                freq['T' - 'A'] = 0;
            }
            if (n > 0) {
                ans.append(n, 'N');
                freq['N' - 'A'] = 0;
            }
        } else {
            // 3) build the FT‐block
            string ft;
            ft.reserve(f + t + 2);
            if (t >= f) {
                // T F repeated f times, then leftover Ts
                for (int i = 0; i < f; i++) {
                    ft.push_back('T');
                    ft.push_back('F');
                }
                int leftT = t - f;
                if (leftT > 0) {
                    ft.append(leftT, 'T');
                }
            } else {
                // F T repeated t times, then leftover Fs
                for (int i = 0; i < t; i++) {
                    ft.push_back('F');
                    ft.push_back('T');
                }
                int leftF = f - t;
                if (leftF > 0) {
                    ft.append(leftF, 'F');
                }
            }
            // 4) prepend all Ns before the FT‐block
            if (n > 0) {
                ans.append(n, 'N');
            }
            // 5) append the FT‐block
            ans += ft;
            freq['F' - 'A'] = freq['N' - 'A'] = freq['T' - 'A'] = 0;
        }

        // Sanity: all freq should now be zero
        // (we've appended every character exactly once)
        cout << ans << "\n";
    }

    return 0;
}
