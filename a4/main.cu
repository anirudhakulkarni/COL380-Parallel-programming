#include <bits/stdc++.h>
using namespace std;

// unordered_map<int, int> angle = {{-1, -45}, {0, 0}, {1, 45}};
// unordered_map<int, float> cosine = {{-1, 0.70710}, {0, 1}, {1, 0.70710}};
// unordered_map<int, float> sine = {{-1, -0.70710}, {0, 0}, {1, 0.70710}};

// __device__ int** angle_kernel={{-1, -45}, {0, 0}, {1, 45}};
// __device__ float**  cosine_kernel = {{-1, 0.70710}, {0, 1}, {1, 0.70710}};
// __device__ float**  sine_kernel = {{-1, -0.70710}, {0, 0}, {1, 0.70710}};
// 
__device__ int angle[3] = { -45,  0, 45 };
__device__ float  cosine[3] = { 0.70710, 1, 0.70710 };
__device__ float  sine[3] = { -0.70710 , 0, 0.70710 };

__global__ void matchTemplateGPU_kernel(int* queryImage, int* dataImage, int r_d, int c_d, int r_q, int c_q, float threshold1, float threshold2, int* anserarray)
{
    // int angle[3]={ -45,  0, 45};
    // float  cosine[3] = {0.70710, 1, 0.70710};
    // float  sine[3] = { -0.70710 , 0, 0.70710};
    // DEBUG:
  //   int i1=blockIdx.x*blockDim.x+threadIdx.x;
  //   int j1=blockIdx.y*blockDim.y+threadIdx.y;
  //   int k1=blockIdx.z*blockDim.z+threadIdx.z;
    int absi = blockIdx.x * 256 + threadIdx.x;
    int i1 = absi / c_d;
    int j1 = absi % c_d;
    //   if(i1==0)
    //   printf("%d, %d\n",i1,j1);
    // if (!(i1 < r_d && j1<c_d)){printf("i:%d, j:%d\n",i1,j1);return;}
    for (int k1 = 0;k1 < 3;k1++)
    {
        float temp = 0;
        if (!(i1 < r_d && j1 < c_d && k1 < 3))break;
        anserarray[i1 * c_d * 3 + j1 * 3 + k1] = 1;
        // if(r_d-i1-1==290 ){
        //   printf("%d, %d, %d\n",i1,j1,k1);
        // }
    // calculate greyscale difference
  //   float greyscale=0;
  //   if (greyscale >= threshold2)
  // 	{
  // 		return;
  // 	}
      // greyscale=queryImage[4];
  // 3*i*width+j*3+k
      // temp=temp+1;
        for (int i = 0; i < r_q; i++)
        {
            for (int j = 0; j < c_q; j++)
            {
                for (int k = 0; k < 3; k++)
                {

                    int i2 = i1 - int(i * cosine[k1] + j * sine[k1]), j2 = j1 + int(j * cosine[k1] - i * sine[k1]);
                    temp += pow(queryImage[(r_q - i - 1) * 3 * c_q + j * 3 + k] - ((i2 >= 0 && i2 < r_d&& j2 >= 0 && j2 < c_d) ? dataImage[i2 * 3 * c_d + j2 * 3 + k] : 0), 2);
                    // if(r_d-i1-1==290 && j1==119 &&k==2){
                    //     printf("I: %d,J: %d, K: %d, Temp: %d\n",i,j,k,temp);
                    //   }
                              // temp += pow(queryImage[((i2 >= 0 && i2 < r_d && j2 >= 0 && j2 < c_d) ? dataImage[i2*3*r_d+j2*3+k] : 0), 2);
                    // temp += pow(queryImage[(i )*3*c_q+j*3+k], 2);
                    // float f=queryImage[(r_q - i - 1)*3*c_q+j*3+k]);// - ((i2 >= 0 && i2 < r_d && j2 >= 0 && j2 < c_d) ? dataImage[i2*3*c_d+j2*3+k] : 0);
                    // float f=0.0001;
                    // printf("value: %f",temp);
                    // if(temp>10000)return;
                    // temp+=f;
                    // temp = temp+(f*f)/ r_q / c_q / 3.0;

                }
            }
        }
        float ans;
        //   printf("temp: %f\n",temp);

        ans = sqrt(((float)(temp)) / r_q / c_q / 3.0);
        //   printf("ans: %f",ans);
        if (ans < 30) {
            printf("ANSwer: I: %d, J: %d, k: %d, ans: %f\n", i1, j1, k1, ans);
            // cout<<i<<" "<<j<<" "<<k<<" "<<ans<<endl;
        }
    }
}


