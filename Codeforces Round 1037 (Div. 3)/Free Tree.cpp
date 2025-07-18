#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    const int MAX = 200000;
    const int B   = 450;  // ≃ sqrt(MAX)

    while (T--) {
        int n, q;
        cin >> n >> q;

        vector<int> color(n+1);
        for (int i = 1; i <= n; i++) 
            cin >> color[i];

        vector<vector<pair<int,ll>>> adj(n+1);
        for (int i = 0; i < n-1; i++){
            int u, v;
            ll w;
            cin >> u >> v >> w;
            adj[u].push_back({v,w});
            adj[v].push_back({u,w});
        }

        // Identify heavy vertices
        vector<bool> isHeavy(n+1,false);
        for(int i=1;i<=n;i++)
            if ((int)adj[i].size() > B)
                isHeavy[i] = true;

        // For each heavy v: map<color, sumWeights>
        vector<unordered_map<int,ll>> heavyMap(n+1);
        heavyMap.reserve(n+1);
        for(int v=1; v<=n; v++){
            if(!isHeavy[v]) continue;
            auto &M = heavyMap[v];
            M.reserve(adj[v].size());
            for(auto &e: adj[v]){
                int u = e.first;
                ll w  = e.second;
                M[color[u]] += w;
            }
        }

        // For each v: list of heavy neighbors (with weights)
        vector<vector<pair<int,ll>>> heavyNbr(n+1);
        for(int v=1; v<=n; v++){
            for(auto &e: adj[v]){
                int u = e.first;
                ll w  = e.second;
                if(isHeavy[u])
                    heavyNbr[v].push_back({u,w});
            }
        }

        // Compute initial total cost
        ll total = 0;
        vector<bool> seen(n+1,false);
        for(int v=1; v<=n; v++){
            for(auto &e: adj[v]){
                int u = e.first;
                ll w  = e.second;
                if(!seen[u] && color[u]!=color[v])
                    total += w;
            }
            seen[v]=true;
        }

        // Process queries
        while(q--){
            int v, newCol;
            cin >> v >> newCol;
            int oldCol = color[v];
            if(oldCol != newCol){
                ll delta = 0;

                if(isHeavy[v]){
                    // O(1) difference from the map
                    auto &M = heavyMap[v];
                    delta = M[oldCol] - M[newCol];
                } else {
                    // scan its ≤ B edges
                    ll sOld=0, sNew=0;
                    for(auto &e: adj[v]){
                        int u = e.first; ll w = e.second;
                        if(color[u]==oldCol) sOld += w;
                        if(color[u]==newCol) sNew += w;
                    }
                    delta = sOld - sNew;
                }

                // apply cost change
                total += delta;

                // update heavy‐neighbor maps
                for(auto &e: heavyNbr[v]){
                    int u = e.first; ll w = e.second;
                    auto &M = heavyMap[u];
                    M[oldCol] -= w;
                    M[newCol] += w;
                }

                color[v] = newCol;
            }

            cout << total << "\n";
        }
    }

    return 0;
}
