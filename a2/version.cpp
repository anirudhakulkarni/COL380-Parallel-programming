#include <string>
#include <mpi.h>
#include <assert.h>
#include "randomizer.hpp"
#include <bits/stdc++.h>
#include <fstream>

struct Graph
{
    int num_nodes;
    int num_edges;
    int num_steps;
    int num_rec;
    int num_walk;
    std::vector<int> *edges;
    std::vector<std::pair<int, int>> *influence_countert;
    std::vector<int> *circle;

    Graph(int num_nodes, int num_edges, int num_steps, int num_rec, int num_walk, std::vector<int> *edges)
    {
        this->num_nodes = num_nodes;
        this->num_edges = num_edges;
        this->edges = edges;
        this->num_steps = num_steps;
        this->num_rec = num_rec;
        this->num_walk = num_walk;
        influence_countert=new 
        for (int i = 0; i < num_nodes; i++)
        {
            influence_countert[i].resize(num_nodes);
            for (int j = 0; j < num_nodes; j++)
            {
                influence_countert[i][j] = std::make_pair(j, 0);
            }
        }

    }
}