#include <bits/stdc++.h>
#include "input.cpp"
using namespace std;

/*
Dimensions:
L: number of news items == number of nodes
D: number of features
U: number of users
level: L-dimensional array of levels. indicates level of each news item
index: L+1-dimensional array of indices
indptr: index[L]-dimensional array of indices
level_offset: max_level-dimensional array of offsets
vect: LxD-dimension: represents embedding of each news item
user: UxD-dimensional array of user features
*/

int main()
{
    // read file max_level.txt to get max_level
    int max_level, ep, L = 6, D = 5, U = 5;
    int *level, *index, *indptr, *level_offset, **vect, **user;
    string inputDir = "./anz_dd/";
    getinputs(inputDir, max_level, ep, level, index, indptr, level_offset, vect, user, L, D, U);
    printinputs(max_level, ep, level, index, indptr, level_offset, vect, user, L, D, U);
}