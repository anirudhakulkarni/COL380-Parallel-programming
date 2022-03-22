#include <bits/stdc++.h>
// #include "input.cpp"
#include "HNSWpred.h"

/*
Dimensions:
L: number of news items == number of nodes
D: number of features
U: number of users
level: L-dimensional array of levels. indicates level of each news item
indptr: L+1-dimensional array of indices
index: index[L]-dimensional array of indices
level_offset: max_level-dimensional array of offsets
vect: LxD-dimension: represents embedding of each news item
user: UxD-dimensional array of user features
*/
using namespace std::chrono;
int main(int argc, char** argv)
{
    if(MPI_Init(NULL, NULL) != MPI_SUCCESS)
    {
        fprintf(stderr, "Unable to initialize MPI!\n");return -1;
    }
    int rank, size;
    int k = std::stoi(argv[1]);
    std::cout<<k<<"...........\n";
    std::string inputDir = argv[2];
    int num_threads = std::stoi(argv[3]);
    auto start=high_resolution_clock::now();
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    Graph *G = new Graph;
    G->getinputs(inputDir);
    int batch_size = G->U / size + (G->U % size != 0);
    int n_threads=5;
    int *ansoutput=new int[G->U];
#pragma omp parallel num_threads(10)
#pragma omp single
{    // #pragma omp parallel for num_threads(n_threads)
    for(int i = rank*batch_size; i < std::min((rank+1)*batch_size, G->U); i++){
    // for(int i = 2; i < 3; i++){
        // for(int i = 0; i < G->U; i++){

#pragma omp task 
{
        pq hola;
        // std::cout<<"User: "<<i<<"\n";
        QueryHNSW(G->user[i],i,k,hola,G, num_threads,ansoutput);
        // print priority queue

        //COmment afterwards
        // #pragma omp critical
        // {

            // std::cout<<"i: "<<i<<": ";
            int j = 0;
            while(!hola.empty() && j++ < k){
                std::pair<int,float> a=hola.top();
                hola.pop();
                // std::cout<<a.first<<"->"<<a.second<<",";
            }
            // std::cout << std::endl;
        // }
}
    }
#pragma omp taskwait

}    // G->printinputs();
    MPI_Finalize();
    auto stop=high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout<<duration.count()/1000000.0<<" seconds" <<std::endl;
    return 0;
}
//setfacl -m u:cs5190421:rwx *
//setfacl -m u:cs5190421:rwx .