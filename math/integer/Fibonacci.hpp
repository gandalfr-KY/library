#ifndef FIBONACCI
#define FIBONACCI
#include <bits/stdc++.h>
#include "../matrix/matrix.hpp"
#include "../../standard/power.hpp"

long long Fibonacci(long long n){
    if(n == 0) return 0;
    matrix<long long> mat(2, 2, 1);
    mat[1][1] = 0;
    return power(mat, n - 1)[0][0];
}

long long Fibonacci(long long n, long long MOD){
    if(n == 0) return 0;
    matrix<long long> mat(2, 2);
    mat[0][0] = 1; mat[0][1] = 1;
    mat[1][0] = 1; mat[1][1] = 0;
    return power(mat, n - 1, MOD)[0][0];
}

#endif