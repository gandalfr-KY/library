#define PROBLEM "https://judge.yosupo.jp/problem/matrix_det"
#include <bits/stdc++.h>
#include "../math/matrix.hpp"
#include "../standard/mod_integer.hpp"
using namespace std;
using ll = long long;
#define rep(i, j, n) for(ll i = (ll)(j); i < (ll)(n); i++)
#define all(a) (a).begin(),(a).end()

int main(void){

    int N;
    cin >> N;
    matrix<_mint> A(N, N);
    cin >> A;
    cout << A.determinant() << endl;

}
