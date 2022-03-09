#include <string>
#include <assert.h>
#include <algorithm>
#include <set>
#include <ostream>
#include <iostream>
#include <fstream>
using namespace std;
int main(){
    // string graph_file = "data/8717_31525/edges_out_100_30_20.dat";
    string graph_file = "graph_2.bin";
    // string graph_file = "./pokar.dat";
    // string text_file = "text.txt";
    ofstream myfile;
  myfile.open ("example2.txt");
  int num_nodes = 8717;
  int num_rec = 20;
        std::fstream file(graph_file, std::ios::in | std::ios::binary);
        if(!file){
            std::cout << "File not Found" << std::endl;
            exit(1);
        }
        file.seekg(0);
        int to, from;
        unsigned char line[4];
        for(int e = 0; e < num_nodes; e++){
            file.read((char *)&line, sizeof(line));
            from = (int)line[3] | (int)line[2]<<8 | (int)line[1]<<16 | (int)line[0]<<24; 
            myfile << from <<  ": ";
            for (int d = 0; d < num_rec; d++) {
                file.read((char *)&line, sizeof(line));
                from = (int)line[3] | (int)line[2]<<8 | (int)line[1]<<16 | (int)line[0]<<24; 
                file.read((char *)&line, sizeof(line));
                to = (int)line[3] | (int)line[2]<<8 | (int)line[1]<<16 | (int)line[0]<<24; 
                myfile << from << ' ' << to << ' ';
            }   
            myfile << endl;
        }
        file.close(); 
}