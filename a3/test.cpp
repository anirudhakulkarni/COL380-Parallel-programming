#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

using namespace std::chrono;
// void read1darray(float *arr,int loc, int L, string filename)
// {
//     cout << "reading from txt: " << filename << endl;
//     std::ifstream file(filename.c_str());
//     // file.seekg(loc*sizeof(float));
//     cout << "file opened: " << filename << endl;
//     for (int i = 0; i < L; i++)
//     {
//         file >> arr[i];
//         cout<<arr[i]<<endl;
//     }
//     file.close();
// }
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
int main()
{
    // use mpi to read 2d vector in parallel and save it to a file
    int rank, size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // time taken to read and save
    auto start = high_resolution_clock::now();

    int L = 1305265;
    int D = 768;
    cout << "rank: " << rank << " size: " << size << endl;
    // read in single array of size L/size*D
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
    float *arr = new float[end - loc];
    read1darray(arr, loc, end - loc, "converted/vect.bin");
    // for(int i=0;i<end-loc;i++){
    //     cout<<arr[i]<<" ";
    // }
    // cout<<endl;

    // cout<<"displs: ";
    // for(int i=0;i<size;i++){
    //     cout<<displs[i]<<" ";
    // }
    // cout<<endl;
    // cout<<"counts: ";
    // for(int i=0;i<size;i++){
    //     cout<<counts[i]<<" ";
    // }
    // cout<<"counts: ";
    MPI_Barrier(MPI_COMM_WORLD);
    float *big_arr = new float[L * D];
    MPI_Gatherv(arr, end - loc, MPI_FLOAT, big_arr, counts, displs, MPI_FLOAT, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        for (int i = 0; i < L * D; i++)
        {
            // cout << big_arr[i] << " ";
            // cout << endl;
        }
    }
    MPI_Finalize();
    // print time taken
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << duration.count() / 1000000.0 << " seconds" << std::endl;
}