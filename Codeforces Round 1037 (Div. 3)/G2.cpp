#include <bits/stdc++.h>
using namespace std;
#define all(a) (a).begin(), (a).end()
#define pb push_back
typedef long long ll;
const int INF = INT_MAX/2;
struct Node { int sum, min_pref, max_pref; };
int n;
vector<int> a;
vector<vector<int>> by_value;
vector<Node> st;
Node merge(const Node &L, const Node &R){
    return {L.sum+R.sum, min(L.min_pref, L.sum+R.min_pref),
                     max(L.max_pref, L.sum+R.max_pref)};
}
void build(int p,int l,int r){
    if(l==r){
        st[p].sum = -1;
        st[p].min_pref = -1;
        st[p].max_pref = -1;
        return;
    }
    int m=(l+r)>>1;
    build(p<<1,l,m);
    build(p<<1|1,m+1,r);
    st[p]=merge(st[p<<1],st[p<<1|1]);
}
void update(int p,int l,int r,int i,int v){
    if(l==r){
        st[p].sum+=v;
        st[p].min_pref=st[p].sum;
        st[p].max_pref=st[p].sum;
        return;
    }
    int m=(l+r)>>1;
    if(i<=m) update(p<<1,l,m,i,v);
    else     update(p<<1|1,m+1,r,i,v);
    st[p]=merge(st[p<<1],st[p<<1|1]);
}
Node query(int p,int l,int r,int i,int j){
    if(i>r||j<l) return {0,INF,-INF};
    if(i<=l&&r<=j) return st[p];
    int m=(l+r)>>1;
    if(j<=m) return query(p<<1,l,m,i,j);
    if(i>m) return query(p<<1|1,m+1,r,i,j);
    Node L=query(p<<1,l,m,i,j), R=query(p<<1|1,m+1,r,i,j);
    return merge(L,R);
}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int T; cin>>T;
    while(T--){
        cin>>n;
        a.assign(n+1,0);
        int MAXV=0;
        for(int i=1;i<=n;i++){
            cin>>a[i];
            MAXV=max(MAXV,a[i]);
        }
        by_value.assign(MAXV+2, vector<int>());
        for(int i=1;i<=n;i++) by_value[a[i]].pb(i);
        vector<int> low(n+1), high(n+1);
        for(int i=1;i<=n;i++){
            low[i]=1; high[i]=MAXV;
        }
        vector<int> to_search;
        to_search.reserve(n);
        for(int i=1;i<=n;i++) to_search.pb(i);
        vector<vector<int>> buckets(MAXV+2);
        vector<char> mid_mark(MAXV+2);
        vector<int> used_mids;
        st.assign(4*(n+1), Node());
        while(!to_search.empty()){
            used_mids.clear();
            for(int j: to_search){
                if(low[j]<high[j]){
                    int mid=(low[j]+high[j]+1)>>1;
                    buckets[mid].pb(j);
                    if(!mid_mark[mid]){
                        mid_mark[mid]=1;
                        used_mids.pb(mid);
                    }
                }
            }
            if(used_mids.empty()) break;
            build(1,1,n);
            int cur_m=MAXV+1;
            for(int m=MAXV;m>=1;m--){
                while(cur_m>m){
                    cur_m--;
                    for(int pos: by_value[cur_m])
                        update(1,1,n,pos,2);
                }
                if(mid_mark[m]){
                    for(int j: buckets[m]){
                        Node L,R;
                        if(j>1) L=query(1,1,n,1,j-1);
                        else     L={0,0,0};
                        R=query(1,1,n,j,n);
                        int Pjm1=L.sum;
                        int Lmin=min(0,L.min_pref);
                        int Rmax=Pjm1+R.max_pref;
                        if(Rmax>=Lmin) low[j]=m;
                        else           high[j]=m-1;
                    }
                }
            }
            for(int mid: used_mids){
                buckets[mid].clear();
                mid_mark[mid]=0;
            }
            vector<int> nxt;
            nxt.reserve(to_search.size());
            for(int j: to_search)
                if(low[j]<high[j])
                    nxt.pb(j);
            to_search.swap(nxt);
        }
        ll ans=0;
        for(int i=1;i<=n;i++)
            ans = max(ans, (ll)low[i]-a[i]);
        cout<<ans<<"\n";
    }
    return 0;
}
