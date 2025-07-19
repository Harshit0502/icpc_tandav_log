// Problem D: Traffic Lights — Cascading solution with specialized branches
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pll = pair<ll,ll>;

// Dispatch thresholds
const int PLAIN_DEG_LIMIT      = 1;
const int LAYERED_N_LIMIT      = 2000;
const int LAYERED_DEG_LIMIT    = 20;
const ll  LAYERED_EST_T_LIMIT  = 50000;
const int ASTAR_DEG_LIMIT      = 50;
const int TWO_HOP_LIMIT        = 1000;

// Case 1: Pure BFS for chains (deg ≤ 1)
void bfs_plain(int n, const vector<vector<int>>& adj) {
    vector<int> dist(n+1, -1);
    queue<int> q;
    dist[1] = 0;
    q.push(1);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    cout << dist[n] << " " << 0 << '\n';
}

// Case 2: Time-layered BFS
void solve_layered(int n, const vector<vector<int>>& adj) {
    const int INF = INT_MAX;
    vector<int> wait(n+1, INF), next_wait;
    wait[1] = 0;
    vector<int> curr = {1}, next_front;
    int T = 0;
    while (true) {
        next_wait.assign(n+1, INF);
        next_front.clear();
        for (int u : curr) {
            // WAIT
            if (wait[u] + 1 < next_wait[u]) {
                next_wait[u] = wait[u] + 1;
                next_front.push_back(u);
            }
            // MOVE
            int Du = adj[u].size();
            if (Du > 0) {
                int k = T % Du;
                int v = adj[u][k];
                if (wait[u] < next_wait[v]) {
                    next_wait[v] = wait[u];
                    next_front.push_back(v);
                }
            }
        }
        ++T;
        curr.swap(next_front);
        wait.swap(next_wait);
        if (wait[n] != INF) {
            cout << T << " " << wait[n] << '\n';
            return;
        }
    }
}

// Case 3: A* search on expanded (u,r)
void solve_astar(int n, const vector<vector<int>>& adj) {
    // heuristic: unweighted dist to n
    vector<int> h(n+1, -1);
    queue<int> q;
    h[n] = 0; q.push(n);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) if (h[v] == -1) {
            h[v] = h[u] + 1;
            q.push(v);
        }
    }
    vector<vector<pll>> dist(n+1);
    for (int u = 1; u <= n; ++u)
        dist[u].assign(adj[u].size(), {LLONG_MAX, LLONG_MAX});
    using State = tuple<ll,ll,ll,int,int>;
    priority_queue<State, vector<State>, greater<State>> pq;
    dist[1][0] = {0,0};
    pq.emplace(h[1], 0LL, 0LL, 1, 0);
    while (!pq.empty()) {
        auto [f, T, W, u, r] = pq.top(); pq.pop();
        if (u == n && dist[u][r] == make_pair(T,W)) {
            cout << T << " " << W << '\n';
            return;
        }
        if (r < 0 || r >= (int)dist[u].size() || dist[u][r] != make_pair(T,W)) continue;
        int Du = adj[u].size();
        for (int k = 0; k < Du; ++k) {
            int v = adj[u][k];
            int dw = (k - r + Du) % Du;
            ll T2 = T + dw + 1;
            ll W2 = W + dw;
            int r2 = adj[v].empty() ? 0 : T2 % adj[v].size();
            if (r2 < (int)dist[v].size() && make_pair(T2,W2) < dist[v][r2]) {
                dist[v][r2] = {T2,W2};
                ll f2 = T2 + (h[v] < 0 ? 0 : h[v]);
                pq.emplace(f2, T2, W2, v, r2);
            }
        }
    }
}

// Case 4: Lexicographic Dijkstra on expanded (u,r)
void solve_lex_dijkstra(int n, const vector<vector<int>>& adj) {
    vector<vector<pll>> dist(n+1);
    for (int u = 1; u <= n; ++u)
        dist[u].assign(adj[u].size(), {LLONG_MAX, LLONG_MAX});
    using State = tuple<ll,ll,int,int>;
    priority_queue<State, vector<State>, greater<State>> pq;
    dist[1][0] = {0,0};
    pq.emplace(0,0,1,0);
    while (!pq.empty()) {
        auto [T, W, u, r] = pq.top(); pq.pop();
        if (dist[u][r] != make_pair(T,W)) continue;
        if (u == n) {
            cout << T << " " << W << '\n';
            return;
        }
        int Du = adj[u].size();
        for (int k = 0; k < Du; ++k) {
            int v = adj[u][k];
            int dw = (k - r + Du) % Du;
            ll T2 = T + dw + 1;
            ll W2 = W + dw;
            int r2 = adj[v].empty() ? 0 : T2 % adj[v].size();
            if (r2 < (int)dist[v].size() && make_pair(T2,W2) < dist[v][r2]) {
                dist[v][r2] = {T2,W2};
                pq.emplace(T2,W2,v,r2);
            }
        }
    }
}

