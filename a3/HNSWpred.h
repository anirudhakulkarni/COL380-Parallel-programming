#pragma once
#include "graph.h"
#include <cmath>
#include <queue>
#include <mpi.h>
using namespace std;

void debug(int a , int b)
{
    std::cout << "#" << a << ' ' << b << std::endl;
}
class myComparator
{
public:
    int operator() (const pair<int, double>& p1, const pair<int, double>& p2)
    {
        // min heap. Min value stays at top >
        return p1.second > p2.second;
    }
};

#define pq priority_queue<pair<int, double>, vector<pair<int, double>>, myComparator>
// #define pq priority_queue<pair<int, double>, vector<pair<int, double>>>//, greater<pair<int, double>>>

double cosine_dist(double *A, double *B, int &len)
{
    // std::cout<<"before\n";
    double dot = 0.0, den_a = 0.0, den_b = 0.0;
    for(int i = 0; i < len; i++){
        // std::cout<<i<<"\n";
        // std::cout<<i<<" "<<A[i]<<" "<<B[i]<<"\n";
        dot+=A[i]*B[i];
        den_a+= A[i]*A[i];den_b+= B[i]*B[i];
    }
    // std::cout<<"Cosine: "<<1.0-dot / (sqrt(den_a)*sqrt(den_b))<<std::endl;
    // std::cout<<"after\n";
    return 1.0-dot / (sqrt(den_a)*sqrt(den_b));
}
void printVisited(bool visited[]){
    std::cout<<"printing visited"<<std::endl;
    for(int i=0;i<5;i++){
        std::cout<<visited[i]<<std::endl;
    }
}
void printQueue(pq hola){
    int j = 0;
    while(!hola.empty() && j++ < 5){
        std::pair<int,double> a=hola.top();
        hola.pop();
        std::cout<<a.first<<"->"<<a.second<<",";
    }

}
int getMaxInPQ(pq f) {
    double max = -1.0;
    std::cout<<"finding max: \n";
    while (!f.empty()) {
        double val = f.top().second;
        std::cout<<val<<" ";
        if (val > max) {
            max = val;
        }
        f.pop();
    }
    std::cout<<"\n found max = "<<max<<"\n";
    return max;
}

void SearchLayer(double *q,int k,int i, bool visited[], pq &cand, pq top_k,int curr_lvl, Graph *G){
    double max_so_far = -1.0;
    while(cand.size() > 0)
    {
        pair<int, double> top_el = cand.top(); cand.pop();
        std::cout<<"top_el: "<<top_el.first<<" "<<top_el.second<<" current_lvl: "<<curr_lvl<<" \n";
        int start = G->indptr[top_el.first] + G->level_offset[curr_lvl];
        int end = G->indptr[top_el.first] + G->level_offset[curr_lvl+1];
        // debug(start, end);
        // printVisited(visited);

        for(int e = start; e < end; e++){
            int node = G->index[e];
            // debug(i, node);
            if (node < 0 || visited[node]){
                continue;
            }
            visited[node] = true;
            std::cout<<e<<" =e\n";
            std::cout<<node<<" = Node\n";
            double curr_dist = cosine_dist(q, G->vect[node], G->D);
            // DEBUG: its cand.max and cand is min heap 
            std::cout<<cand.size()<<"..........................\n";
            // if (cand.size()>=k && curr_dist > cand.top().second){
            //     continue;
            // }
            if (cand.size()>=k && curr_dist>getMaxInPQ(cand) ){
                continue;
            }
            // if (curr_dist > max_so_far && max_so_far > -0.5 && cand.size()>=k)continue;
            top_k.push(make_pair(node, curr_dist));
            max_so_far = max(curr_dist, max_so_far);
            //
            pq tmp;
            while(!top_k.empty()){
                if(tmp.size() < k) {
                    tmp.push({top_k.top().first, top_k.top().second});
                    // max_so_far = max(max_so_far, top_k.top().second);
                }
                top_k.pop();
            }
            // std::cout<<"Before"<<std::endl;
            // printQueue(top_k);
            top_k = tmp;
            // printQueue(top_k);

            //
            cand.push(make_pair(node, curr_dist));
        }
    }
    cand = top_k;
}
void QueryHNSW(double *q,int i,int k, pq &top_k, Graph *G, int num_threads)
{   
    bool visited[G->L];
    memset(visited, false, G->L);
    visited[G->ep] = true;
    top_k.push(make_pair(G->ep, cosine_dist(q, G->vect[G->ep], G->D)));
    // debug(i, G->ep);
    for(int curr_lvl = G->max_level-1; curr_lvl >= 0; curr_lvl--){
        // debug(-1,-1);
        SearchLayer(q, k, i, visited, top_k, top_k, curr_lvl, G);
    }
    return;
}