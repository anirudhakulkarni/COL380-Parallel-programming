#include <bits/stdc++.h>
using namespace std;

class compare
{
public:
    bool operator() (const pair<float, vector<int>> &a, const pair<float, vector<int>> &b){
        return a.first<b.first;
    }
};

#define pq priority_queue <pair<float, vector<int>>, vector<pair<float, vector<int>>>, compare >

int angle[3] = { -45,  0, 45 };
__device__ float  cosine[3] = { 0.70710678, 1, 0.70710678 };
__device__ float  sine[3] = { -0.70710678 , 0, 0.70710678 };

__global__ void matchTemplateGPU_kernel(int* queryImage, float *greyscaleAverageQ, int* dataImage, long long int *dataImagePrefixSum, int r_d, int c_d, int r_q, int c_q, float threshold1, float threshold2, float* anserarray){
    int absi = blockIdx.x * 256 + threadIdx.x;
    int i1 = absi / c_d;
    int j1 = absi % c_d;

    for (int k1 = 0;k1 < 3;k1++){
        float temp = 0;
        if (!(i1 < r_d && j1 < c_d && k1 < 3)) continue;
        anserarray[i1 * c_d * 3 + j1 * 3 + k1] = -1;
        int w = c_q - 1, h = r_q - 1;
        int i2 = i1-int(0*cosine[k1]+w*sine[k1]), j2 = j1+int(w*cosine[k1]-0*sine[k1]), i3 = i1-int(h*cosine[k1]+0*sine[k1]), j3 = j1+int(0*cosine[k1]-h*sine[k1]), i4 = i1-int(h*cosine[k1]+w*sine[k1]), j4 = j1+int(w*cosine[k1]-h*sine[k1]);
        int i_min = min(i1, min(i2, min(i3, i4))), i_max = max(i1, max(i2, max(i3, i4)));
        int j_min = min(j1, min(j2, min(j3, j4))), j_max = max(j1, max(j2, max(j3, j4)));
        int sizeD = (i_max-i_min+1)*(j_max-j_min+1);
        i_min = max(i_min, 0), i_max = min(i_max, r_d-1);
        j_min = max(j_min, 0), j_max = min(j_max, c_d-1);
        float greyscaleAverageD = float(dataImagePrefixSum[i_max*c_d+j_max] - ((i_min-1>=0)? dataImagePrefixSum[(i_min-1)*c_d+j_max]: 0) - ((j_min-1>=0)? dataImagePrefixSum[i_max*c_d+(j_min-1)]: 0) + ((i_min-1>=0 && j_min-1>=0)? dataImagePrefixSum[(i_min-1)*c_d+(j_min-1)]: 0))/sizeD;
        // float greyscaleAverageD=0.0;
        // for(int i=i_min;i<=i_max;i++){
        //     for(int j=j_min;j<=j_max;j++){
        //         int temp=0;
        //         for(int k_=0;k_<3;k_++){
        //             temp+=dataImage[(i) * 3 * c_d + j * 3 + k_];
                
        //         }
        //         greyscaleAverageD+=temp/3;        
        //     }                
        // }
        // greyscaleAverageD/=sizeD;
        float greyscale = abs(greyscaleAverageD - *greyscaleAverageQ);
        if (greyscale >= threshold2){
            continue;
        }
        for (int i = 0; i < r_q; i++){
            for (int j = 0; j < c_q; j++){
                float i_temp = i * cosine[k1] + j * sine[k1], j_temp = j * cosine[k1] - i * sine[k1];
                float i2 = i1 - i_temp, j2 = j1 + j_temp;
                int i2_min = min(i1 - floor(i_temp), i1 - ceil(i_temp)), i2_max = max(i1 - floor(i_temp), i1 - ceil(i_temp)), j2_min = min(j1 + floor(j_temp), j1 + ceil(j_temp)), j2_max = max(j1 + floor(j_temp), j1 + ceil(j_temp));
                float x = i2-i2_min, y = j2-j2_min;
                for (int k = 0; k < 3; k++){
                    float y1 = ((i2_min >= 0 && i2_min < r_d&& j2_min >= 0 && j2_min < c_d) ? dataImage[i2_min * 3 * c_d + j2_min * 3 + k] : 0)*(1-x)*(1-y) + ((i2_max >= 0 && i2_max < r_d&& j2_min >= 0 && j2_min < c_d) ? dataImage[i2_max * 3 * c_d + j2_min * 3 + k] : 0)*x*(1-y) + ((i2_min >= 0 && i2_min < r_d&& j2_max >= 0 && j2_max < c_d) ? dataImage[i2_min * 3 * c_d + j2_max * 3 + k] : 0)*(1-x)*y + ((i2_max >= 0 && i2_max < r_d&& j2_max >= 0 && j2_max < c_d) ? dataImage[i2_max * 3 * c_d + j2_max * 3 + k] : 0)*x*y;
                    float tempdhairya=float(queryImage[(r_q-i-1) * 3 * c_q + j * 3 + k]) - y1;
                    temp+=(tempdhairya*tempdhairya)/ (r_q * c_q * 3.0);
                    // temp += pow(float(queryImage[(r_q-i-1) * 3 * c_q + j * 3 + k]) - y1, 2) / (r_q * c_q * 3.0);

                }
            }
        }
        float ans;
        ans = sqrt(temp);
        if(ans<threshold1){
            anserarray[i1 * c_d * 3 + j1 * 3 + k1] = ans;
            // if(ans<10)
            // printf("%f, %f\n",ans,greyscale);
        }
    }
}


