#include <queue>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

using namespace std;

void save_2darray(float **arr, int r, int c, string filename)
{
    ofstream out(filename, ios::out | ios::binary);
    unsigned char buffer[4];
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            float val = arr[i][j];
            memcpy(buffer, &val, sizeof(float));
            out.write((char *)buffer, sizeof(float));
        }
    }
}

void load_2darray(float **arr, int r, int c, string filename)
{
    ifstream in(filename, ios::in | ios::binary);
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

void read2darray(float **arr, int L, int D, string filename)
{
    std::ifstream file(filename.c_str());
    for (int i = 0; i < L; i++)
    {
        for (int j = 0; j < D; j++)
        {
            file >> arr[i][j];
        }
    }
    file.close();
}
void read1darray(float *arr, int L, string filename)
{
    cout << "reading from txt: " << filename << endl;
    std::ifstream file(filename.c_str());
    cout << "file opened: " << filename << endl;
    for (int i = 0; i < L; i++)
    {
        file >> arr[i];
    }
    file.close();
}
void save_1darray(int *arr, int L, string filename)
{
    ofstream out(filename, ios::out | ios::binary);
    unsigned char buffer[4];
    for (int i = 0; i < L; i++)
    {
        int val = arr[i];
        // cout<<"val: "<<val<<endl;
        memcpy(buffer, &val, sizeof(float));
        out.write((char *)buffer, sizeof(float));
    }
}
void load_1dbinary(float *arr, int L, string filename)
{
    ifstream in(filename, ios::in | ios::binary);
    for (int i = 0; i < L; i++)
    {
        float val;
        in.read((char *)&val, sizeof(float));
        arr[i] = val;
    }
    in.close();
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
    stringstream ss(line);
    std::string token;
    while (getline(ss, token, ' '))
    {
        num_cols++;
    }
    return num_cols;
}
void save_parameters(int L, int D, int ep, int max_level, string filename)
{
    ofstream out(filename, ios::out | ios::binary);
    unsigned char buffer[4];
    memcpy(buffer, &L, sizeof(float));
    out.write((char *)buffer, sizeof(float));
    memcpy(buffer, &D, sizeof(float));
    out.write((char *)buffer, sizeof(float));
    memcpy(buffer, &ep, sizeof(float));
    out.write((char *)buffer, sizeof(float));
    memcpy(buffer, &max_level, sizeof(float));
    out.write((char *)buffer, sizeof(float));

}

int main()
{
    string inputDir = "./anz_dd_cp/";
    int L, D, ep, max_level, *index, *indptr, *level_offset, *level;
    L = get_num_lines(inputDir + "vect.txt");
    D = get_num_cols(inputDir + "vect.txt");
    level = new int[L];
    indptr = new int[L + 1];
    std::string filename = inputDir + "max_level.txt";
    std::ifstream file(filename.c_str());
    file >> max_level;
    file.close();

    filename = inputDir + "ep.txt";
    file.open(filename.c_str());
    file >> ep;
    file.close();

    filename = inputDir + "level.txt";
    file.open(filename.c_str());
    for (int i = 0; i < L; i++)
    {
        file >> level[i];
    }
    file.close();
    filename = inputDir + "indptr.txt";
    file.open(filename.c_str());
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
    string outDir="./";
    save_1darray(level, L, outDir + "level.bin");
    cout << "level saved" << endl;
    save_1darray(indptr, L + 1, outDir + "indptr.bin");
    cout << "indptr saved" << endl;
    save_1darray(index, indptr[L], outDir + "index.bin");
    cout << "index saved" << endl;
    save_1darray(level_offset, max_level + 1, outDir + "level_offset.bin");
    cout << "level_offset saved" << endl;
    float **arr = new float *[L];
    for (int i = 0; i < L; i++)
    {
        arr[i] = new float[D];
    }
    read2darray(arr, L, D, inputDir + "vect.txt");
    save_2darray(arr, L, D, outDir+"vect.bin");
    save_parameters(L,D,ep,max_level,outDir+"header.bin");
    return 0;
}