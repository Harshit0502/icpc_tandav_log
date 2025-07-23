#include <bits/stdc++.h>
using namespace std;
using ll = long long;

static const ll DP_LIMIT = 30000000;
static const ll INF      = (ll)4e15;
static const int K       = 6;

// --- Exact DP to pick exactly k new windows (skip originals) ---
ll dpPickK(int P, int k, const vector<ll>& costExt){
    vector<ll> prev(P+1, 0), curr(P+1, INF);
    for(int pick = 1; pick <= k; ++pick){
        curr[0] = INF;
        for(int i = 1; i <= P; ++i){
            int pi = max(0, i - K);
            ll take = prev[pi] + costExt[i];
            curr[i] = min(curr[i-1], take);
        }
        swap(prev, curr);
    }
    return prev[P];
}

// --- Multi‑variant greedy to add k new windows ---
ll multiGreedyAdd(const vector<int>& cost,
                   const vector<char>& isExact,
                   int mu, int t0,
                   const string& s)
{
    int P = cost.size(), N = s.size();
    int need = t0 - mu;
    if(need <= 0) return mu - t0;  // we only break originals

    // mark original‐occupied positions
    vector<char> baseUsed(N,0);
    for(int i=0;i<P;i++){
        if(isExact[i]){
            for(int x=i; x<i+K; x++) baseUsed[x] = 1;
        }
    }

    // collect candidates
    vector<pair<int,int>> cand;
    cand.reserve(P);
    for(int i=0;i<P;i++){
        if(isExact[i]) continue;
        bool ok=true;
        for(int x=i;x<i+K;x++){
            if(baseUsed[x]){ ok=false; break; }
        }
        if(ok) cand.emplace_back(cost[i], i);
    }

    ll best = INF;

    // 4 tie‐break variants: start↑, start↓, end↑, end↓
    for(int pass=0; pass<4; ++pass){
        auto used = baseUsed;
        auto order = cand;
        switch(pass){
            case 0: // cost, start asc
                sort(order.begin(), order.end(),
                     [&](auto &a, auto &b){
                         if(a.first!=b.first) return a.first<b.first;
                         return a.second < b.second;
                     }); break;
            case 1: // cost, start desc
                sort(order.begin(), order.end(),
                     [&](auto &a, auto &b){
                         if(a.first!=b.first) return a.first<b.first;
                         return a.second > b.second;
                     }); break;
            case 2: // cost, end asc
                sort(order.begin(), order.end(),
                     [&](auto &a, auto &b){
                         if(a.first!=b.first) return a.first<b.first;
                         return (a.second+K) < (b.second+K);
                     }); break;
            default:// cost, end desc
                sort(order.begin(), order.end(),
                     [&](auto &a, auto &b){
                         if(a.first!=b.first) return a.first<b.first;
                         return (a.second+K) > (b.second+K);
                     }); break;
        }

        ll sumC = 0;
        int got = 0;
        for(auto &pr : order){
            if(got==need) break;
            int i = pr.second;
            bool ok=true;
            for(int x=i; x<i+K; x++){
                if(used[x]){ ok=false; break; }
            }
            if(!ok) continue;
            for(int x=i; x<i+K; x++){
                used[x] = 1;
            }
            sumC += pr.first;
            ++got;
        }
        if(got==need) best = min(best, sumC);
    }

    return best;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T; 
    cin >> T;
    while(T--){
        string s; cin >> s;
        int N = s.size();

        int n; 
        cin >> n;
        vector<pair<ll,ll>> ranges(n);
        for(int i=0;i<n;i++){
            cin >> ranges[i].first >> ranges[i].second;
        }

        const string PAT = "docker";

        // 1) Precompute P, OccMax, costs, and mu
        int P = max(0, N - K + 1);
        int OccMax = N / K;

        vector<int> cost(P);
        vector<char> isExact(P,0);
        int mu = 0;
        for(int i=0;i<P;i++){
            int d=0;
            for(int j=0;j<K;j++){
                if(s[i+j]!=PAT[j]) ++d;
            }
            cost[i] = d;
            if(d==0){ isExact[i]=1; ++mu; }
        }

        // 2) Build coverage array f[t]
        vector<ll> diff(OccMax+2,0), cov(OccMax+1,0);
        for(auto &rg : ranges){
            ll l = max(0LL, rg.first);
            ll r = min((ll)OccMax, rg.second);
            if(l<=r){
                diff[l]   += 1;
                diff[r+1] -= 1;
            }
        }
        ll run=0;
        for(int t=0;t<=OccMax;t++){
            run += diff[t];
            cov[t] = run;
        }

        // 3) Pick t0 maximizing cov[t], then minimizing |t-mu|
        ll fmax = *max_element(cov.begin(), cov.end());
        int t0=0; ll bestDist=LLONG_MAX;
        for(int t=0;t<=OccMax;t++){
            if(cov[t]!=fmax) continue;
            ll d = llabs((ll)t - mu);
            if(d<bestDist || (d==bestDist && t<t0)){
                bestDist = d;
                t0 = t;
            }
        }

        // 4) Compute answer
        ll answer;
        if(t0 < mu){
            // break (mu - t0) originals
            answer = mu - t0;
        } else {
            int need = t0 - mu;
            // build costExt for DP: INF for originals, else cost[i]
            vector<ll> costExt(P+1, INF);
            for(int i=1;i<=P;i++){
                if(!isExact[i-1]) 
                    costExt[i] = cost[i-1];
            }
            ll comp = (ll)P * need;

            if(need>0 && comp <= DP_LIMIT){
                // exact DP on the k new windows
                answer = dpPickK(P, need, costExt);
            } else {
                // four‐variant greedy fallback
                answer = multiGreedyAdd(cost, isExact, mu, t0, s);
            }
            // safety net: break all originals if still INF
            if(answer >= INF/2) answer = mu;
        }

        cout << answer << "\n";
    }
    return 0;
}
