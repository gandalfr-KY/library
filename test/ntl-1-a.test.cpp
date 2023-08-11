#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/NTL_1_A"
#include <bits/stdc++.h>
#include "../math/prime_number_utility.hpp"
using namespace std;
using ll = long long;
#define rep(i, j, n) for(ll i = (ll)(j); i < (ll)(n); i++)

int main(void){
 
    //input

    int N;
    cin >> N;

    //calculate

    cout << N << ":";
    for(auto p : prime_number_utility::factorize(N)){
        rep(i,0,p.second) cout << " " << p.first;
    }
    cout << endl;

}