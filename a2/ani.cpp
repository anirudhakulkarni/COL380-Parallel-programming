#include <string>
#include <mpi.h>
#include <assert.h>
#include "randomizer.hpp"
#include <bits/stdc++.h>
#include <fstream>
#include <iostream>

void writehelper(int u, unsigned char *buffer1, std::ofstream &outfile, int &offset)
{
    unsigned char buffer[4];
    // std::cout << "u: " << u << std::endl;
    // std::cout << "offset: " << offset << " u:" << u << std::endl;
    buffer[3] = (unsigned char)(u & 0xFF);
    buffer[2] = (unsigned char)((u >> 8) & 0xFF);
    buffer[1] = (unsigned char)((u >> 16) & 0xFF);
    buffer[0] = (unsigned char)((u >> 24) & 0xFF);
    // outfile.seekp(offset, std::ios::beg);
    // offset += 4;
    outfile.write((char *)&buffer, sizeof(u));
}
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
    int offset = 0;
    // declare 2d array of ints
    int **output_r;
    int **output_i;
    unsigned char buffer[4];
    int rank;
    std::ofstream outfile;

    // std::vector<std::vector<std::pair<int, int>>> influence_countert;
    std::vector<int> *circle;
    Graph(int num_nodes_global, int num_edges, int num_steps, int num_rec, int num_walk, std::vector<int> *edges, int start_node, int end_node, int rank,int size,std::string filename)
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
        this->output_r = new int *[num_nodes_rank];
        this->output_i = new int *[num_nodes_rank];
        // if(rank==)
        this->offset =  (num_rec * 8 + 4)*(rank * num_nodes_global / size);
        this->rank=rank;

        if(rank==0){
             outfile.open(filename, std::ios::binary);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        if(rank>0){
            outfile.open(filename,std::ios::binary|std::ios::out|std::ios::in);
        }
        // influence_countert.resize(num_nodes_global);
        // for (int i = 0; i < num_nodes_global; i++)
        // {
        //     influence_countert[i].resize(num_nodes_global);
        //     for (int j = 0; j < num_nodes_global; j++)
        //     {
        //         influence_countert[i][j] = std::make_pair(j, 0);
        //     }
        // }
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
        // if (edges[node].size() == 0)
        // {
        //     return;
        // }
        // std::cout << "node: " << node << std::endl;
        int influence_countert[num_nodes_global][2];
        for (int i = 0; i < num_nodes_global; i++)
        {
            influence_countert[i][0] = i;
            influence_countert[i][1] = 0;
        }
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
                        influence_countert[curr_node][1]++;
                    }
                }
            }
        }
        for (auto adjedge : edges[node])
        {
            influence_countert[adjedge][1] = 0;
        }
        influence_countert[node][1] = 0;
        // TODO:
        // print type of influence_countert

        // sort influence_countert[num_nodes_global][2] according to second element
        std::qsort(influence_countert, num_nodes_global, sizeof(*influence_countert),
                   [](const void *arg1, const void *arg2) -> int
                   {
                       int const *rhs = static_cast<int const *>(arg1);
                       int const *lhs = static_cast<int const *>(arg2);
                       if (lhs[1] == rhs[1])
                           return lhs[0] < rhs[0];
                       return lhs[1] > rhs[1];
                       return (lhs[0] < rhs[0]) ? -1
                                                : ((rhs[0] < lhs[0]) ? 1
                                                                     : (lhs[1] < rhs[1] ? -1
                                                                                        : ((rhs[1] < lhs[1] ? 1 : 0))));
                   }); // std::sort(influence_countert, influence_countert + num_nodes_global, [](int a[2], int b[2])
        //           { return a[1] > b[1]; });
        // std::sort(influence_countert, influence_countert + num_nodes_global, [](int *a, int *b)
        //           { return a[1] > b[1]; });
        // std::sort(influence_countert, influence_countert + num_nodes_global, [](const std::pair<int, int> &a, const std::pair<int, int> &b)
        //           { if(a.second==b.second) return a.first<b.first; else return a.second>b.second; });

        // for each node, select top num_rec nodes as circle
        // if (node == 0)
        // {
        //     // print influence_countert
        //     for (int i = 0; i < num_nodes_global; i++)
        //     {
        //         std::cout << influence_countert[i][0] << " " << influence_countert[i][1] << "\n";
        //     }
        // }
        // int circle[num_rec];
        int ans = 0;
        // std::ofstream outfile("graph.bin", std::ios::binary);
        // writehelper(node, buffer, outfile, offset);
        // writehelper(edges[node].size(), buffer, outfile, offset);
        // std::cout << node << " " << edges[node].size();
        output_i[node - start_node] = new int[num_rec];
        output_r[node - start_node] = new int[num_rec];
        for (int j = 0; j < num_rec; j++)
        {
            // circle[j] = influence_countert[j][0];
            if (influence_countert[j][1])
            {
                output_r[node - start_node][j] = influence_countert[j][0];
                output_i[node - start_node][j] = influence_countert[j][1];
                // std::cout << "," << influence_countert[j][0] << " " << influence_countert[j][1];
                // writehelper(influence_countert[j][0], buffer, outfile, offset);
                // writehelper(influence_countert[j][1], buffer, outfile, offset);
            }
            else
            {
                output_r[node - start_node][j] = -1;
                output_i[node - start_node][j] = -1;
                // std::cout << ",NULL NULL";
                // outfile.write("NULL", 4);
                // offset += 4;
                // outfile.seekp(offset);
                // outfile.write("NULL", 4);
                // offset += 4;
                // outfile.seekp(offset);
            }
        }
        // std::cout << "\n";
        // std::cout << ans << std::endl;
        // std::cout << "after node: " << node << std::endl;
    }
    void writebin()
    {


        outfile.seekp(offset);
        for (int i = 0; i < num_nodes_rank; i++)
        {
            // writehelper(i + start_node, buffer, outfile, offset);
            writehelper(edges[i + start_node].size(), buffer, outfile, offset);
            for (int j = 0; j < num_rec; j++)
            {
                if (output_r[i][j] != -1)
                {

                    writehelper(output_r[i][j], buffer, outfile, offset);
                    writehelper(output_i[i][j], buffer, outfile, offset);
                }
                else
                {
                    outfile.write("NULL", 4);
                    // offset += 4;
                    // outfile.seekp(offset);
                    outfile.write("NULL", 4);
                    // offset += 4;
                    // outfile.seekp(offset);
                }
            }
        }
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
// void writetobinary(Graph &g, int offset)
// {
//     // for each node write node, adj_list
//     std::ofstream outfile("graph.bin", std::ios::binary);

//     uint32_t u, v;
//     unsigned char buffer[4];
//     // write node number and outdegree
//     for (int i = 0; i < g.num_nodes_rank; i++)
//     {
//         // writehelper(i + g.start_node, buffer, outfile);
//         writehelper(g.edges[i].size(), buffer, outfile, offset);
//         // write num_rec recommendations
//         for (int j = 0; j < g.num_rec; j++)
//         {
//             if (g.influence_countert[i][j][0])
//             {
//                 writehelper(g.influence_countert[i][j].first, buffer, outfile, offset);
//                 writehelper(g.influence_countert[i][j].second, buffer, outfile, offset);
//             }
//             else
//             {
//                 // write "NULL" in 4 bytes
//                 // writehelper(1314212940, buffer, outfile);
//                 // writehelper(1314212940, buffer, outfile);
//                 // buffer[0] = 'N';
//                 // buffer[1] = 'U';
//                 // buffer[2] = 'L';
//                 // buffer[3] = 'L';
//                 // outfile.write((char *)&buffer, sizeof(buffer));
//                 outfile.seekp(offset, std::ios::beg);
//                 outfile.write("NULL", 4);
//                 offset += 4;
//                 outfile.seekp(offset, std::ios::beg);
//                 outfile.write("NULL", 4);
//                 offset += 4;
//                 // outfile.write(1314212940, 4);
//             }
//         }
//     }
// }
// void writehelper(int r, int i, int node, int rec, int rec_id, std::ofstream &outfile)
// {
//     int off = node * (8 * rec + 4);
//     if (rec_id >= 0)
//     {
//         off += 4 + rec_id * 8;
//     }
//     if (r == -1)
//     {
//         outfile.seekp(off, std::ios::beg);
//         outfile.write("NULL", 4);
//         outfile.seekp(off + 4, std::ios::beg);
//         outfile.write("NULL", 4);
//         return;
//     }
//     unsigned char buffer[4];
//     buffer[3] = (unsigned char)(r & 0xFF);
//     buffer[2] = (unsigned char)((r >> 8) & 0xFF);
//     buffer[1] = (unsigned char)((r >> 16) & 0xFF);
//     buffer[0] = (unsigned char)((r >> 24) & 0xFF);
//     outfile.seekp(off, std::ios::beg);
//     outfile.write((char *)&buffer, sizeof(r));
//     if (rec_id >= 0)
//     {
//         buffer[3] = (unsigned char)(i & 0xFF);
//         buffer[2] = (unsigned char)((i >> 8) & 0xFF);
//         buffer[1] = (unsigned char)((i >> 16) & 0xFF);
//         buffer[0] = (unsigned char)((i >> 24) & 0xFF);
//         outfile.seekp(off + 4, std::ios::beg);
//         outfile.write((char *)&buffer, sizeof(i));
//     }
// }
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
    int end_node = std::min((rank + 1) * num_nodes / size, num_nodes);
    std::cout << "start_node: " << start_node << " end_node: " << end_node << std::endl;
    std::string filename= graph_file.substr(0, graph_file.size()-4) + "_out_"+std::to_string(num_steps)+"_"+std::to_string(num_walks)+"_"+std::to_string(num_rec)+".dat";
    Graph g(num_nodes, num_edges, num_steps, num_rec, num_walks, new std::vector<int>[num_nodes], start_node, end_node, rank,size,filename);
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
    // for (int i = 0; i < 10; i++)
    // {
    //     std::cout << g.edges[i].size() << " ";
    // }
    for (int i = 0; i < num_nodes; i++)
    {
        if (start_node <= i && i < end_node)
        {
            // std::cout << "node: " << i << "\n";
            g.randomWalk(i, random_generator);
        }
    }
    std::cout << "Graph random walk done" << std::endl;
    int startlocofrank = rank * (4 * (num_rec * 2 + 1));
    g.writebin();
    // writetobinary(g, startlocofrank);
    // print_random(rank, num_nodes, random_generator);
    // MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

    printf("[MPI process %d] time elapsed during the job: %.2fs.\n", rank, end - start);
    // std::ofstream outfile("graph.bin", std::ios::binary);
    // int f=10;
    // writehelper(6969,new unsigned char[4],outfile ,f);
    MPI_Finalize();
}
