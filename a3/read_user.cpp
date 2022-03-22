#include <fstream>
#include <limits>
#include <iostream>

using namespace std;
void deb(int i){std::cout << i << std::endl;}
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

#define np 4
#define total 16

int main()
{
    fstream files[np];
    int batch_size = total / np;
            std::cout << "Starting" << std::endl;
    for(int rank = 0; rank < np; rank++)
    {
        string filename = "user_test.txt";
        files[rank].open(filename.c_str());
            // std::cout << rank << std::endl;
       GotoLine(files[rank], rank * batch_size);
        // std::cout << rank << std::endl;
        for(int i = rank*batch_size; i < std::min(total, (rank+1)*batch_size); i++)
        {
            // deb(i);continue;
            int x;
            files[rank] >> x;
            std::cout << x << std::endl;
            // break;
        }
    }
    return 0;
}