void matchTemplate_GPU(int* queryImage, float *greyscaleAverageQ, int* dataImage, long long int* dataImagePrefixSum, float& threshold1, float& threshold2, int& r_d, int& c_d, int& r_q, int& c_q, float* anserarray){
    matchTemplateGPU_kernel << <(r_d * c_d + 255) / 256, 256 >> > (queryImage, greyscaleAverageQ, dataImage, dataImagePrefixSum, r_d, c_d, r_q, c_q, threshold1, threshold2, anserarray);
}

void readImage_GPU(string path_of_image, int* &Image, long long int* &ImagePrefixSum, int &r,int &c){
  fstream fs(path_of_image, fstream::in);
  fs>>r>>c;
    Image=new int[r*c*3];
    ImagePrefixSum=new long long int[r*c];
  for(int i=0;i<r;i++){
    for(int j=0; j<c; j++){
        int temp = 0;
        for(int k=0; k<3; k++){
            fs>>Image[i*c*3+j*3+k];
            temp+=Image[i*c*3+j*3+k];
        }
        ImagePrefixSum[i*c+j] = ((j-1>=0)? ImagePrefixSum[i*c+(j-1)]: 0) + ((i-1>=0)? ImagePrefixSum[(i-1)*c+j]: 0) - ((i-1>=0 && j-1>=0)? ImagePrefixSum[(i-1)*c+(j-1)]: 0) + temp/3;
    }
  }
}

