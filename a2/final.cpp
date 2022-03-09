#include <string>
#include <mpi.h>
#include <assert.h>
#include "randomizer.hpp"
#include <bits/stdc++.h>
#include <fstream>

struct Graph
{
    int num_nodes_global;
    int num_nodes_rank;
    int num_edges;
    int num_steps;
    int num_rec;
    int num_walk;
    int start_node;
    int end_node;
    std::vector<int> *edges;
    std::vector<std::vector<std::pair<int, int>>> influence_countert;
    std::vector<int> *circle;
    Graph(int num_nodes_global, int num_edges, int num_steps, int num_rec, int num_walk, std::vector<int> *edges, int start_node, int end_node)
    {
        this->num_nodes_global = num_nodes_global;
        this->num_edges = num_edges;
        this->edges = edges;
        this->num_steps = num_steps;
        this->num_rec = num_rec;
        this->num_walk = num_walk;
        this->start_node = start_node;
        this->end_node = end_node;
        this->num_nodes_rank = end_node - start_node;
        influence_countert.resize(num_nodes_rank);
        for (int i = 0; i < num_nodes_rank; i++)
        {
            influence_countert[i].resize(num_nodes_global);
            for (int j = 0; j < num_nodes_global; j++)
            {
                influence_countert[i][j] = std::make_pair(j, 0);
            }
        }
    }
    void addedge(int u, int v)
    {
        edges[u].push_back(v);
        num_edges++;
    }
    void printEdges()
    {
        for (int i = 0; i < num_nodes_global; i++)
        {
            std::cout << i << " : ";
            for (int j = 0; j < edges[i].size(); j++)
            {
                std::cout << edges[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
    void randomWalk(int node, Randomizer &r)
    {
        for (auto adjnode : edges[node])
        {
            int curr_node = adjnode;
            if (edges[curr_node].size() == 0)
            {
                curr_node = adjnode;
                continue;
            }
            for (int walk = 0; walk < num_walk; walk++)
            {
                curr_node = adjnode;
                for (int step = 0; step < num_steps; step++)
                {
                    // std::cout << "n\n";
                    if (edges[curr_node].size() == 0)
                    {
                        curr_node = adjnode;
                        continue;
                    }

                    int rand_num = r.get_random_value(node);
                    // std::cout << "rand_num: " << rand_num << "\n";
                    if (rand_num == -1)
                    {
                        curr_node = adjnode;
                        continue;
                    }
                    else
                    {
                        rand_num = (int)(rand_num % edges[curr_node].size());
                        curr_node = edges[curr_node][rand_num];
                        // std::cout << node << " " << curr_node << " " << rand_num << "\n";
                        influence_countert[node - start_node][curr_node].second++;
                    }
                }
            }
        }
        for (auto adjedge : edges[node])
        {
            influence_countert[node][adjedge].second = 0;
        }
        influence_countert[node][node].second = 0;
        std::sort(influence_countert[node].begin(), influence_countert[node].end(), [](const std::pair<int, int> &a, const std::pair<int, int> &b)
                  { if(a.second==b.second) return a.first<b.first; else return a.second>b.second; });
    }
    std::vector<int> *find_circle()
    {
        // for each node, select top num_rec nodes as circle
        circle = new std::vector<int>[num_nodes_rank];
        int ans = 0;
        for (int i = 0; i < num_nodes_rank; i++)
        {
            // std::cout << i << " " << edges[i].size();
            for (int j = 0; j < num_rec; j++)
            {
                circle[i].push_back(influence_countert[i][j].first);
                // if (influence_countert[i][j].second)
                //     std::cout << "," << influence_countert[i][j].first << " " << influence_countert[i][j].second;
                // else
                //     std::cout << ",NULL NULL";
                if (influence_countert[i][j].first)
                {
                    ans++;
                }
            }
            // std::cout << "\n";
        }
        std::cout << ans << std::endl;
        return circle;
    }
};

void readfromtxt(Graph &g, std::string graph_file, int num_nodes, int num_edges)
{
    std::string line;
    std::ifstream myfile(graph_file);
    if (myfile.is_open())
    {
        std::cout << "file opened\n";
        while (getline(myfile, line))
        {
            std::stringstream ss(line);
            int u, v;
            ss >> u >> v;
            // std::cout << u << " " << v << std::endl;
            g.addedge(u, v);
        }
        myfile.close();
    }
}
void readfrombin(Graph &g, std::string graph_file, int num_nodes, int num_edges)
{
    std::ifstream infile(graph_file, std::ios::binary);
    std::cout << "num_nodes: " << num_nodes << " num_edges: " << num_edges << std::endl;
    std::vector<int> *edges = new std::vector<int>[num_nodes];
    uint32_t u, v;
    unsigned char buffer[4];
    for (int i = 0; i < num_edges; i++)
    {
        infile.read((char *)&buffer, sizeof(u));
        u = (int)buffer[3] | (int)buffer[2] << 8 | (int)buffer[1] << 16 | (int)buffer[0] << 24;
        infile.read((char *)&buffer, sizeof(v));
        v = (int)buffer[3] | (int)buffer[2] << 8 | (int)buffer[1] << 16 | (int)buffer[0] << 24;

        // std::cout << u << " " << v << std::endl;
        g.addedge(u, v);
        // edges[u].push_back(v);
    }
    infile.close();
}
void writehelper(int u, unsigned char *buffer, std::ofstream &outfile)
{
    // unsigned char buffer[4];
    // std::cout << "u: " << u << std::endl;
    buffer[3] = (unsigned char)(u & 0xFF);
    buffer[2] = (unsigned char)((u >> 8) & 0xFF);
    buffer[1] = (unsigned char)((u >> 16) & 0xFF);
    buffer[0] = (unsigned char)((u >> 24) & 0xFF);
    outfile.write((char *)&buffer, sizeof(u));
}
void writetobinary(Graph &g, int offset)
{
    // for each node write node, adj_list
    std::ofstream outfile("graph.bin", std::ios::binary);

    outfile.seekp(offset);
    uint32_t u, v;
    unsigned char buffer[4];
    // write node number and outdegree
    for (int i = 0; i < g.num_nodes_rank; i++)
    {
        // writehelper(i + g.start_node, buffer, outfile);
        writehelper(g.edges[i].size(), buffer, outfile);
        // write num_rec recommendations
        for (int j = 0; j < g.num_rec; j++)
        {
            if (g.influence_countert[i][j].first)
            {
                writehelper(g.influence_countert[i][j].first, buffer, outfile);
                writehelper(g.influence_countert[i][j].second, buffer, outfile);
            }
            else
            {
                // write "NULL" in 4 bytes
                writehelper(1314212940, buffer, outfile);
                writehelper(1314212940, buffer, outfile);

                // outfile.write(1314212940, 4);
                // outfile.write(1314212940, 4);
            }
        }
    }
}
int main(int argc, char *argv[])
{
#define Wtime start

    assert(argc > 8);
    std::string graph_file = argv[1];
    int num_nodes = std::stoi(argv[2]);
    int num_edges = std::stoi(argv[3]);
    float restart_rand_num = std::stof(argv[4]);
    int num_steps = std::stoi(argv[5]);
    int num_walks = std::stoi(argv[6]);
    int num_rec = std::stoi(argv[7]);
    int seed = std::stoi(argv[8]);
    double start;
    double end;

    start = MPI_Wtime();

    // Only one randomizer object should be used per MPI rank, and all should have same seed
    Randomizer random_generator(seed, num_nodes, restart_rand_num);
    int rank, size;

    // Starting MPI pipeline
    MPI_Init(NULL, NULL);

    // Extracting Rank and Processor Count
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Creating a graph object
    int start_node = rank * num_nodes / size;
    int end_node = (rank + 1) * num_nodes / size;
    Graph g(num_nodes, num_edges, num_steps, num_rec, num_walks, new std::vector<int>[num_nodes], start_node, end_node);
    // Reading the graph from file
    // open file
    // readfromtxt(g, graph_file);
    if (".txt" == graph_file.substr(graph_file.size() - 4, 4))
    {
        readfromtxt(g, graph_file, num_nodes, num_edges);
    }
    else
    {
        readfrombin(g, graph_file, num_nodes, num_edges);
    }
    std::cout << "Graph read from file" << std::endl;
    // print first 10 edges
    for (int i = 0; i < 10; i++)
    {
        std::cout << g.edges[i].size() << " ";
    }
    // g.printEdges();

    // for all elements call this function
    // for (int i = start_thread; i < end_thread; i++)
    // {
    //     // std::cout << "Node " << i << "\n";
    //     g.randomWalk(i, random_generator);
    // }
    for (int i = 0; i < num_nodes; i++)
    {
        // if (i == 10)
        //     break;
        if (start_node <= i && i < end_node)
            g.randomWalk(i, random_generator);
        else
        {
            break;
        }
        // std::cout << i << "\n";
    }
    std::cout << "Graph random walk done" << std::endl;
    g.find_circle();
    int startlocofrank = rank * (4 * (num_rec * 2 + 1));
    // writetobinary(g, startlocofrank);
    // print_random(rank, num_nodes, random_generator);
    // MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

    printf("[MPI process %d] time elapsed during the job: %.2fs.\n", rank, end - start);

    MPI_Finalize();
}
