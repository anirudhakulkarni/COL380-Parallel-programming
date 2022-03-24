#include <iostream>
#include <string>
#include <fstream>
#include <assert.h>
#include <chrono>
#include <mpi.h>
using namespace std;
class Graph
{
public:
    int max_level, ep, L = 6, U = 5, D = 5;
    int *level, *index, *indptr, *level_offset;
    float **vect, **user;

    int get_num_lines(std::string filename)
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
    int get_num_cols(std::string filename)
    {
        std::ifstream file(filename.c_str());
        std::string line;
        int num_cols = 0;
        getline(file, line);
        std::stringstream ss(line);
        std::string token;
        while (getline(ss, token, ' '))
        {
            num_cols++;
        }
        return num_cols;
    }
    void read1darray(float *arr, int loc, int L, string filename)
    {
        ifstream in(filename, ios::in | ios::binary);
        in.seekg(loc * sizeof(float));
        for (int i = 0; i < L; i++)
        {
            float val;
            in.read((char *)&val, sizeof(float));
            arr[i] = val;
        }
        in.close();
    }
void parallel_read(float **vect,int L,int D, string filename){
    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int *displs = new int[size];
    int *counts = new int[size];
    for (int i = 0; i < L % size; i++)
    {
        counts[i] = D * (L / size + 1);
        displs[i] = i * counts[i];
    }
    for (int i = L % size; i < size; i++)
    {
        if (i == L % size)
        {
            counts[i] = D * (L / size);
            displs[i] = (L % size) * D * (L / size + 1);
        }
        else
        {
            counts[i] = D * (L / size);
            displs[i] = displs[i - 1] + counts[i - 1];
        }
        // displs[i]=i-(L%size)*(L/size)+D%size*(L/size+1);
        // counts[i]=D*(L/size);
    }
    int loc = displs[rank];
    int end = displs[rank] + counts[rank];



    // int loc=rank*L/size;
    // int end=min(loc+L/size*D,L*D);
    float *arr = new float[end-loc];
    read1darray(arr, loc,end-loc, filename);
    float *big_arr=new float[L*D];
    // MPI_Gatherv(arr, end - loc, MPI_FLOAT, big_arr, counts, displs, MPI_FLOAT, 0, MPI_COMM_WORLD);
    // cout<<"sending\n";
    MPI_Allgatherv(arr, end - loc, MPI_FLOAT, big_arr, counts, displs, MPI_FLOAT, MPI_COMM_WORLD);
    // cout<<"recieved\n";
    // MPI_Gather(arr,L/size,MPI_FLOAT,big_arr,L/size,MPI_FLOAT,0,MPI_COMM_WORLD);
    int ii=0,jj=0;
    for(int i=0;i<L*D;i++){
        vect[jj][ii]=big_arr[i];
        ii++;
        if(ii%D==0){
            ii=0;
            jj++;
        }
    }
    delete big_arr;
    delete arr;
}

    void getinputs(std::string inputDir)
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::string filename = inputDir + "max_level.txt";
        std::ifstream file(filename.c_str());
        file >> max_level;
        file.close();

        filename = inputDir + "ep.txt";
        file.open(filename.c_str());
        file >> ep;
        file.close();

        L = get_num_lines(inputDir + "vect.txt");
        U = get_num_lines(inputDir + "user.txt");
        int D1 = get_num_cols(inputDir + "user.txt");
        D = get_num_cols(inputDir + "vect.txt");

        assert(D1 == 0 || D1 == D);
        level = new int[L];
        filename = inputDir + "level.txt";
        file.open(filename.c_str());
        for (int i = 0; i < L; i++)
        {
            file >> level[i];
        }
        file.close();

        vect = new float *[L];
        for (int i = 0; i < L; i++)
        {
            vect[i] = new float[D];
        }
        // std::cout << "Reading vect.txt" << std::endl;
        filename = inputDir + "vect.txt";
        file.open(filename.c_str());
        for (int i = 0; i < L; i++)
        {
            for (int j = 0; j < D; j++)
            {
                file >> vect[i][j];
                // if(i%50000==0){
                //     std::cout<<vect[i][j]<<" ";
                // }
            }
            if (i % 50000 == 0)
            {

                std::cout << i << std::endl;
            }
        }
        file.close();
        // std::cout << "Reading indptr.txt" << std::endl;
        filename = inputDir + "indptr.txt";
        file.open(filename.c_str());
        indptr = new int[L + 1];
        for (int i = 0; i < L + 1; i++)
        {
            file >> indptr[i];
        }
        file.close();

        int index_size = indptr[L];
        filename = inputDir + "index.txt";
        file.open(filename.c_str());
        index = new int[index_size];
        for (int i = 0; i < index_size; i++)
        {
            file >> index[i];
        }
        file.close();

