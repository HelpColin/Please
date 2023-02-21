#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>
#include <unordered_map>

using namespace std;

/* operations for the actual pancake stack */
vector<int> del_move(vector<int> v, int k){
    reverse(v.begin()+k, v.end());
    v.pop_back();
    return v;
}
vector<int> add_move(vector<int> v, int i, int k){
    for(int i = 0; i < v.size(); i ++){
        if(v[i] >= k) v[i] ++;
    }
    v.push_back(k);
    reverse(v.begin()+i, v.end());
    return v;
}
bool is_sorted(vector<int> v){
    for(int i = 1; i < (int)v.size(); i ++){
        if(v[i] > v[i-1]) return false;
    }
    return true;
}


/* Faculty used to get unique indexes */
int fakt[13];
void compute_fakt() {
    fakt[0] = 1;
    for(int i = 1; i < 13; i ++){
        fakt[i] = fakt[i-1] * i;
    }
}
int get_index(vector<int> v){
    int n = (int)v.size();
    int num = 1;
    for(int i = 0; i < n - 1; i++){
        int cnt_smaller = 0;
        for(int j = i + 1; j < n; j ++){
            if(v[j] < v[i]){
                cnt_smaller ++;
            }
        }
        num += cnt_smaller * fakt[n-i-1];
    }
    return num;
}

int count_inversions(vector<int> v){
    int n = (int)v.size();
    int inversions = 0;
    for(int i = 0; i < n; i ++){
        for(int j = i + 1; j < n; j ++){
            if(v[i] > v[j]){
                inversions ++;
            }
        }
    }
    return inversions;
}
/* sets each element on the index if the array was sorted (e.g: 3 7 8 2 -> 2 3 4 1) */
vector<int> compress(vector<int> v){
    int n = (int)v.size();
    vector<pair<int,int> > sov;
    for(int i = 0; i < n; i ++){
        sov.push_back({v[i], i});
    }
    sort(sov.begin(), sov.end());
    for(int i = 0; i < n; i ++){
        v[sov[i].second] = i + 1;
    }
    return v;
}

unordered_map<int, int> dp[20];

void BFS_END_TO_BEGIN(int n){
    queue<vector<int> > Q;
    vector<int> g;
    for(int i = 1; i <= n; i ++){
        g.push_back(i);
        dp[i][get_index(g)] = 1;
        Q.push(g);
    }
    int ans = 0;
    vector<int> worst_vec;
    while(!Q.empty()){
        vector<int> v = Q.front();
        int index = get_index(v);
        Q.pop();
        if((int)v.size() == n){
            if(dp[v.size()][index] - 1 > ans){
                ans = dp[v.size()][index] - 1;
                worst_vec = v;
            }
            continue;
        }
        for(int k = 1; k <= v.size()+1; k ++){
            for(int i = 0; i <= v.size(); i ++){
                vector<int> new_vec = add_move(v, i, k);
                if(dp[new_vec.size()][get_index(new_vec)] == 0){
                    dp[new_vec.size()][get_index(new_vec)] = dp[v.size()][index] + 1;
                    Q.push(new_vec);
                }
            }
        }
    }
}

int BFS_BEGIN_TO_END(vector<int> v, int worst){
    unordered_map<int, bool> visited[20];
    queue<pair<vector<int>, int> > Q;
    Q.push({v, 0});
    visited[v.size()][get_index(v)] = true;
    int ans = (int)v.size();
    while(!Q.empty() && ans > worst){
        vector<int> v = Q.front().first;
        int index = get_index(v);
        
        int d = Q.front().second;
        Q.pop();
        if(dp[v.size()][index] != 0){
            ans = min(ans, dp[v.size()][index] + d - 1);
            continue;
        }
        for(int i = 0; i < (int)v.size(); i ++){
            vector<int> new_vec = compress(del_move(v, i));
            if(!visited[new_vec.size()][get_index(new_vec)]){
                visited[new_vec.size()][get_index(new_vec)] = true;
                Q.push({new_vec, d+1});
            }
        }
    }
    return ans;
}

void solve_for(int n){
    BFS_END_TO_BEGIN(n-1);
    vector<int> v;
    for(int i = 1; i <= n; i ++){
        v.push_back(i);
    }
    int max_moves = 0;
    vector<int> worst_vec;
    
    int half = ((n*(n-1))/2)/2;
    do{
        if(abs(half-count_inversions(v)) > 1) continue;
        int min_moves = BFS_BEGIN_TO_END(v, max_moves);
        if(min_moves > max_moves){
            max_moves = min_moves;
            worst_vec = v;
        }
    }
    while(next_permutation(v.begin(), v.end()));
    cout<<max_moves<<endl;
}

int main(){
    compute_fakt();
    int n;
    cin >> n;
    solve_for(n);
    return 0;
}
