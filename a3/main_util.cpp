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

std::fstream& GotoLine(std::fstream& file, int num){

if (num== 0)return file;
    file.seekg(std::ios::beg);
    // deb(-1);
    string garbage;
    for(int i=0; i < num ; ++i){
        // deb(-2);
        // file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        std::getline(file, garbage);
    }
    return file;
}

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
    int batch_size = G->U / size + (rank < G->U % size);
        int off = 0;
        if (rank < G->U % size)off+= rank*(G->U / size+1);
        else off+= (G->U % size)*(G->U / size+1) + (rank-G->U % size)*(G->U / size);
    // int n_threads = 5;
    int **ansoutput = new int *[G->U];

    fstream fileu;
    string filename = "user.txt";
    fileu.open(filename.c_str());
    GotoLine(fileu, off);

    float **user_curr;
    // std::cout << "haha" <<batch_size << ' ' << off << std::endl;
        // return 0;
    user_curr = new float *[batch_size];
    // std::cout << "G->u3" << G->U << std::endl;
    for(int i = off; i < off+batch_size; i++)
    {
        // std::cout << i << std::endl;
        user_curr[i-off] = new float[G->D];
        for(int j = 0; j < G->D; j++)
        fileu >> user_curr[i-off][j];
    }



#pragma omp parallel num_threads(num_threads)
#pragma omp single
    { // #pragma omp parallel for num_threads(n_threads)
        for(int i = off; i < off+batch_size; i++)
        {
#pragma omp task shared(ansoutput)
            {
                pq hola;
                std::cout<<"\nUser: "<<i<<"\n";
                // #pragma omp critical
                // {
                //     std::cout<<"\nUser: "<<i<<"\n";
                //     for(int y = 0; y < G->D; y++)std::cout << user_curr[i-off][y] << "|";
                //     std::cout << std::endl;
                // }
                QueryHNSW(user_curr[i-off],i,k,hola,G, num_threads,ansoutput);
            }
        }
#pragma omp taskwait
    }
    if (rank==0){
                string filename("tmp2.txt");
        fstream file_out;
        file_out.open(filename, std::ios_base::out);
    

        for(int i = off; i < off+batch_size; i++){
// std::cout << ansoutput[ii][0];
            for (int j = 0; j < ansoutput[i][0]; j++)
            {
                file_out << ansoutput[i][j + 1] << " ";
            }
            file_out<< "\n";
        }
    }

    MPI_Finalize();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << duration.count() / 1000000.0 << " seconds" << std::endl;
    return 0;
}
// setfacl -m u:cs5190421:rwx *
// setfacl -m u:cs5190421:rwx .
// setfacl -m u:cs1190444:rwx *
// setfacl -m u:cs1190444:rwx .