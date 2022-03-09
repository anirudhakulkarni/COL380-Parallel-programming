#include <string>
#include <assert.h>
#include "randomizer.hpp"
#include <bits/stdc++.h>
#include <fstream>
#include <string>
#include <assert.h>
#include "randomizer.hpp"
#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
void writehelper(int u, unsigned char *buffer, std::ofstream &outfile, int &offset)
{
    // unsigned char buffer[4];
    std::cout << "u: " << u << std::endl;
    // std::cout << "offset: " << offset << " u:" << u << std::endl;
    buffer[3] = (unsigned char)(u & 0xFF);
    buffer[2] = (unsigned char)((u >> 8) & 0xFF);
    buffer[1] = (unsigned char)((u >> 16) & 0xFF);
    buffer[0] = (unsigned char)((u >> 24) & 0xFF);
    // outfile.seekp(offset, std::ios::beg);
    // offset += 4;
    outfile.write((char *)&buffer, sizeof(u));
}
void readfrombin()
{
    std::ifstream infile("graph.bin", std::ios::binary);
    uint32_t u, v;
    unsigned char buffer[4];
    for (int i = 0; i < 10; i++)
    {
        infile.read((char *)&buffer, sizeof(u));
        u = (int)buffer[3] | (int)buffer[2] << 8 | (int)buffer[1] << 16 | (int)buffer[0] << 24;
        // infile.read((char *)&buffer, sizeof(v));
        // v = (int)buffer[3] | (int)buffer[2] << 8 | (int)buffer[1] << 16 | (int)buffer[0] << 24;
        std::cout << u << std::endl;
    }
    infile.close();
}
// void writehelper(int u, std::ofstream &outfile)
// {
//     unsigned char buffer[4];
//     std::cout << "u: " << u << std::endl;
//     buffer[3] = (unsigned char)(u & 0xFF);
//     buffer[2] = (unsigned char)((u >> 8) & 0xFF);
//     buffer[1] = (unsigned char)((u >> 16) & 0xFF);
//     buffer[0] = (unsigned char)((u >> 24) & 0xFF);
//     outfile.write((char *)&buffer, sizeof(u));
// }
// void writetobin()
// {
//     std::vector<int> myarr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
//     // save to binary file
//     std::ofstream outfile("myfile.bin", std::ios::binary);
//     for (int i = 0; i < myarr.size(); i++)
//     {
//         writehelper(myarr[i], outfile);
//     }
// }
void f()
{
    std::ifstream infile("output_8717_31525_100_30_20_369.dat", std::ios::binary);
    uint32_t u, v;
    unsigned char buffer[4];
    for (int i = 0; i < 50; i++)
    {
        infile.read((char *)&buffer, sizeof(u));
        u = (int)buffer[3] | (int)buffer[2] << 8 | (int)buffer[1] << 16 | (int)buffer[0] << 24;
        // infile.read((char *)&buffer, sizeof(v));
        // v = (int)buffer[3] | (int)buffer[2] << 8 | (int)buffer[1] << 16 | (int)buffer[0] << 24;
        std::cout << u << std::endl;
    }
    infile.close();
}
int main()
{
    std::string graph_file = "data/8717_31525/edges.dat";
    // writetobin();
        std::ofstream outfile("graph.bin", std::ios::binary);
    int f=10;
    writehelper(6969,new unsigned char[4],outfile ,f);

    readfrombin();
    // f();
    return 0;
}