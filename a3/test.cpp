#include <iostream>
#include <string>
#include <fstream>
#include <assert.h>
#include <chrono>
#include <bits/stdc++.h>

void save_array(float **arr, int r, int c, std::string filename)
{
    std::ofstream out(filename, std::ios::out | std::ios::binary);
    unsigned char buffer[4];
    // save 2d array to binary
    std::cout<<"saving to binary"<<std::endl;
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            std::cout<<arr[i][j]<<" ";
            float val = arr[i][j];
            memcpy(buffer, &val, sizeof(float));
            out.write((char *)buffer, sizeof(float));
        }
    }
}

void load_array(float **arr, int r, int c, std::string filename)
{
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            float val;
            in.read((char *)&val, sizeof(float));
            arr[i][j] = val;
        }
    }
    in.close();
}

void read2darray(float **arr, int L, int D,std::string filename)
{
    std::cout<<"reading from txt"<<std::endl;
    std::ifstream file(filename.c_str());
    std::cout<<"file opened: "<<filename<<std::endl;
    for (int i = 0; i < L; i++)
    {
        for (int j = 0; j < D; j++)
        {
            // output file
            file >> arr[i][j];
            std::cout<<arr[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
    file.close();
}

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
int main()
{
    std::string inputDir = "./anz_dd_cp/";
    int L, D;
    L = get_num_lines(inputDir + "vect.txt");
    D = get_num_cols(inputDir + "vect.txt");
    float **arr= new float*[L];
    for (int i = 0; i < L; i++)
    {
        arr[i] = new float[D];
    }
    read2darray(arr, L, D, inputDir + "vect.txt");
    // float **arr = new float *[10];
    // for (int i = 0; i < 10; i++)
    // {
    //     arr[i] = new float[10];
    //     for (int j = 0; j < 10; j++)
    //     {
    //         arr[i][j] = (i + 17.0) / (j + 1.0);
    //     }
    // }
    for (int i = 0; i < L; i++)
    {
        for (int j = 0; j < D; j++)
        {
            std::cout << arr[i][j] << " ";
        }
        std::cout << std::endl;
    }
    save_array(arr, L, D, "test.bin");
    std::cout<<"saved\n";
    float **arr2 = new float *[L];
    for (int i = 0; i < L; i++)
    {
        arr2[i] = new float[D];
    }
    load_array(arr2, L, D, "test.bin");
    for (int i = 0; i < L; i++)
    {
        for (int j = 0; j < D; j++)
        {
            std::cout << arr2[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout<<"loaded\n";

    std::string filename="./anz_dd_cp/vect.txt";
    std::cout<<"reading from txt"<<std::endl;
    std::ifstream file(filename.c_str());
    file.open(filename.c_str());
    std::cout<<"file opened: "<<filename<<std::endl;
    float a;
    file>>a;
    std::cout<<a<<std::endl;
    for (int i = 0; i < L; i++)
    {
        for (int j = 0; j < D; j++)
        {
            // output file
            float a;
            file >> a;
            std::cout<<a<<" ";
        }
        std::cout<<std::endl;
    }
    file.close();


    // read filename and print


    return 0;
}