int main(int argc, char const* argv[])
{
    auto begin = std::chrono::high_resolution_clock::now();
    string path_of_data_image = argv[1];
    string path_of_query_image = argv[2];
    float threshold1 = stof(argv[3]);
    float threshold2 = stof(argv[4]);
    int n = stoi(argv[5]);
    int* dataImage; int* queryImage;
    long long int *dataImagePrefixSum;long long int *queryImagePrefixSum;
    int r_d, c_d, r_q, c_q; 
    readImage_GPU(path_of_data_image, dataImage, dataImagePrefixSum,r_d,c_d);
    readImage_GPU(path_of_query_image, queryImage, queryImagePrefixSum,r_q,c_q);
    float greyscaleAverageQTemp = float(queryImagePrefixSum[(r_q-1)*c_q+(c_q-1)])/(r_q*c_q);
    float *greyscaleAverageQ = &greyscaleAverageQTemp;
    
    float *greyscaleAverageQ_GPU;
    cudaMalloc(&greyscaleAverageQ_GPU,sizeof(float));
    cudaMemcpy(greyscaleAverageQ_GPU,greyscaleAverageQ,sizeof(float),cudaMemcpyHostToDevice);
    
    long long int *dataImagePrefixSum_GPU;
    cudaMalloc(&dataImagePrefixSum_GPU,r_d*c_d*sizeof(long long int));
    cudaMemcpy(dataImagePrefixSum_GPU,dataImagePrefixSum,r_d*c_d*sizeof(long long int),cudaMemcpyHostToDevice);

    int* dataImage_GPU;
    cudaMalloc(&dataImage_GPU, r_d * c_d * 3 * sizeof(int));
    cudaMemcpy(dataImage_GPU, dataImage, r_d * c_d * 3 * sizeof(int), cudaMemcpyHostToDevice);
 
    int* queryImage_GPU;
    cudaMalloc(&queryImage_GPU, r_q * c_q * 3 * sizeof(int));
    cudaMemcpy(queryImage_GPU, queryImage, r_q * c_q * 3 * sizeof(int), cudaMemcpyHostToDevice);
    
    cout << "Time taken for input reading:\t" << (1e-6 * (std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - begin)).count()) << "ms" << endl;

    float* anserarray = new float[r_d * c_d * 3];
    memset(anserarray, 0, sizeof(anserarray));

    float* anserarray_GPU;
    cudaMalloc(&anserarray_GPU, r_d * c_d * 3 * sizeof(float));
    cudaMemcpy(anserarray_GPU, anserarray, r_d * c_d * 3 * sizeof(float), cudaMemcpyHostToDevice);
    cout << "Time taken for alllocate:\t" << (1e-6 * (std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - begin)).count()) << "ms" << endl;

    matchTemplate_GPU(queryImage_GPU, greyscaleAverageQ_GPU, dataImage_GPU, dataImagePrefixSum_GPU, threshold1, threshold2, r_d, c_d, r_q, c_q, anserarray_GPU);
    std::cout << "CUDA error: " << cudaGetErrorString(cudaGetLastError()) << std::endl; // add

    cudaDeviceSynchronize();
    std::cout << "CUDA error: " << cudaGetErrorString(cudaGetLastError()) << std::endl; // add

    cudaMemcpy(anserarray, anserarray_GPU, r_d * c_d * 3 * sizeof(float), cudaMemcpyDeviceToHost);
    
    pq top_n;
    for (int i = 0; i < r_d ; i++) {
        for(int j=0; j<c_d; j++){
            for(int k=0; k<3; k++){
                if(anserarray[i * c_d * 3 + j * 3 + k]!=-1){
                    top_n.push(make_pair(anserarray[i * c_d * 3 + j * 3 + k], vector<int>({i, j, k})));
                    while(top_n.size()>n){
                        top_n.pop();
                    }
                }
            }
        }
    }
    vector<vector<int> > answer(top_n.size());
    // DEBUG
    vector<float> debug_answer(top_n.size());
    // DEBUG
    fstream fs_debug("debug.txt", fstream::out);

    for(int i=answer.size()-1; i>=0; i--){
        answer[i]=top_n.top().second;
        // DEBUG
        debug_answer[i]=top_n.top().first;
        top_n.pop();
    }
    fstream fs("output.txt", fstream::out);
    for(int i=0; i<answer.size(); i++){
        //change
        fs<<r_d-answer[i][0]-1<<" "<<answer[i][1]<<" "<<angle[answer[i][2]]<<endl;
        // DEBUG
        fs_debug<<debug_answer[i]<<" "<<endl;
    }
    // DEBUG
    fs_debug.close();
    fs.close();

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    float duration = (1e-6 * (std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin)).count());
    cout << "Time taken \t" << duration << "ms" << endl;
    std::cout << "CUDA error: " << cudaGetErrorString(cudaGetLastError()) << std::endl; // add

    return 0;

}