// --- Original simple checker (chain vs lex-Dijkstra) ---
// void solve_case(int n, int m, const vector<vector<int>>& adj) {
//     // 1) Compute maximum degree
//     int max_deg = 0;
//     for (int u = 1; u <= n; ++u)
//         max_deg = max(max_deg, (int)adj[u].size());
//     
//     // 2) If it's essentially a chain (all middle nodes deg ≤ 1), do plain BFS
//     bool is_chain = true;
//     for (int u = 2; u < n; ++u) {
//         if (adj[u].size() > 1) {
//             is_chain = false;
//             break;
//         }
//     }
//     if (is_chain) {
//         bfs_plain(n, adj);
//         return;
//     }
//
//     // 3) Otherwise, run the fully-general lexicographic Dijkstra
//     solve_lex_dijkstra(n, adj);
// }

// Checker with cascading cases
void solve_case(int n, int m, const vector<vector<int>>& adj) {
    int max_deg = 0;
    for (int u = 1; u <= n; ++u)
        max_deg = max(max_deg, (int)adj[u].size());
    // Case 1: chain
    bool is_chain = true;
    for (int u = 2; u < n; ++u)
        if (adj[u].size() > PLAIN_DEG_LIMIT) { is_chain = false; break; }
    if (is_chain) { bfs_plain(n,adj); return; }
    // BFS dist
    vector<int> d(n+1, -1);
    queue<int> qq;
    d[1]=0; qq.push(1);
    while(!qq.empty()){
        int u=qq.front(); qq.pop();
        for(int v:adj[u]) if(d[v]==-1){ d[v]=d[u]+1; qq.push(v); }
    }
    int graph_dist = (d[n]<0?INT_MAX:d[n]);
    // Case 2: direct edge
    int deg1 = adj[1].size();
    for(int i=0;i<deg1;i++){
        if(adj[1][i]==n){
            int dw = (i+deg1)%deg1;
            cout<<dw+1<<" "<<dw<<"\n";
            return;
        }
    }
    // Case 3: two-hop
    if(deg1<=TWO_HOP_LIMIT){
        pll best={LLONG_MAX,LLONG_MAX};
        for(int i=0;i<deg1;i++){
            int v=adj[1][i];
            int dw1=i%deg1;
            ll T1=dw1+1, W1=dw1;
            int dv=adj[v].size();
            int r1=dv?T1%dv:0;
            for(int j=0;j<dv;j++){
                if(adj[v][j]==n){
                    int dw2=(j-r1+dv)%dv;
                    ll T2=T1+dw2+1, W2=W1+dw2;
                    best=min(best,make_pair(T2,W2));
                }
            }
        }
        if(best.first<LLONG_MAX){ cout<<best.first<<" "<<best.second<<"\n"; return; }
    }
    // Case 4: three-hop dist==3
    if(graph_dist==3 && (ll)deg1*max_deg*max_deg<=10000000){
        pll best3={LLONG_MAX,LLONG_MAX};
        for(int i=0;i<deg1;i++){
            int v=adj[1][i];
            int dw1=i%deg1;
            ll T1=dw1+1, W1=dw1;
            int dv=adj[v].size();
            int r1=dv?T1%dv:0;
            for(int j=0;j<dv;j++){
                int w=adj[v][j];
                int dw2=(j-r1+dv)%dv;
                ll T2=T1+dw2+1, W2=W1+dw2;
                int dw_deg=adj[w].size();
                int r2=dw_deg?T2%dw_deg:0;
                for(int k=0;k<dw_deg;k++){
                    if(adj[w][k]==n){
                        int dw3=(k-r2+dw_deg)%dw_deg;
                        ll T3=T2+dw3+1, W3=W2+dw3;
                        best3=min(best3,make_pair(T3,W3));
                    }
                }
            }
        }
        if(best3.first<LLONG_MAX){ cout<<best3.first<<" "<<best3.second<<"\n"; return; }
    }
    // Fallback branches
    ll est_T = (ll)graph_dist * max_deg;
    if(n<=LAYERED_N_LIMIT && max_deg<=LAYERED_DEG_LIMIT && est_T<=LAYERED_EST_T_LIMIT)
        solve_layered(n,adj);
    else if(max_deg<=ASTAR_DEG_LIMIT)
        solve_astar(n,adj);
    else
        solve_lex_dijkstra(n,adj);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int TC; cin>>TC;
    while(TC--){
        int n,m; cin>>n>>m;
        vector<vector<int>> adj(n+1);
        for(int i=0,u,v;i<m;i++){cin>>u>>v; adj[u].push_back(v); adj[v].push_back(u);}        
       solve_case(n,m,adj);
        }
    return 0;
}
