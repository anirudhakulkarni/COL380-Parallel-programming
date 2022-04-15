// #include "includes.hpp"
#include<bits/stdc++.h>
using namespace std;


int intersection(std::vector<std::string> &v1,
                                      std::vector<std::string> &v2){
    std::vector<std::string> v3;

    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());

    std::set_intersection(v1.begin(),v1.end(), v2.begin(),v2.end(), back_inserter(v3));
    return v3.size();
}

pair<double,double> precision(string pred_file,string gt_file,int k){
    fstream pred_stream(pred_file.c_str(),ios::in);
    fstream gt_stream(gt_file.c_str(),ios::in);

    string temp;
    vector<vector<string>> pred;
    vector<vector<string>> gt;
    string s;
    int i=0;
    // int j=0;
    while(getline(pred_stream,temp)){
        stringstream ss(temp);
        pred.push_back(vector<string>());
        while(ss>>s){
            // j++;
            pred[i].push_back(s);
        }
        i++;
    }
    i=0;
    pred_stream.close();
    while(getline(gt_stream,temp)){
        stringstream ss(temp);
        gt.push_back(vector<string>());
        // j=0;
        while(ss>>s){
            // j++;
            gt[i].push_back(s);
        }
        i++;
    }
    gt_stream.close();

    double precision=0.0;
    double recall = 0.0;
    if(pred.size()!=gt.size()){
        cerr<<"Number of predictions: "<<pred.size()<<"Number of ground truth Labels: "<<gt.size()<<endl;
        return {0.0,0.0};
    }
    for(int i=0;i<pred.size();i++){
        precision += (double)intersection(pred[i],gt[i])/k;
        recall += (double)intersection(pred[i],gt[i])/gt[i].size();
    }
    return {precision/(double)pred.size(),recall/(double)pred.size()};
}

int main(int argc, char **argv){
    string pred_file=argv[1];
    string gt_file=argv[2];
    int k = stoi(argv[3]);
    pair<double,double> predictions = precision(pred_file,gt_file,k);
    cout<<"Precision: "<<predictions.first<<endl;
    cout<<"Recall: "<<predictions.second<<endl;
}