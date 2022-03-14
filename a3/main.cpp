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
int main(int argc, char** argv)
{
    if(MPI_Init(NULL, NULL) != MPI_SUCCESS)
    {
        fprintf(stderr, "Unable to initialize MPI!\n");return -1;
    }
    int rank, size;
    int k = std::stoi(argv[1]);
    std::string inputDir = argv[2];
    int num_threads = std::stoi(argv[3]);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    Graph *G = new Graph;
    G->getinputs(inputDir);
    int batch_size = G->U / size + (G->U % size != 0);
    int n_threads=5;
    // #pragma omp parallel for num_threads(n_threads)
    for(int i = rank*batch_size; i < std::min((rank+1)*batch_size, G->U); i++){
        pq hola;
        QueryHNSW(G->user[i],i,k,hola,G, num_threads);
        // print priority queue

        //COmment afterwards
        #pragma omp critical
        {
            std::cout<<"i: "<<i<<": ";
            int j = 0;
            while(!hola.empty() && j++ < k){
                std::pair<int,double> a=hola.top();
                hola.pop();
                std::cout<<a.first<<"->"<<a.second<<",";
            }
            std::cout << std::endl;
        }
    }

    // G->printinputs();
    MPI_Finalize();
    return 0;
}
//setfacl -m u:cs5190421:rwx *
//setfacl -m u:cs5190421:rwx .