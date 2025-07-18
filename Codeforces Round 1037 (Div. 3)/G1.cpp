#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// maximum n across all test cases is 2e5
static int a_arr[200000];
static int S[200001], pref_min[200001], suf_max[200001], t_arr[200000];

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while(T--){
        int n;
        cin >> n;
        for(int i = 0; i < n; i++){
            cin >> a_arr[i];
        }

        // build sorted unique values (<=100 distinct)
        vector<int> V(a_arr, a_arr + n);
        sort(V.begin(), V.end());
        V.erase(unique(V.begin(), V.end()), V.end());
        int D = V.size();

        int answer = 0;

        // check if there exists a subarray with min=m_val and med>=M_val
        auto can = [&](int m_val, int M_val)->bool{
            int idx = 0;
            while(idx < n){
                // skip positions where a[idx]<m_val (barriers)
                while(idx < n && a_arr[idx] < m_val) 
                    idx++;
                if(idx >= n) break;

                // collect one segment [start..idx-1]
                int len = 0;
                vector<int> zeros; // positions in segment where a==m_val
                while(idx < n && a_arr[idx] >= m_val){
                    int t = (a_arr[idx] >= M_val ? +1 : -1);
                    t_arr[len] = t;
                    if(a_arr[idx] == m_val)
                        zeros.push_back(len);
                    len++;
                    idx++;
                }

                if(zeros.empty()) 
                    continue;

                // build prefix sums S[0..len]
                S[0] = 0;
                for(int i = 0; i < len; i++){
                    S[i+1] = S[i] + t_arr[i];
                }
                // build pref_min
                pref_min[0] = S[0];
                for(int i = 1; i <= len; i++){
                    pref_min[i] = min(pref_min[i-1], S[i]);
                }
                // build suf_max
                suf_max[len] = S[len];
                for(int i = len-1; i >= 0; i--){
                    suf_max[i] = max(suf_max[i+1], S[i]);
                }

                // now test each zero‑position j
                // zero at segment‑index j corresponds to S‑indices j and j+1
                for(int j0 : zeros){
                    // we need max_{r>=j0}S[r] - min_{0<=l<=j0}S[l] >= 0
                    // which is suf_max[j0+1] - pref_min[j0] >= 0
                    if(suf_max[j0+1] - pref_min[j0] >= 0)
                        return true;
                }
            }
            return false;
        };

        // for every possible min m = V[mi], binary‐search the best median > m
        for(int mi = 0; mi < D; mi++){
            int m_val = V[mi];
            int lo = mi+1, hi = D-1, best = -1;
            while(lo <= hi){
                int mid = (lo + hi) >> 1;
                if(can(m_val, V[mid])){
                    best = mid;
                    lo = mid + 1;
                } else {
                    hi = mid - 1;
                }
            }
            if(best != -1){
                answer = max(answer, V[best] - m_val);
            }
        }

        cout << answer << "\n";
    }
}
