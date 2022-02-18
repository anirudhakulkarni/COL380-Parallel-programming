#include "classify.h"
#include <omp.h>
#include <atomic>
int glob = 0;
#define STRIDE 8
static std::atomic <int> freq[1001] __attribute__ ((aligned (32)));
// int get_range(const Ranges&R, int val, int start, int end)                          // {                                                                                   //    if (start > end)return -1;                                                       //    int mid = start + (end - start) / 2;                                             //    if (R[mid].within(val))return mid;                                               //    if (val < R[mid].lo)return get_range(R, val, start, mid);                        //    return get_range(R, val, mid+1, end);                                            // }                                                                                   
Data classify(Data &D, const Ranges &R, unsigned int numt)
{
    assert(numt < MAXTHREADS);
    Counter counts[R.num()];
    Data D2 = Data(D.ndata);
    unsigned int nData = D.ndata;
    unsigned int nRan = R.num();
    unsigned int *rangecount = new unsigned int[R.num()];
    unsigned int sz = nData / numt;
    for(int r = 0; r < R.num(); r+=1) {
        rangecount[r] = 0;
        freq[r] = 0;
   }
#pragma omp parallel num_threads(numt)
{
    int tid = omp_get_thread_num();                                                        
    for(int i = tid; i < nData; i+=numt){
        D.data[i].value = R.range(D.data[i].key);
    }
}
#pragma omp parallel num_threads(numt)
{
    int tid = omp_get_thread_num();
    for(int i = tid; i < nData; i+=numt)
    {
        int v = D.data[i].value;// = R.range(D.data[i].key);
        counts[v].increase(tid);
   }
        
}
        rangecount[0] = 0;
        for(int r = 1; r < R.num(); r+=1) { // For all intervals                                  //rangecount[r] = rangecount[r-1];                                                     
        rangecount[r] = rangecount[r-1];
        for(int t=0; t<numt; t++) // For all threads
        rangecount[r] += counts[r].get(t);
        }
        // for(int i=1; i<nRan; i++) {                                                         //    //std::cout << rangecount[i] << " elements in Range " << i << "\n"; // Debugging statement                                                                              //    rangecount[i] += rangecount[i-1];                                                // }                                                                                                                                                                                                                                                                 // #pragma omp parallel num_threads(numt)                                              // {                                                                                   //    int tid = omp_get_thread_num();                                                  //    for(int r = tid; r < nRan; r+=numt) { // Thread together share-loop through the intervals                                                                               //       int rcount = 0;                                                               //       //#pragma omp for                                                             //       for(int d=0; d<nData; d+=1) // For each interval, thread loops through all of data and                                                                               //          if(D.data[d].value == r) // If the data item is in this interval           //          {                                                                          //             D2.data[rangecount[r-1]+rcount++] = D.data[d]; // Copy it to the appropriate place in D2.                                                                      //             // rcount++;                                                            //             //if (rangecount[r] - rangecount[r-1] == rcount)break;                  //          }                                                                          //    }                                                                                // }                                                                                   // // int freq[nRan];                                                                  // // for(int y = 0; y < nRan; y++)freq[y] = 0;                                        // #pragma omp parallel num_threads(numt)                                              // {                                                                                   //    int tid = omp_get_thread_num();                                                  //    for(int d = tid; d < nData; d+=numt)                                             //    {                                                                                //       //get_range(R, d.data[d].key, 0, nRan);                                       
        //       int r = D.data[d].value;                                                      //       assert(r > 0);                                                                //          D2.data[rangecount[r-1]+freq[r]++] = D.data[d];                            //    }                                                                                // }                                                                                   
        #pragma omp parallel num_threads(numt)
        {
            int tid = omp_get_thread_num();                                                        
            for (uint d = tid * nData / numt; d < ((tid + 1) * nData) / numt; d+=STRIDE)           {   
                for (uint i = d; i < std::min(d+STRIDE, ((tid + 1) * nData) / numt); i++)              { 
                    int r = D.data[i].value;
                    assert(r > 0);
                    D2.data[rangecount[r-1]+freq[r]++] = D.data[i];
                    }
                    }

                  }                                                                                      

return D2;} 