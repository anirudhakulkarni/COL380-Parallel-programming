#include <bits/stdc++.h>
using namespace std;
int get_num_lines(string filename)
{
    ifstream file(filename);
    string line;
    int num_lines = 0;
    while (getline(file, line))
    {
        num_lines++;
    }
    return num_lines;
}
int get_num_cols(string filename)
{
    ifstream file(filename);
    string line;
    int num_cols = 0;
    getline(file, line);
    stringstream ss(line);
    string token;
    while (getline(ss, token, ' '))
    {
        num_cols++;
    }
    return num_cols;
}
void getinputs(string inputDir, int &max_level, int &ep, int *level, int *index, int *indptr, int *level_offset, int **vect, int **user, int &L, int &D, int &U)
{
    string filename = inputDir + "max_level.txt";
    std::ifstream file(filename);
    file >> max_level;
    file.close();
    filename = inputDir + "ep.txt";
    file.open(filename);
    file >> ep;
    file.close();
    L = get_num_lines(inputDir + "level.txt");
    U = get_num_lines(inputDir + "user.txt");
    D = get_num_cols(inputDir + "user.txt");
    level = new int[L];
    filename = inputDir + "level.txt";
    file.open(filename);
    for (int i = 0; i < L; i++)
    {
        file >> level[i];
    }
    file.close();
    vect = new int *[L];
    for (int i = 0; i < L; i++)
    {
        vect[i] = new int[D];
    }
    filename = inputDir + "vect.txt";
    file.open(filename);
    for (int i = 0; i < L; i++)
    {
        for (int j = 0; j < D; j++)
        {
            file >> vect[i][j];
        }
    }
    file.close();
    filename = inputDir + "user.txt";
    file.open(filename);
    index = new int[L + 1];
    for (int i = 0; i < L + 1; i++)
    {
        file >> index[i];
    }
    file.close();
    int indexptr_size = index[L];
    filename = inputDir + "indptr.txt";
    file.open(filename);
    indptr = new int[indexptr_size];
    for (int i = 0; i < indexptr_size; i++)
    {
        file >> indptr[i];
    }
    file.close();
    int level_offset_size = max_level + 1;
    filename = inputDir + "level_offset.txt";
    file.open(filename);
    level_offset = new int[level_offset_size];
    for (int i = 0; i < level_offset_size; i++)
    {
        file >> level_offset[i];
    }
    file.close();
    user = new int *[U];
    for (int i = 0; i < U; i++)
    {
        user[i] = new int[D];
    }
    filename = inputDir + "user.txt";
    file.open(filename);
    for (int i = 0; i < U; i++)
    {
        for (int j = 0; j < D; j++)
        {
            file >> user[i][j];
        }
    }
    file.close();
    return;
}
void printinputs(int max_level, int ep, int *level, int *index, int *indptr, int *level_offset, int **vect, int **user, int L, int D, int U)
{
    cout << "max_level: " << max_level << endl;
    cout << "ep: " << ep << endl;
    cout << "level: ";
    for (int i = 0; i < L; i++)
    {
        cout << level[i] << " ";
    }
    cout << endl;
    cout << "vect: " << endl;
    for (int i = 0; i < L; i++)
    {
        for (int j = 0; j < D; j++)
        {
            cout << vect[i][j] << " ";
        }
        cout << endl;
    }
    cout << "index: ";
    for (int i = 0; i < L + 1; i++)
    {
        cout << index[i] << " ";
    }
    cout << endl;
    cout << "indptr: ";
    for (int i = 0; i < index[L]; i++)
    {
        cout << indptr[i] << " ";
    }
    cout << endl;
    cout << "level_offset: ";
    for (int i = 0; i < max_level + 1; i++)
    {
        cout << level_offset[i] << " ";
    }
    cout << endl;
    cout << "user: " << endl;
    for (int i = 0; i < U; i++)
    {
        for (int j = 0; j < D; j++)
        {
            cout << user[i][j] << " ";
        }
        cout << endl;
    }
    return;
}