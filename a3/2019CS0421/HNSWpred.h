#pragma once
#include "graph.h"
#include <cmath>
#include <queue>
#include <mpi.h>
using namespace std;

void debug(int a, int b)
{
    std::cout << "#" << a << ' ' << b << std::endl;
}
class myComparator
{
public:
    int operator()(const pair<int, float> &p1, const pair<int, float> &p2)
    {
        // min heap. Min value stays at top >
        return p1.second < p2.second;
    }
};
class myComparator2
{
public:
    int operator()(const pair<int, float> &p1, const pair<int, float> &p2)
    {
        // min heap. Min value stays at top >
        return p1.second > p2.second;
    }
};

#define pq priority_queue<pair<int, float>, vector<pair<int, float>>, myComparator>
#define pq_min priority_queue<pair<int, float>, vector<pair<int, float>>, myComparator2>
// #define pq priority_queue<pair<int, float>, vector<pair<int, float>>>//, greater<pair<int, float>>>

float cosine_dist(float *A, float *B, int &len)
{
    // std::cout<<"before\n";
    double dot = 0.0, den_a = 0.0, den_b = 0.0;
    for (int i = 0; i < len; i++)
    {
        // std::cout<<i<<"\n";
        // std::cout<<i<<" "<<A[i]<<" "<<B[i]<<"\n";
        dot += A[i] * B[i];
        den_a += A[i] * A[i];
        den_b += B[i] * B[i];
    }
    if (den_a == 0.0)
    {
        // std::cout << "This is 0.0:\n";
        // for (int i = 0; i < len; i++)
        // {
        //     // std::cout<<i<<"\n";
        //     std::cout << i << " " << A[i] << " " << B[i] << "\n";
        // }
        // std::cout << "Cosine: length: " << len << ", den_a: " << den_a << ' ' << den_b << ' ' << 1.0 - dot / (sqrt(den_a) * sqrt(den_b)) << std::endl;
    }
    // std::cout<<"after\n";
    return 1.0 - dot / (sqrt(den_a) * sqrt(den_b));
}
void printVisited(bool visited[])
{
    // std::cout << "printing visited" << std::endl;
    for (int i = 0; i < 5; i++)
    {
        std::cout << visited[i] << std::endl;
    }
}
void printQueue(pq hola)
{
    int j = 0;
    while (!hola.empty() && j++ < 5)
    {
        std::pair<int, float> a = hola.top();
        hola.pop();
        std::cout << a.first << "->" << a.second << ",";
    }
}
int getMaxInPQ(pq f)
{
    float max = -1.0;
    // std::cout<<"finding max: \n";
    while (!f.empty())
    {
        float val = f.top().second;
        // std::cout<<val<<" ";
        if (val > max)
        {
            max = val;
        }
        f.pop();
    }
    // std::cout<<"\n found max = "<<max<<"\n";
    return max;
}

void SearchLayer(float *q, int k, int i, bool visited[], pq &cand, pq top_k, int curr_lvl, Graph *G)
{
    float max_so_far = -1.0;
    pq_min cand_min;
    
    while (cand.size() > 0)
    {
        pair<int, float> top_el = cand.top();
        cand.pop();
        int start = G->indptr[top_el.first] + G->level_offset[curr_lvl];
        int end = G->indptr[top_el.first] + G->level_offset[curr_lvl + 1];
        // debug(start, end);
        // printVisited(visited);

        for (int e = start; e < end; e++)
        {
            int node = G->index[e];
            // debug(i, node);
            if (node < 0 || visited[node])
            {
                continue;
            }
            visited[node] = true;
            // std::cout<<e<<" =e\n";
            // std::cout<<node<<" = Node\n";
            float curr_dist = cosine_dist(q, G->vect[node], G->D);
            // DEBUG: its cand.max and cand is min heap
            // std::cout<<cand.size()<<"..........................\n";
            // if (cand.size()>=k && curr_dist > cand.top().second){
            //     continue;
            // }
            if (top_k.size() >= k && curr_dist > top_k.top().second)
            {
                continue;
            }
            // if (curr_dist > max_so_far && max_so_far > -0.5 && cand.size()>=k)continue;
            top_k.push(make_pair(node, curr_dist));
            while (top_k.size() > k)
                top_k.pop();
            cand.push(make_pair(node, curr_dist));
        }
    }
    cand = top_k;
}
void QueryHNSW(float *q, int i, int k, pq &top_k, Graph *G, int *ansoutput)
{
    bool visited[G->L];
    memset(visited, false, G->L);
    visited[G->ep] = true;
    top_k.push(make_pair(G->ep, cosine_dist(q, G->vect[G->ep], G->D)));
    // debug(i, G->ep);
    for (int curr_lvl = G->max_level - 1; curr_lvl >= 0; curr_lvl--)
    {
        // debug(-1,-1);
        // std::cout<<i<<"\n";
        SearchLayer(q, k, i, visited, top_k, top_k, curr_lvl, G);
    }
    // std::cout<<"FFFFFFFFFFFFFFFFFFFf :"<<top_k.size()<<" i:"<<i<<std::endl;
    // ansoutput =new int[k+1];
    // std::cout << "Size of max heap: " << top_k.size() << std::endl;
    // ansoutput[0] = top_k.size();
    int j = 1;
    while ((!top_k.empty()) && j <= k)
    {
        std::pair<int, float> a = top_k.top();
        // std::cout << a.first << "->" << a.second << ",";
        top_k.pop();
        ansoutput[k-j ] = a.first;
        j++;
        // std::cout<<a.first<<" ";
    }
    // std::cout<<"FUCK:inside "<<ansoutput[i][1]<<std::endl; 331/40

    return;
}
