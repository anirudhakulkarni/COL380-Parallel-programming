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
int main(int argc, char **argv)
{
    if (MPI_Init(NULL, NULL) != MPI_SUCCESS)
    {
        fprintf(stderr, "Unable to initialize MPI!\n");
        return -1;
    }
    int rank, size;
    int k = std::stoi(argv[1]);
    std::cout << k << "...........\n";
    std::string inputDir = argv[2];
    int num_threads = std::stoi(argv[3]);
    auto start = high_resolution_clock::now();
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    Graph *G = new Graph;
    G->getinputs(inputDir);
    int batch_size = G->U / size + (G->U % size != 0);
    // int n_threads = 5;
    int **ansoutput = new int *[G->U];
#pragma omp parallel num_threads(num_threads)
#pragma omp single
    { // #pragma omp parallel for num_threads(n_threads)
        for (int i = rank * batch_size; i < std::min((rank + 1) * batch_size, G->U); i++)
        {
#pragma omp task shared(ansoutput)
            {
                pq hola;
                std::cout<<"User: "<<i<<"\n";
                QueryHNSW(G->user[i], i, k, hola, G, num_threads, ansoutput);
            }
        }
#pragma omp taskwait
    }
    if (rank==0){
                string filename("tmp2.txt");
        fstream file_out;
        file_out.open(filename, std::ios_base::out);
    

        for (int i = rank * batch_size; i < std::min((rank + 1) * batch_size, G->U); i++){
            for (int j = 0; j < ansoutput[i][0]; j++)
            {
                file_out << ansoutput[i][j + 1] << " ";
            }
            file_out<< "\n";
        }
    }
    // int **ansoutput_master ;
    // if(rank == 0){
    //     std::cout<<"Master thread: assigning\n";
    //     ansoutput_master = new int *[G->U];
    //     for (int i = 0; i < G->U; i++)
    //     {
    //         ansoutput_master[i] = new int[k + 1];
    //         for (int j = 0; j < k + 1; j++)
    //         {
    //             ansoutput_master[i][j] = ansoutput[i][j];
    //         }
    //     }
    //     std::cout<<"Gathering:\n";
    // }
    // MPI_Gather(ansoutput, batch_size * (k + 1), MPI_INT, ansoutput_master, batch_size * (k + 1), MPI_INT, 0, MPI_COMM_WORLD);
    // if(rank == 0){
    //     std::ofstream output;
    //     output.open("output.txt");
    //     for (int i = 0; i < G->U; i++)
    //     {
    //         for (int j = 0; j < k + 1; j++)
    //         {
    //             output << ansoutput_master[i][j] << " ";
    //         }
    //         output << "\n";
    //     }
    //     output.close();
    // }
    

    MPI_Finalize();
        // string filename("tmp.txt");
        // fstream file_out;
        // file_out.open(filename, std::ios_base::out);
    
        // std::cout << "Printing final ans: " << G->U << "\n";
        // for (int ii = 0; ii < G->U; ii++)
        // {
        //     // std::cout << ansoutput[ii][0];
        //     for (int j = 0; j < ansoutput[ii][0]; j++)
        //     {
        //         file_out << ansoutput[ii][j + 1] << " ";
        //     }
        //     file_out<< "\n";
        // }
        // file_out.close();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << duration.count() / 1000000.0 << " seconds" << std::endl;
    return 0;
}
// setfacl -m u:cs5190421:rwx *
// setfacl -m u:cs5190421:rwx .