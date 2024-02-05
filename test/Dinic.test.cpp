#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/GRL_6_A"
#include <bits/stdc++.h>
#include "gandalfr/graph/FlowGraph.hpp"
using namespace std;
using ll = long long;
#define rep(i, j, n) for(ll i = (ll)(j); i < (ll)(n); i++)
#define all(a) (a).begin(),(a).end()

using namespace gandalfr;

int main(void){

    int N, M;
    cin >> N >> M;
    FlowGraph G(N, M);
    rep(i,0,M) {
        int a, b, c;
        cin >> a >> b >> c;
        G.addEdge(a, b, c);
    }

    std::cout << G.dinic(0, N - 1) << std::endl;

}
