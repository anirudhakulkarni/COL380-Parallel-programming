#include <string>
#include <mpi.h>
#include <assert.h>
#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
using namespace std;
void writehelper(int u, unsigned char *buffer1, std::ofstream &outfile, int &offset)
{
    unsigned char buffer[4];
    // std::cout << "u: " << u << std::endl;
    // std::cout << "offset: " << offset << " u:" << u << std::endl;
    buffer[3] = (unsigned char)(u & 0xFF);
    buffer[2] = (unsigned char)((u >> 8) & 0xFF);
    buffer[1] = (unsigned char)((u >> 16) & 0xFF);
    buffer[0] = (unsigned char)((u >> 24) & 0xFF);
    // outfile.seekp(offset, std::ios::beg);
    // offset += 4;
    outfile.write((char *)&buffer, sizeof(u));
}

int main(){

}