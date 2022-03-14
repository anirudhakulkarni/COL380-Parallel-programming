#pragma once
#include "graph.h"
#include <cmath>
#include <queue>
#include <mpi.h>
using namespace std;
class myComparator
{
public:
    int operator() (const pair<int, double>& p1, const pair<int, double>& p2)
    {
        return p1.second > p2.second;
    }
};

#define pq priority_queue<pair<int, double>, vector<pair<int, double>>, myComparator>
// #define pq priority_queue<pair<int, double>, vector<pair<int, double>>>//, greater<pair<int, double>>>

double cosine_dist(double *A, double *B, int &len)
{
    double dot = 0.0, den_a = 0.0, den_b = 0.0;
    for(int i = 0; i < len; i++){
        dot+=A[i]*B[i];
        den_a+= A[i]*A[i];den_b+= B[i]*B[i];
    }
    // std::cout<<"Cosine: "<<1.0-dot / (sqrt(den_a)*sqrt(den_b))<<std::endl;
    return 1.0-dot / (sqrt(den_a)*sqrt(den_b));
}
void SearchLayer(double *q,int k, bool visited[], pq &cand, pq top_k,int curr_lvl, Graph *G){
    while(cand.size() > 0)
    {
        pair<int, double> top_el = cand.top(); cand.pop();
        int start = G->indptr[top_el.first] + G->level_offset[curr_lvl];
        int end = G->indptr[top_el.first] + G->level_offset[curr_lvl+1];
        for(int node = G->index[start]; node < G->index[end]; node++){
            if (node < 0 || visited[node]){
                continue;
            }
            visited[node] = true;
            double curr_dist = cosine_dist(q, G->vect[node], G->D);
            if (curr_dist > cand.top().second){
                continue;
            }
            top_k.push(make_pair(node, curr_dist));
            //
            pq tmp;
            while(!top_k.empty()){
                if(tmp.size() < k) tmp.push({top_k.top().first, top_k.top().second});
                top_k.pop();
            }
            top_k = tmp;
            //
            cand.push(make_pair(node, curr_dist));
        }
    }
    cand = top_k;
}
void QueryHNSW(double *q,int k, pq &top_k, Graph *G, int num_threads)
{   
    // cout << cosine_dist(G->vect[0], G->vect[1], G->D) << std::endl;
    // cout << cosine_dist(G->vect[1], G->vect[2], G->D) << std::endl;
    // cout << cosine_dist(G->vect[2], G->vect[0], G->D) << std::endl;
    // return;
    bool visited[G->L];
    memset(visited, false, G->L);
    visited[G->ep] = true;
    top_k.push(make_pair(G->ep, cosine_dist(q, G->vect[G->ep], G->D)));
    for(int curr_lvl = G->max_level-1; curr_lvl >= 0; curr_lvl--){
        SearchLayer(q,k, visited, top_k, top_k, curr_lvl, G);
    }
    return;
}