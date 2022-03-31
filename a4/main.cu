#include <bits/stdc++.h>
using namespace std;
#define tempThreshold1 2;
#define tempThreshold2 2;

unordered_map<int, int> angle_ = {{-2, -90}, {-1, -45}, {0, 0}, {1, 45}, {2, 90}};
unordered_map<int, float> cos_ = {{-2, 0}, {-1, 0.414}, {0, 1}, {1, 0.414}, {2, 0}};
unordered_map<int, float> sin_ = {{-2, -1}, {-1, -0.414}, {0, 0}, {1, 0.414}, {2, 1}};

float matchTemplateGPU(vector<vector<vector<uint8_t>>> &queryImage, vector<vector<vector<uint8_t>>> &dataImage, int &i, int &j, int &k){

}

float filterTemplate(vector<vector<float>> &queryImagePrefixSum, vector<vector<float>> &dataImagePrefixSum, int &i, int &j, int &k){
	int w = queryImagePrefixSum[0].size(), h = queryImagePrefixSum.size();
	int i1 = i, j1 = j - floor(h*sin_[k]) -1, i2 = i + floor(w*cos_[k] + h*cos_[k]) -1, j2 = j + floor(w*cos_[k]) - 1;
	return abs((dataImagePrefixSum[max(i1, i2)][max(j1, j2)] - dataImagePrefixSum[max(min(i1, i2)-1, 0)][max(min(j1, j2)-1, 0)])/abs(i2-i1+1)/abs(j2-j1+1)-(queryImagePrefixSum[w-1][h-1]/(w)/(h)));
}

void matchTemplateConfig(vector<vector<vector<uint8_t>>> &queryImage, vector<vector<float>> &queryImagePrefixSum, vector<vector<vector<uint8_t>>> &dataImage, vector<vector<float>> &dataImagePrefixSum, int &i, int &j, int &k, float &threshold1, float &threshold2, int &n, priority_queue<pair<int, vector<int>> > &top_n){
	tempThreshold2 = filterTemplate(queryImagePrefixSum, dataImagePrefixSum, i, j, k);
	if(tempThreshold2>=threshold2){
		return;
	}
	tempThreshold1 = matchTemplateGPU(queryImage, dataImage, i, j, k);
	if(tempThreshold1<threshold1){
		top_n.push(make_pair(tempThreshold1, {i, j, k}));
		while(top_n.size()>n){
			top_n.pop();
		}
	}
}

void matchTemplate(vector<vector<vector<uint8_t>>> &queryImage, vector<vector<float>> &queryImagePrefixSum, vector<vector<vector<uint8_t>>> &dataImage, vector<vector<float>> &dataImagePrefixSum, float &threshold1, float &threshold2, int &n){
	priority_queue<pair<int, vector<int>> > top_n;
	for(int i=0; i<dataImage.size(); i++){
		for(int j=0; j<dataImage[i].size(); j++){
			for(int k=-2; k<=2; k++){
				matchTemplateConfig(queryImage, dataImage, i, j, k, threshold1, threshold2, k, top_n)
			}
		}
	}
	fstream fs("output.txt", fstream::out);
	int topn = top_n.size();
	vector<vector<int>> output(topn);
	while(top_n.size()){
		output[--topn] = top_n.top();
		top_n.pop();
	}
	int m = dataImage.size();
	for(int i=0; i<output.size(); i++){
		fs<<m-output[i][0]-1<<" "<<output[i][1]<<" "<<angle_[output[i][2]]<<endl;
	}
	fs.close();
}

void readImage(string path_of_image, vector<vector<vector<uint8_t>>> &Image, vector<vector<uint8_t>> &ImagePrefixSum){
	fstream fs(path_of_image, fstream::in);
	int m, n;
	fs>>m>>n;
	Image.resize(m);
	ImagePrefixSum.resize(m);
	vector<vector<vector<uint8_t>>> tempImage(m, vector<vector<uint8_t>>(n, vector<uint8_t>(3)))
	vector<vector<float>> tempImagePrefixSum(m, vector<vector<float>>(n, 0));
	for(int i=0; i<m; i++){
		for(int j=0; j<n; j++){
			float temp = 0.0;
			for(int k=0; k<3; k++){
				fs>>tempImage[i][j][k];
				temp+=tempImage[i][j][k];
			}
			tempImagePrefixSum[i][j] = tempImagePrefixSum[i][max(j-1, 0)] + tempImagePrefixSum[max(i-1, 0)][j] + temp/3.0;
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
	vector<vector<vector<uint8_t>>> dataImage, queryImage;
	vector<vector<float>> dataImagePrefixSum, queryImagePrefixSum;
	readImage(path_of_data_image, dataImage, dataImagePrefixSum);
	readImage(path_of_query_image, queryImage, queryImagePrefixSum);
	matchTemplate(queryImage, queryImagePrefixSum, dataImage, dataImagePrefixSum, threshold1, threshold2, n);
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    float duration = (1e-6 * (std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin)).count());
    cout << "Time taken " << duration << "ms" << endl;
	return 0;
}