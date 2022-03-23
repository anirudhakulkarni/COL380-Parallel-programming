#include <iostream>
#include <string>
#include <fstream>
#include <assert.h>
#include <chrono>


class Graph{
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
        U = get_num_lines( inputDir + "user.txt");
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
        std::cout<<"Reading vect.txt"<<std::endl;
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
            if(i%50000==0){

                std::cout<<i<<std::endl;
            }
        }
        file.close();
        std::cout<<"Reading indptr.txt"<<std::endl;
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

    void getinputs_binary(std::string inputDir)
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::string filename = inputDir + "parameters.bin";
        std::fstream file(filename.c_str(), std::ios::in | std::ios::binary);
        file >> L;
        file>>ep;
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
        std::cout<<"Reading vect.txt"<<std::endl;
        filename = inputDir + "vect.txt";
        file.open(filename.c_str());
        for (int i = 0; i < L; i++)
        {
            for (int j = 0; j < D; j++)
            {
                file >> vect[i][j];
            }
        }
        file.close();
        std::cout<<"Reading indptr.txt"<<std::endl;
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
        std::cout << "indptr: ";
        for (int i = 0; i < L + 1; i++)
        {
            std::cout << indptr[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "index: ";
        for (int i = 0; i < indptr[L]; i++)
        {
            std::cout << index[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "level_offset: ";
        for (int i = 0; i < max_level + 1; i++)
        {
            std::cout << level_offset[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "user: " << std::endl;
        for (int i = 0; i < U; i++)
        {
            for (int j = 0; j < D; j++)
            {
                std::cout << user[i][j] << " ";
            }
            std::cout << std::endl;
        }
        return;
    }
};