void matchTemplate_GPU(int* queryImage, int* dataImage, float& threshold1, float& threshold2, int& r_d, int& c_d, int& r_q, int& c_q, int* anserarray)
{
    // allocate memory for int*** queryImage
    cout << "r_d: " << r_d << " c_d: " << c_d << " r_q: " << r_q << " c_q: " << c_q << endl;
    //   matchTemplateGPU_kernel<<<dim3(r_d/256,c_d,3),256>>>(queryImage, dataImage,r_d,c_d,r_q,c_q,threshold1,threshold2);
    //   matchTemplateGPU_kernel<<<(r_d*c_d*3)/256,256>>>(queryImage, dataImage,r_d,c_d,r_q,c_q,threshold1,threshold2);
    // matchTemplateGPU_kernel<<<dim3(r_d,c_d),3>>>(queryImage, dataImage,r_d,c_d,r_q,c_q,threshold1,threshold2);
    matchTemplateGPU_kernel << <(r_d * c_d + 255) / 256, 256 >> > (queryImage, dataImage, r_d, c_d, r_q, c_q, threshold1, threshold2, anserarray);
}

void readImage_GPU(string path_of_image, int*& Image, int& r, int& c) {
    fstream fs(path_of_image, fstream::in);
    fs >> r >> c;
    Image = new int[r * c * 3];
    for (int i = 0;i < r * c * 3;i++) {
        fs >> Image[i];
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
    // vector<vector<vector<int>>> dataImage, queryImage;
    // long long int **dataImagePrefixSum;long long int **queryImagePrefixSum;
    // vector<vector<long long int>> dataImagePrefixSum, queryImagePrefixSum;
    int r_d, c_d, r_q, c_q;
    readImage_GPU(path_of_data_image, dataImage, r_d, c_d);
    readImage_GPU(path_of_query_image, queryImage, r_q, c_q);



    int* dataImage_GPU;
    cudaMalloc(&dataImage_GPU, r_d * c_d * 3 * sizeof(int));
    //   cout<<"allocagted\n";
    cudaMemcpy(dataImage_GPU, dataImage, r_d * c_d * 3 * sizeof(int), cudaMemcpyHostToDevice);
    //   cout<<"allocagted\n";
    int* queryImage_GPU;
    cudaMalloc(&queryImage_GPU, r_q * c_q * 3 * sizeof(int));
    //   cout<<"allocagted\n";
    cudaMemcpy(queryImage_GPU, queryImage, r_q * c_q * 3 * sizeof(int), cudaMemcpyHostToDevice);
    //   cout<<"allocagted\n";
    cout << "Time taken for input reading:\t" << (1e-6 * (std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - begin)).count()) << "ms" << endl;

    int* anserarray = new int[r_d * c_d * 3];
    memset(anserarray, 0, sizeof(anserarray));

    int* anserarray_GPU;
    cudaMalloc(&anserarray_GPU, r_d * c_d * 3 * sizeof(int));
    // cudaMalloc(&anserarray, r_d * c_d * 3 * sizeof(int));
    // std::cout << "CUDA error: " << cudaGetErrorString(cudaGetLastError()) << std::endl; // add
    cudaMemcpy(anserarray_GPU, anserarray, r_d * c_d * 3 * sizeof(int), cudaMemcpyHostToDevice);
    // memset(anserarray,0,sizeof(anserarray));
    cout << "Time taken for alllocate:\t" << (1e-6 * (std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - begin)).count()) << "ms" << endl;
    std::cout << "CUDA error: " << cudaGetErrorString(cudaGetLastError()) << std::endl; // add

    matchTemplate_GPU(queryImage_GPU, dataImage_GPU, threshold1, threshold2, r_d, c_d, r_q, c_q, anserarray_GPU);
    std::cout << "CUDA error: " << cudaGetErrorString(cudaGetLastError()) << std::endl; // add
    cudaDeviceSynchronize();
    cudaMemcpy(anserarray, anserarray_GPU, r_d * c_d * 3 * sizeof(int), cudaMemcpyDeviceToHost);
    for (int i = 0;i < r_d * c_d * 3;i++) {
        if (!anserarray[i])cout << i << endl;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    float duration = (1e-6 * (std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin)).count());
    cout << "Time taken \t" << duration << "ms" << endl;
    // cudaError_t err = cudaGetLastError();  // add
    std::cout << "CUDA error: " << cudaGetErrorString(cudaGetLastError()) << std::endl; // add

    return 0;
}