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

std::ifstream &GotoLine(std::ifstream &file, int num)
{
    file.seekg(std::ios::beg);
    if (num == 0)
        return file;
    // deb(-1);
    string garbage;
    for (int i = 0; i < num; ++i)
    {
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
    std::string usertxt = argv[4];
    auto start = high_resolution_clock::now();
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::cout << size << std::endl;
    Graph *G = new Graph;
    G->getinputs_binary(inputDir, usertxt);
    int batch_size = G->U / size + (rank < G->U % size);
    int off = 0;
    if (rank < G->U % size)
        off += rank * (G->U / size + 1);
    else
        off += (G->U % size) * (G->U / size + 1) + (rank - G->U % size) * (G->U / size);
    // int n_threads = 5;
    int *final_outputs;
    if (rank == 0)
    {
        final_outputs = new int[G->U * (k + 1)];
        // for(int i = 0; i < G->U; i++)final_outputs[i] = new int [k+1];
    }
    int *ansoutput = new int[batch_size * (k + 1)];

    ifstream fileu;
    string filename = usertxt;
    std::cout << filename << std::endl;
    fileu.open(filename);
    // string random;
    // while(getline(fileu, random)){
    //     std::cout << random << std::endl;
    // }
    // if (!fileu)std::cout << "Failed to open" << std::endl;
    GotoLine(fileu, off);

    float **user_curr;
    user_curr = new float *[batch_size];
    for (int i = off; i < off + batch_size; i++)
    {
        // std::cout << i << std::endl;
        string to_read;
        getline(fileu, to_read);
        // std::cout << "-->" << to_read << std::endl;
        istringstream iss(to_read);
        user_curr[i - off] = new float[G->D];
        for (int j = 0; j < G->D; j++)
            iss >> user_curr[i - off][j];
    }
    // string file_user("user_read.txt");
    // fstream file_u;
    // file_u.open(file_user);
    // if (!file_u)
    //     std::cout << "Failed to open" << std::endl;
    // for(int i = off; i < off+batch_size; i++)
    // {
    //     std::cout << i << ": "<<std::endl;
    //     // user_curr[i-off] = new float[G->D];
    //     for(int j = 0; j < G->D; j++)
    //     std::cout  << user_curr[i-off][j] << ' ';
    //     std::cout << '\n';
    // }
    // return 0;

#pragma omp parallel num_threads(num_threads)
#pragma omp single
    { // #pragma omp parallel for num_threads(n_threads)
//         for (int i = off; i < off + batch_size; i++)
//         {
//             // std::cout<<"\nUserx: "<<i<<"\n";
// #pragma omp task shared(ansoutput, G, user_curr)
//             {
//                 pq hola;
//                 std::cout << "\nUser: " << i << "\n";
//                 // #pragma omp critical
//                 // {
//                 // std::cout<<"\nUser: "<<i<<"\n";

//                 // for(int y = 0; y < G->D; y++)std::cout << user_curr[i-off][y] << "|";
//                 // std::cout << std::endl;
//                 // }
//                 QueryHNSW(user_curr[i - off], i, k, hola, G, num_threads, ansoutput + (i - off) * (k + 1));
//             }
//         }
        for (int i = off; i < off + batch_size; i+=10)
        {

#pragma omp task shared(ansoutput, G, user_curr)
            {
                for (int j=i; j < std::min(i+10, off + batch_size); j++)
                {
                    pq hola;
                    // std::cout << "\nUser: " << j << "\n";
                    QueryHNSW(user_curr[j - off], j, k, hola, G, num_threads, ansoutput + (j - off) * (k + 1));
                }
                
            }
        }

#pragma omp taskwait
    }
    // if (rank == 2)
    // for(int i = 0; i < batch_size*(k+1); i++){
    //     std::cout << ansoutput[i] << ' ';
    // }
    std::cout << std::endl;
    MPI_Gather(ansoutput, batch_size * (k + 1), MPI_INT, final_outputs, batch_size * (k + 1), MPI_INT, 0, MPI_COMM_WORLD);
    // if (rank == 0)
    // for(int i = 0; i < (G->U)*(k+1); i++){
    //     std::cout << final_outputs[i] << ' ';
    // }
    //     std::cout << std::endl;
    if (rank == 0)
    {
        std::cout<<"Saving\n";
        string filename2("tmp3_or.txt");
        fstream file_out;
        file_out.open(filename2,fstream::out);
        if(file_out.is_open()){
            std::cout<<"opened\n";
        }
        // GotoLine(file_out, off);
        for (int i = 0; i < G->U; i++)
        {
            // std::cout << ansoutput[ii][0];
            for (int j = 1; j < k + 1; j++)
            {
                file_out << final_outputs[i * (k + 1) + j] << " ";
                // std::cout<<final_outputs[i * (k + 1) + j] << " ";
            }
            file_out << "\n";
        }
        file_out.close();
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