        int level_offset_size = max_level + 1;
        filename = inputDir + "level_offset.txt";
        file.open(filename.c_str());
        level_offset = new int[level_offset_size];
        for (int i = 0; i < level_offset_size; i++)
        {
            file >> level_offset[i];
        }
        file.close();

        user = new float *[U];
        for (int i = 0; i < U; i++)
        {
            user[i] = new float[D];
        }
        filename = inputDir + "user.txt";
        file.open(filename.c_str());
        if (file)
            for (int i = 0; i < U; i++)
            {
                for (int j = 0; j < D; j++)
                {
                    file >> user[i][j];
                }
            }
        file.close();
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        std::cout << "Time taken by to read inputs: "
                  << duration.count() / 1000000.0 << " seconds" << std::endl;
        return;
    }

    void getinputs_binary(std::string inputDir, std::string usertxt)
    {
        auto start = std::chrono::high_resolution_clock::now();
        U = get_num_lines(usertxt);
        std::string filename = inputDir + "header.bin";
        std::ifstream file(filename, std::ios::in | std::ios::binary);
        file.read((char *)&L, sizeof(float));
        file.read((char *)&D, sizeof(float));
        file.read((char *)&ep, sizeof(float));
        file.read((char *)&max_level, sizeof(float));
        file.close();
        // std::cout << "parameters: " << L << " " << D << " " << ep << " " << max_level << "\n";
        level = new int[L];
        filename = inputDir + "level.bin";
        file.open(filename.c_str());
        for (int i = 0; i < L; i++)
        {
            file.read((char *)&level[i], sizeof(float));
        }
        file.close();

        vect = new float *[L];
        for (int i = 0; i < L; i++)
        {
            vect[i] = new float[D];
        }
        parallel_read(vect,L,D,inputDir+"vect.bin");
        // std::cout << "Reading vect.bin" << std::endl;
        // filename = inputDir + "vect.bin";
        // file.open(filename.c_str());
        // for (int i = 0; i < L; i++)
        // {
        //     for (int j = 0; j < D; j++)
        //     {
        //         file.read((char *)&vect[i][j], sizeof(float));
        //     }
        // }
        // file.close();
        // std::cout << "Reading indptr.bin" << std::endl;
        filename = inputDir + "indptr.bin";
        file.open(filename.c_str());
        indptr = new int[L + 1];
        for (int i = 0; i < L + 1; i++)
        {
            file.read((char *)&indptr[i], sizeof(float));
        }
        file.close();

        int index_size = indptr[L];
        filename = inputDir + "index.bin";
        file.open(filename.c_str());
        index = new int[index_size];
        for (int i = 0; i < index_size; i++)
        {
            file.read((char *)&index[i], sizeof(float));
        }
        file.close();

        int level_offset_size = max_level + 1;
        filename = inputDir + "level_offset.bin";
        file.open(filename.c_str());
        level_offset = new int[level_offset_size];
        for (int i = 0; i < level_offset_size; i++)
        {
            file.read((char *)&level_offset[i], sizeof(float));
        }
        file.close();

        user = new float *[U];
        for (int i = 0; i < U; i++)
        {
            user[i] = new float[D];
        }
        file.close();
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        std::cout << "Time taken by to read inputs: "
                  << duration.count() / 1000000.0 << " seconds" << std::endl;
        return;
    }
    void printinputs()
    {
        std::cout << "max_level: " << max_level << std::endl;
        std::cout << "ep: " << ep << std::endl;
        std::cout << "level: ";
        for (int i = 0; i < L; i++)
        {
            std::cout << level[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "vect: " << std::endl;
        for (int i = 0; i < L; i++)
        {
            for (int j = 0; j < D; j++)
            {
                std::cout << vect[i][j] << " ";
            }
            std::cout << std::endl;
        }
        // std::cout << "indptr: ";
        // for (int i = 0; i < L + 1; i++)
        // {
        //     std::cout << indptr[i] << " ";
        // }
        // std::cout << std::endl;
        // std::cout << "index: ";
        // for (int i = 0; i < indptr[L]; i++)
        // {
        //     std::cout << index[i] << " ";
        // }
        // std::cout << std::endl;
        // std::cout << "level_offset: ";
        // for (int i = 0; i < max_level + 1; i++)
        // {
        //     std::cout << level_offset[i] << " ";
        // }
        // std::cout << std::endl;
        // std::cout << "user: " << std::endl;
        // for (int i = 0; i < U; i++)
        // {
        //     for (int j = 0; j < D; j++)
        //     {
        //         std::cout << user[i][j] << " ";
        //     }
        //     std::cout << std::endl;
        // }
        return;
    }
};