#include <bits/stdc++.h>
// #include "input.cpp"
#include "HNSWpred.h"
// #include "graph.h"
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

    int get_num_lines2(std::string filename)
    {
        std::ifstream file(filename.c_str());
        std::string line;
        int num_lines = 0;
        while (getline(file, line))
        {
            num_lines++;
        }
        return num_lines;
    }

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
     G->U =get_num_lines2("user.txt");
    // int batch_size = G->U / size + (rank < G->U % size);
    int batch_size = G->U / size + (rank < G->U % size);
        int off = 0;
        if (rank < G->U % size)off+= rank*(G->U / size+1);
        else off+= (G->U % size)*(G->U / size+1) + (rank-G->U % size)*(G->U / size);
    int n_threads=5;
    int **ansoutput = new int *[G->U];

    // Pratyush
    // G->U =get_num_lines2("user.txt");
    // std::cout << "G->u" << G->U << std::endl;
    fstream fileu;
    string filename = "user.txt";
    fileu.open(filename.c_str());
    GotoLine(fileu, off);
    // std::cout << "G->u2" << G->U << std::endl;
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
    // if (rank == 1){
    // for(int i = off; i < off+batch_size; i++)
    // {
    //     // std::cout << ";;;" << i-off << ' ' << G->D << std::endl;
    //     for(int y = 0; y < G->D; y++)std::cout << user_curr[i-off][y] << ' ';
    //         std::cout << std::endl;
    // }
    // }
    // MPI_Finalize();
    // return 0;
    std::cout << "G->u4" << G->U << std::endl;
    std::cout << "rank" << rank << " Batch size " << batch_size << std::endl;
#pragma omp parallel num_threads(10)
#pragma omp single
{    // #pragma omp parallel for num_threads(n_threads)
    for(int i = off; i < off+batch_size; i++){
    // for(int i = 2; i < 3; i++){
        // for(int i = 0; i < G->U; i++){
            std::cout<<"User: "<<i<<"\n";

#pragma omp task 
{
        pq hola;
        
        #pragma omp critical
        {
             std::cout << i <<": ";
        for(int y = 0; y < G->D; y++)std::cout << user_curr[i-off][y] << ' ';
        std::cout << std::endl;
        }
        
        QueryHNSW(user_curr[i-off],i,k,hola,G, num_threads,ansoutput);
        
        for (int j = 0; j < ansoutput[i][0]; j++)
            {
                std::cout << ansoutput[i][j + 1] << " ";
            }
            std::cout<< "\n";
        // print priority queue

        //COmment afterwards
        // #pragma omp critical
        // {

        //     std::cout<<"i: "<<i<<": ";
        //     int j = 0;
        //     while(!hola.empty() && j++ < k){
        //         std::pair<int,float> a=hola.top();
        //         hola.pop();
        //         std::cout<<a.first<<"->"<<a.second<<",";
        //     }
        //     std::cout << std::endl;
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