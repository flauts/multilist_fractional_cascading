#include <bits/stdc++.h>
using namespace std;
using vi = vector<int>;
using pi = pair<int,int>;
using ll = long long;
ll binpow(ll a, ll b) {
    ll res = 1;
    while (b > 0) {
        if (b & 1)
            res = res * a;
        a = a * a;
        b >>= 1;
    }
    return res;
}

struct Node {
    int key;
    int curr_idx;  //index on L[i]
    int down_idx;  //index on M[i+1]
    
    Node(int k) : key(k), curr_idx(-1), down_idx(-1) {}
};

bool search_multilist(int key, const vector<vector<int>>& L, const vector<vector<Node>>& M) {
    if (M.empty()) return false;
    
    int low = 0, high = M[0].size() - 1;
    int idx = -1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (M[0][mid].key >= key) {
            idx = mid;
            high = mid - 1; 
        } else {
            low = mid + 1;
        }
    }

    if (idx == -1) return false;

    for (int i = 0; i < M.size(); ++i) {
        int c_idx = M[i][idx].curr_idx;
        
        if (c_idx < L[i].size() && L[i][c_idx] == key) {
            return true;
        }

        if (i < M.size() - 1) {
            idx = M[i][idx].down_idx;
            if (idx > 0 && M[i+1][idx - 1].key >= key) { 
                idx = idx - 1; 
            }
        }
    }
    return false;
}

void build_fractional_cascade(const vector<vector<int>>& L, vector<vector<Node>>& M) {
    int k = L.size();
    if (k == 0) return;

    M.resize(k);

    for (int i = 0; i < L[k - 1].size(); ++i) {
        Node n(L[k - 1][i]);
        n.curr_idx = i;
        n.down_idx = -1;
        M[k - 1].push_back(n); //fill last layer
    }

    for (int i = k - 2; i >= 0; --i) { //start from second to last
        int j = 0, q = 1; // q starts at 1 to grab the 2nd, 4th, 6th element.
        
        while(j < L[i].size() && q < M[i+1].size()) { //merge sort
            if (L[i][j] <= M[i+1][q].key) {
                Node n(L[i][j]);
                n.curr_idx = j;      
                n.down_idx = -1;     
                M[i].push_back(n);
                ++j; //all remain
            } else {
                Node n(M[i+1][q].key);
                n.curr_idx = -1;     
                n.down_idx = q;      
                M[i].push_back(n);
                q += 2; //put one, leave 1
            }
        }        

        while (j < L[i].size()) { //fill with all that remain
            Node n(L[i][j]);
            n.curr_idx = j;
            n.down_idx = -1;
            M[i].push_back(n);
            ++j;
        }
        //only 1 of these executes 
        while (q < M[i+1].size()) {
            Node n(M[i+1][q].key);
            n.curr_idx = -1;
            n.down_idx = q;
            M[i].push_back(n);
            q += 2;
        }
        int last_curr = L[i].size(); //out of bounds     
        int last_down = M[i+1].size();   
        //we need to put the idx of the items to
        //travel this structure, the nearest one is assigned
        //by default
        
        for (int p = M[i].size() - 1; p >= 0; --p) {
            if (M[i][p].curr_idx != -1) {
             last_curr = M[i][p].curr_idx;
            } else {
                M[i][p].curr_idx = last_curr;
            }
            
            if (M[i][p].down_idx != -1) {
                last_down = M[i][p].down_idx;
            } else {
                M[i][p].down_idx = last_down;
            }
        }
    }

}


int main() {
    vector<vector<int>> L = {
        {10, 20, 30, 40},
        {15, 25, 35},
        {12, 18, 24, 30, 45}
    };

    for (int i = 0; i < L.size(); ++i) {
        cout << "L[" << i << "]: ";
        for (int val : L[i]) cout << val << " ";
        cout << "\n";
    }

    vector<vector<Node>> M;
    build_fractional_cascade(L, M);
    cout<<"Found 12:" << search_multilist(12, L,M);

    return 0;
}