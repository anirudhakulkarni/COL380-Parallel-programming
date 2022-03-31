#include <bits/stdc++.h>
using namespace std;

unordered_map<int, int> angle = {{-1, -45}, {0, 0}, {1, 45}};
unordered_map<int, float> cosine = {{-1, 0.414}, {0, 1}, {1, 0.414}};
unordered_map<int, float> sine = {{-1, -0.414}, {0, 0}, {1, 0.414}};

float matchTemplateGPU(vector<vector<vector<int>>> &queryImage, vector<vector<vector<int>>> &dataImage, int &i1, int &j1, int &k1){
	long long int temp = 0;
	for(int i=0; i<queryImage.size(); i++){
		for(int j=0; j<queryImage[0].size(); j++){
			for(int k=0; k<3; k++){
				int i2 = i1+int(i*cosine[k]+j*sine[k]), j2 = j1+int(j*cosine[k]-i*sine[k]);
				temp+=pow(queryImage[i][j][k] - ((i2>=0 && i2<dataImage.size() && j2>=0 && j2<dataImage[0].size())? dataImage[i2][j2][k]: 0), 2);
			}
		}
	}
	return sqrt(((float)(temp))/queryImage.size()/queryImage[0].size()/3.0);
}

float filterTemplate(vector<vector<long long int>> &queryImagePrefixSum, vector<vector<long long int>> &dataImagePrefixSum, int &i, int &j, int &k){
	int w = queryImagePrefixSum[0].size() - 1, h = queryImagePrefixSum.size() - 1;
	int i1 = i, j1 = j - int(h*sine[k]), i2 = i + int(w*sine[k] + h*cosine[k]), j2 = j + int(w*cosine[k]);
	if(i1>i2){
		swap(i1, i2);
	}
	if(j1>j2){
		swap(j1, j2);
	}
	int w_d = j2-j1+1, h_d = i2-i1+1;
	i1 = max(i1, 0);
	j1 = max(j1, 0);
	i2 = min(i2, (int)dataImagePrefixSum.size()-1);
	j2 = min(j2, (int)dataImagePrefixSum[0].size()-1);
	return abs(((float)(dataImagePrefixSum[i2][j2] - ((i1-1>=0)? dataImagePrefixSum[i1-1][j1]: 0) - ((j1-1>=0)? dataImagePrefixSum[i1][j1-1]: 0) + ((i1-1>=0 && j1-1>=0)? dataImagePrefixSum[i1-1][j1-1]: 0)))/w_d/h_d-((float)(queryImagePrefixSum[w][h]))/(w+1)/(h+1));
}

void matchTemplateConfig(vector<vector<vector<int>>> &queryImage, vector<vector<long long int>> &queryImagePrefixSum, vector<vector<vector<int>>> &dataImage, vector<vector<long long int>> &dataImagePrefixSum, int &i, int &j, int &k, float &threshold1, float &threshold2, int &n, priority_queue<pair<float, vector<int>> > &top_n){
	float tempThreshold2 = filterTemplate(queryImagePrefixSum, dataImagePrefixSum, i, j, k);
	if(tempThreshold2>=threshold2){
		return;
	}
	float tempThreshold1 = matchTemplateGPU(queryImage, dataImage, i, j, k);
	if(tempThreshold1<threshold1){
		top_n.push({tempThreshold1, {i, j, k}});
		while(top_n.size()>n){
			top_n.pop();
		}
	}
}

void matchTemplate(vector<vector<vector<int>>> &queryImage, vector<vector<long long int>> &queryImagePrefixSum, vector<vector<vector<int>>> &dataImage, vector<vector<long long int>> &dataImagePrefixSum, float &threshold1, float &threshold2, int &n){
	priority_queue<pair<float, vector<int>> > top_n;
	for(int i=0; i<dataImage.size(); i++){
		for(int j=0; j<dataImage[i].size(); j++){
			for(int k=-1; k<=1; k++){
				matchTemplateConfig(queryImage, queryImagePrefixSum, dataImage, dataImagePrefixSum, i, j, k, threshold1, threshold2, n, top_n);
			}
		}
	}
	fstream fs("output.txt", fstream::out);
	int topn = top_n.size();
	vector<vector<int>> output(topn);
	while(top_n.size()){
		output[--topn] = top_n.top().second;
		top_n.pop();
	}
	int m = dataImage.size();
	for(int i=0; i<output.size(); i++){
		fs<<m-output[i][0]-1<<" "<<output[i][1]<<" "<<angle[-1*output[i][2]]<<endl;
	}
	fs.close();
}

void readImage(string path_of_image, vector<vector<vector<int>>> &Image, vector<vector<long long int>> &ImagePrefixSum){
	fstream fs(path_of_image, fstream::in);
	int m, n;
	fs>>m>>n;
	vector<vector<vector<int>>> tempImage(m, vector<vector<int>>(n, vector<int>(3)));
	vector<vector<long long int>> tempImagePrefixSum(m, vector<long long int>(n));
	for(int i=0; i<m; i++){
		for(int j=0; j<n; j++){
			int temp = 0;
			for(int k=0; k<3; k++){
				fs>>tempImage[i][j][k];
				temp+=tempImage[i][j][k];
			}
			tempImagePrefixSum[i][j] = ((j-1>=0)? tempImagePrefixSum[i][j-1]: 0) + ((i-1>=0)? tempImagePrefixSum[i-1][j]: 0) - ((i-1>=0 && j-1>=0)? tempImagePrefixSum[i-1][j-1]: 0) + temp/3.0;
		}
	}
	Image = tempImage;
	ImagePrefixSum = tempImagePrefixSum;
	fs.close();

}

int main(int argc, char const *argv[])
{
	auto begin = std::chrono::high_resolution_clock::now();
	string path_of_data_image = argv[1]; 
	string path_of_query_image = argv[2]; 
	float threshold1 = stof(argv[3]);
	float threshold2 = stof(argv[4]);
	int n = stoi(argv[5]);
	vector<vector<vector<int>>> dataImage, queryImage;
	vector<vector<long long int>> dataImagePrefixSum, queryImagePrefixSum;
	readImage(path_of_data_image, dataImage, dataImagePrefixSum);
	readImage(path_of_query_image, queryImage, queryImagePrefixSum);
	matchTemplate(queryImage, queryImagePrefixSum, dataImage, dataImagePrefixSum, threshold1, threshold2, n);
	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    float duration = (1e-6 * (std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin)).count());
    cout << "Time taken " << duration << "ms" << endl;
	// save the output image

    return 0;
}